#include "ImageProcessing.h"

#include <QImage>
#include <QTime>
#include <QPainter>
#include <QMatrix>

#include <algorithm>

using namespace std;


/**
 * Create color table for 8bit palette images (greyscale)
 */
QVector<QRgb> initGrayscaleRgbTable(void) {
    QVector<QRgb> table; table.resize(256);
    for (int i = 0; i < 256; ++i) {
        table[i] = qRgb(i, i, i);
    }
    return table;
}


/**
 * Default constructor
 */
ImageProcessing::ImageProcessing() {
    ConvolutionKernel temp;

    // 3x3 Gauss kernel
    temp.setMatrixWidth(3);
    temp.divider = 16;
    temp.matrix[0][0] = 1; temp.matrix[0][1] = 2; temp.matrix[0][2] = 1;
    temp.matrix[1][0] = 2; temp.matrix[1][1] = 4; temp.matrix[1][2] = 2;
    temp.matrix[2][0] = 1; temp.matrix[2][1] = 2; temp.matrix[2][2] = 1;

    m_GaussKernel[3] = temp;

    // 5x5 Gauss kernel
    temp.setMatrixWidth(5);
    temp.divider = 179;
    temp.matrix[0][0] = 0; temp.matrix[0][1] = 0; temp.matrix[0][2] = 1; temp.matrix[0][3] = 0; temp.matrix[0][4] = 0;
    temp.matrix[1][0] = 0; temp.matrix[1][1] = 8; temp.matrix[1][2] =21; temp.matrix[1][3] = 8; temp.matrix[1][4] = 0;
    temp.matrix[2][0] = 1; temp.matrix[2][1] =21; temp.matrix[2][2] =59; temp.matrix[2][3] =21; temp.matrix[2][4] = 1;
    temp.matrix[3][0] = 0; temp.matrix[3][1] = 8; temp.matrix[3][2] =21; temp.matrix[3][3] = 8; temp.matrix[3][4] = 0;
    temp.matrix[4][0] = 0; temp.matrix[4][1] = 0; temp.matrix[4][2] = 1; temp.matrix[4][3] = 0; temp.matrix[4][4] = 0;

    m_GaussKernel[5] = temp;
}


/**
 * Destructor
 */
ImageProcessing::~ImageProcessing(void) {

}


/**
 * Convert given image to a greyscaled 8 Bit image
 *
 * @param src   - image to convert
 *
 * @return image with color table of format QImage::Format_Indexed8
 */
QImage ImageProcessing::makeGray(const QImage &src) {
    QImage dest = src.convertToFormat(QImage::Format_Indexed8, Qt::AutoColor);
    dest.setColorTable(initGrayscaleRgbTable());
    return dest;
}


/**
 * Binarize image by given threshold
 *
 * This filter modifies the color table only.
 *
 * @param src           - source image
 * @param dest          - destination image to store the filtered pixels
 * @param iThreshold    - set all greyvalues below this color to black and all above to white
 */
void ImageProcessing::binarizeFilter(const QImage &src, QImage &dest, unsigned iThreshold) {
    dest = src;

    QVector<QRgb> colorTable = initGrayscaleRgbTable();
    for (unsigned i = 0; i < 256; ++i) {
        if (i < iThreshold) {
            colorTable[i] = qRgb(0, 0, 0);
        } else {
            colorTable[i] = qRgb(255, 255, 255);
        }
    }
    dest.setColorTable(colorTable);
}


/**
 * Enhance image contrast by normalizing image between the given thresholds
 *
 * This filter modifies the color table only.
 *
 * @param src           - source image
 * @param dest          - destination image to store the filtered pixels
 * @param iMinThreshold - minimum darkest greyvalue
 * @param iMaxThreshold - maximum brightest greyvalue
 */
void ImageProcessing::contrastStretchingFilter(const QImage &src, QImage &dest, unsigned iMinThreshold, unsigned iMaxThreshold) {
    dest = src;

    int           greyValue  = 0;
    QVector<QRgb> colorTable = initGrayscaleRgbTable();

    for (unsigned i = 0; i < 256; ++i) {
        if (i <= iMinThreshold) {
            greyValue = 0;
        } else if (i >= iMaxThreshold) {
            greyValue = 255;
        } else {
            greyValue = (i - iMinThreshold) * (255 / (iMaxThreshold - iMinThreshold)) + 1;
            greyValue = qBound(0, greyValue, 255);
        }
        colorTable[i] = qRgb(greyValue, greyValue, greyValue);
    }

    dest.setColorTable(colorTable);
}


/**
 * Mean filter to reduce influence of dark pixels
 *
 * This filter uses the param @c iKernelSize as kernel size to get the
 * mean value of neighbour pixels and stores the resulting image
 * in @c src.
 *
 * @param src           - source image
 * @param dest          - destination image to store the filtered pixels
 * @param iKernelSize   - kernel size (num of neighbour pixels to evaluate) works best for 3..9
 */
