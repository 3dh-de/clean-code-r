#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QObject>
#include <QVector>
#include <QHash>


/**
 * Class for image processing algorithms like filters and segmenters
 */
class ImageProcessing
{
public:
    ImageProcessing();
    ~ImageProcessing();

    QImage          makeGray(const QImage& src);

    void            binarizeFilter(const QImage& src, QImage& dest, unsigned iThreshold = 120);
    void            contrastStretchingFilter(const QImage& src, QImage& dest, unsigned iMinThreshold = 15, unsigned iMaxThreshold = 240);
    void            meanFilter(const QImage& src, QImage& dest, unsigned iKernelSize = 3);
    void            medianFilter(const QImage& src, QImage& dest, unsigned iKernelSize = 3);
    void            gaussFilter(const QImage& src, QImage& dest, unsigned iKernelSize = 3);
    void            cropNeighbour(const QImage& inputImage, QRect &croppedRegion, int iMinPixel, int iThreshold, int scaleFactor = 1);

private:
    /**
     * Data to store a symmetric 2D convolution kernel
     */
    struct ConvolutionKernel {
        int                     divider;    ///< factor to divide kernel sum
        int                     width;       ///< matrix width 3, 5, 9 ...
        QVector<QVector<int> >  matrix;     ///< kernel matrix

        /// Resize matrix to given width
        void setMatrixWidth(int w) {
            width = w;
            matrix.clear();
            matrix.resize(width);
            for (int i = 0; i < width; ++i) {
                matrix[i].resize(width);
            }
        }
    };

    QHash<int,ConvolutionKernel> m_GaussKernel;  ///< hash of available gauss kernel with key = kernel size/factor and value = kernel matrix
};

#endif // IMAGEPROCESSING_H