void ImageProcessing::meanFilter(const QImage &src, QImage &dest, unsigned iKernelSize) {
    dest = src;  // dest image must have same size as src

    unsigned     ks2        = iKernelSize / 2;
    unsigned     kPixels    = iKernelSize * iKernelSize;
    int          sx         = src.bytesPerLine();
    int          sy         = src.height();
    uchar       *destBits   = dest.bits();
    const uchar *srcBits    = src.bits();

    for (unsigned y = ks2; y < sy - ks2; ++y) {
        for (unsigned x = ks2; x < sx - ks2; ++x) {
            // store pixel values of kernel
            unsigned sum = 0;
            for (unsigned i = 0; i < iKernelSize; ++i) {
                for (unsigned j = 0; j < iKernelSize; ++j) {
                    sum += srcBits[(y + j - ks2) * sx + (x + i - ks2)];
                }
            }
            // set current pixel to median
            sum /= kPixels;
            destBits[y * sx + x] = static_cast<uchar>(sum > 255 ? 255 : sum);
        }
    }
}


/**
 * Modified median filter to reduce influence of dark pixels
 *
 * This filter uses the param @c iKernelSize as kernel size to get the
 * median value of neighbour pixels and stores the resulting image
 * in @c src.
 *
 * Other than the original median filter this one takes the brightest
 * pixel instead of the mid pixel value. Thus bright areas are gained
 * up and dark pixels are suppressed. This modification helps to crop
 * images by searching for bright regions.
 *
 * @param src           - source image
 * @param dest          - destination image to store the filtered pixels
 * @param iKernelSize   - kernel size (num of neighbour pixels to evaluate) works best for 3..9
 */
void ImageProcessing::medianFilter(const QImage &src, QImage &dest, unsigned iKernelSize) {
    dest = src;  // dest image must have same size as src

    unsigned     ks2        = iKernelSize / 2;
    int          sx         = src.bytesPerLine();
    int          sy         = src.height();
    uchar       *destBits   = dest.bits();
    const uchar *srcBits    = src.bits();
    int          valuesSize = iKernelSize * iKernelSize;
    QVector<uchar> values;
    values.resize(valuesSize);

    for (unsigned y = ks2; y < sy - ks2; ++y) {
        for (unsigned x = ks2; x < sx - ks2; ++x) {
            // store pixel values of kernel
            for (unsigned i = 0; i < iKernelSize; ++i) {
                for (unsigned j = 0; j < iKernelSize; ++j) {
                    values[i * iKernelSize + j] = srcBits[(y + j - ks2) * sx + (x + i - ks2)];
                }
            }
            // sort the pixel by greyscale value
            qSort(values);

            // set current pixel to Median + 1
            destBits[y * sx + x] = values[ks2 + 1];
        }
    }
}


/**
 * Gauss filter to smooth image
 *
 * This filter uses the param @c iKernelSize as kernel size to calculate the
 * gaussian value of neighbour pixels and stores the resulting image
 * in @c src.
 *
 * @param src           - source image
 * @param dest          - destination image to store the filtered pixels
 * @param iKernelSize   - kernel size (num of neighbour pixels to evaluate) works best for 3..9
 */
void ImageProcessing::gaussFilter(const QImage &src, QImage &dest, unsigned iKernelSize) {
    dest = src;  // dest image must have same size as src

    if (m_GaussKernel.size() < 2) {
        qCritical("no gauss kernel available!");
        return;
    }
    if (!m_GaussKernel.contains(iKernelSize)) {
        qWarning("invalid gauss kernel %dx%d given! falling back to kernel 3x3!", iKernelSize);
        iKernelSize = 3;
    }

    unsigned     ks2        = iKernelSize / 2;
    int          sx         = src.bytesPerLine();
    int          sy         = src.height();
    uchar       *destBits   = dest.bits();
    const uchar *srcBits    = src.bits();
    unsigned     kernelSum  = 0;

    for (unsigned y = ks2; y < sy - ks2; ++y) {
        for (unsigned x = ks2; x < sx - ks2; ++x) {
            kernelSum  = 0;
            // store pixel values of kernel
            for (unsigned i = 0; i < iKernelSize; ++i) {
                for (unsigned j = 0; j < iKernelSize; ++j) {
                    kernelSum += m_GaussKernel[iKernelSize].matrix[i][j] * static_cast<unsigned>(srcBits[(y + j - ks2) * sx + (x + i - ks2)]);
                }
            }
            // set current pixel to calculated gauss value
            kernelSum /= m_GaussKernel[iKernelSize].divider;
            destBits[y * sx + x] = qBound(0, (int)kernelSum, 255);
        }
    }
}


/**
 * Crop based on pixel neighbour relationships
 */
void ImageProcessing::cropNeighbour(const QImage& inputImage, QRect& croppedRegion, int iMinPixel, int iThreshold, int scaleFactor) {
    if (inputImage.isNull())
        return;

    QTime ts; ts.start();

    // calc bounding rect of labels
    QImage        dest = inputImage;
    QVector<int>  colorLUT;

    // lookup table for grayscale value of single pixel
    QVector<QRgb> colorTable = dest.colorTable();
    if (inputImage.depth() != 8 || colorTable.size() != 256) {
        qWarning("cropper: only images with 8 Bit depth supported! converting image into grayscale format.");
        dest = makeGray(inputImage);
        colorTable = dest.colorTable();
    }
    foreach (QRgb rgb, colorTable) {
        colorLUT.append(qGray(rgb));
    }

    iMinPixel /= scaleFactor;

    // min contrast != 0  !!!!
    iThreshold = (iThreshold < 1) ? 1 : iThreshold;

    int width   = dest.width() - 1;
    int height  = dest.height() - 1;

    int padding = dest.bytesPerLine() - dest.width();
    int lineBytes = dest.bytesPerLine();

    uchar *pY0 = dest.bits();
    uchar *pY1 = pY0 + dest.bytesPerLine() * dest.height() - 1;

    uchar *pX0 = dest.bits();
    uchar *pX1 = pX0 + dest.bytesPerLine() * dest.height() - (padding + 1);

    int countY0 = 0;
    int countY1 = 0;
    int countX0 = 0;
    int countX1 = 0;

    // y direction

    int iStartY = -1;
    int iStopY  = -1;

    int offsY0 = -1;
    int offsY1 = -1;

    for (int y0 = 0, y1 = height; y0 < height - iMinPixel; ++y0, --y1) {
        countY0 = 0;
        countY1 = 0;

        for (int x = 0; x < width; ++x) {
            if (offsY0 < 0) {
                if (colorLUT[*pY0] < iThreshold)
                    countY0 = 0;
                else if (++countY0 >= iMinPixel)
                    offsY0 = y0;
            }
            if (offsY1 < 0) {
                if (colorLUT[*pY1] < iThreshold)
                    countY1 = 0;
                else if (++countY1 >= iMinPixel)
                    offsY1 = y1;
            }

            ++pY0;
            --pY1;
        }

        pY0 += padding;
        pY1 -= padding;

        if (offsY0 != -1) {
            iStartY = (iStartY < 0 || offsY0 < iStartY) ? offsY0 : iStartY;
            offsY0 = -1;
        }
        if (offsY1 != -1) {
            iStopY = (iStopY < 0 || offsY1 > iStopY) ? offsY1 : iStopY;
            offsY1 = -1;
        }
    }

    // x direction

    int iStartX = -1;
    int iStopX  = -1;

    int offsX0  = -1;
    int offsX1  = -1;

    for (int x0 = 0, x1 = width; x0 < width - iMinPixel; ++x0, --x1) {
        countX0 = 0;
        countX1 = 0;

        for (int y = 0; y < height; ++y) {
            uchar *npX0 = pX0 + y * lineBytes;
            uchar *npX1 = pX1 - y * lineBytes;

            if (offsX0 < 0) {
                if (colorLUT[*npX0] < iThreshold)
                    countX0 = 0;
                else if (++countX0 >= iMinPixel)
                    offsX0 = x0;
            }
            if (offsX1 < 0) {
                if (colorLUT[*npX1] < iThreshold)
                    countX1 = 0;
                else if (++countX1 >= iMinPixel)
                    offsX1 = x1;
            }
        }

        ++pX0;
        --pX1;

        if (offsX0 != -1) {
            iStartX = (iStartX < 0 || offsX0 < iStartX) ? offsX0 : iStartX;
            offsX0 = -1;
        }
        if (offsX1 != -1) {
            iStopX = (iStopX < 0 || offsX1 > iStopX) ? offsX1 : iStopX;
            offsX1 = -1;
        }
    }

    // DEBUG:
    qDebug("cropping state: startX=%d stopX=%d startY=%d stopY=%d width=%d", iStartX, iStopX, iStartY, iStopY, width);

    if (iStartX < 0 && iStopX < 0 && iStartY < 0 && iStopY < 0) {
        croppedRegion = QRect(0, 0, scaleFactor * dest.width(), scaleFactor * dest.height());
    } else {
        iStartX = (iStartX > 0) ? scaleFactor * iStartX : 0;
        iStopX = (iStopX <= width) ? scaleFactor * (iStopX + 1) : 0;

        iStartY = (iStartY > 0) ? scaleFactor * iStartY : 0;
        iStopY = (iStopY <= height) ? scaleFactor * (iStopY + 1) : 0;

        iStopX = (iStopX < 1) ? dest.width() : iStopX;
        iStopX -= iStartX;
        iStopY = (iStopY < 1) ? dest.height() : iStopY;
        iStopY -= iStartY;

        croppedRegion = QRect(iStartX, iStartY, iStopX, iStopY);
    }

    qDebug("cropped image from %dx%d to %dx%d at pos (%d,%d)", inputImage.width() * scaleFactor, inputImage.height() * scaleFactor, croppedRegion.width(), croppedRegion.height(), croppedRegion.x(), croppedRegion.y());
}
