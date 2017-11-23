#include "CropperTestMainWindow.h"
#include "ui_CropperTestMainWindow.h"

#include <QFileDialog>
#include <QPainter>

#include "ImageProcessing.h"

/**
 * Default constructor
 *
 * @param parent - pointer to parent object
 */
CropperTestMainWindow::CropperTestMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pMainWindow(new Ui::CropperTestMainWindow),
    m_pProcessing(new ImageProcessing),
    m_pInputImageViewer(0),
    m_pFilteredImageViewer(0),
    m_pCroppedImageViewer(0)
{
    setProperty("Filter/ModeName", "-None-");
    setProperty("Filter/KernelSize", 3);
    setProperty("Filter/ShrinkFactor", 20);
    setProperty("Filter/Threshold1", 20);
    setProperty("Filter/Threshold2", 220);
    setProperty("Segmentation/MinPixel", 200);
    setProperty("Segmentation/Threshold", 75);

    setupUi();
}


/**
 * Destructor
 */
CropperTestMainWindow::~CropperTestMainWindow() {
    delete m_pMainWindow;
}

/**
 * Process current loaded image and display results
 */
void CropperTestMainWindow::processImage() {
    if (m_pInputImageViewer) m_pInputImageViewer->clear();
    if (m_pFilteredImageViewer) m_pFilteredImageViewer->clear();
    if (m_pCroppedImageViewer) m_pCroppedImageViewer->clear();

    if (m_OriginalImage.isNull())
        return;

    int iShrinkFactor = property("Filter/ShrinkFactor").toInt();
    m_InputImage = m_pProcessing->makeGray(m_OriginalImage.scaledToWidth(m_OriginalImage.width() / (iShrinkFactor > 1 ? iShrinkFactor : 1), Qt::SmoothTransformation));

    if (m_pInputImageViewer) {
        m_pInputImageViewer->setPixmap(QPixmap::fromImage(m_OriginalImage.scaled(m_pInputImageViewer->contentsRect().size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    }

    QImage tempImage;

    filterImage(m_InputImage, tempImage);
    if (m_pFilteredImageViewer) {
        m_pFilteredImageViewer->setPixmap(QPixmap::fromImage(tempImage.scaled(m_pFilteredImageViewer->contentsRect().size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    }

    cropImage(tempImage, tempImage);
    if (m_pCroppedImageViewer) {
        m_pCroppedImageViewer->setPixmap(QPixmap::fromImage(tempImage.scaled(m_pCroppedImageViewer->contentsRect().size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    }
}

/**
 * Start file open dialog and load selected image
 */
void CropperTestMainWindow::on_OpenImageButton_clicked() {
    QString fname = QFileDialog::getOpenFileName(0, tr("Load image"), "./", "*.*");
    if (fname.length())  {
        QImage image(fname);
        m_OriginalImage = image;
        m_InputImage    = QImage();
        if (!image.isNull()) {
            setWindowTitle(fname);
        }
    }

    processImage();
}


/**
 * Filter given image and return shrinked and filtered result
 *
 * @param input     - original image to shrink and filter
 * @param filtered  - resulting image
 */
void CropperTestMainWindow::filterImage(const QImage &input, QImage &filtered) {
    QString mode      = property("Filter/ModeName").toString().toLower().trimmed();
    int iKernelSize   = property("Filter/KernelSize").toInt();

    if (mode == "binarize") {
        m_pProcessing->binarizeFilter(input, filtered, property("Filter/Threshold2").toInt());
    } else if (mode == "contrast") {
        m_pProcessing->contrastStretchingFilter(input, filtered, property("Filter/Threshold1").toInt(), property("Filter/Threshold2").toInt());
    } else if (mode == "mean") {
        m_pProcessing->meanFilter(input, filtered, iKernelSize);
    } else if (mode == "median") {
        m_pProcessing->medianFilter(input, filtered, iKernelSize);
    } else if (mode == "gauss") {
        m_pProcessing->gaussFilter(input, filtered, iKernelSize);
    } else {
        filtered = input;
    }
}


/**
 * Crop given image and return the cropped down result
 *
 * @param input     - shrinked (and filtered) image to crop
 * @param cropped   - resulting image
 */
void CropperTestMainWindow::cropImage(const QImage &input, QImage &cropped) {
    int iShrinkFactor = property("Filter/ShrinkFactor").toInt() > 1 ? property("Filter/ShrinkFactor").toInt() : 1;

    QRect region;
    m_pProcessing->cropNeighbour(input, region, property("Segmentation/MinPixel").toInt(), property("Segmentation/Threshold").toInt(), iShrinkFactor);

    cropped = m_OriginalImage.copy(region);

    // draw region into image viewer
    if (m_pInputImageViewer) {
        QImage dest = m_OriginalImage.scaled(m_pInputImageViewer->contentsRect().size(), Qt::KeepAspectRatio, Qt::FastTransformation);

        QPixmap  pm(*m_pInputImageViewer->pixmap());
        QPainter p(&pm);
        double scaleFacX = double(dest.width()) / double(m_OriginalImage.width());
        double scaleFacY = double(dest.height()) / double(m_OriginalImage.height());
        double scaleFac = scaleFacX > scaleFacY ? scaleFacX : scaleFacY;
        int    ox = pm.width() - dest.width();
        int    oy = pm.height() - dest.height();

        p.setBrush(QBrush(Qt::green));
        p.setPen(Qt::green);

        int y = static_cast<int>(double(region.y()) * scaleFac);
        p.drawLine(ox, oy + y, pm.width() - (ox + 1), oy + y);

        y = static_cast<int>(double(region.y() + region.height()) * scaleFac);
        p.drawLine(ox, oy + y, pm.width() - (ox + 1), oy + y);

        int x = static_cast<int>(double(region.x()) * scaleFac);
        p.drawLine(ox + x, oy, ox + x, oy + pm.height() - 1);

        x = static_cast<int>(double(region.x() + region.width()) * scaleFac);
        p.drawLine(ox + x, oy, ox + x, oy + pm.height() - 1);

        m_pInputImageViewer->setPixmap(pm);
    }
}


/**
 * Init all gui components
 */
void CropperTestMainWindow::setupUi() {
    m_pMainWindow->setupUi(this);

    m_pInputImageViewer     = findChild<QLabel*>("InputImageViewer");
    m_pFilteredImageViewer  = findChild<QLabel*>("FilteredImageViewer");
    m_pCroppedImageViewer   = findChild<QLabel*>("CroppedImageViewer");

    QSpinBox* pSpinBox      = findChild<QSpinBox*>("KernelSizeSpinBox");
    if (pSpinBox) setProperty("Filter/KernelSize", pSpinBox->value());

    pSpinBox                = findChild<QSpinBox*>("ShrinkFactorSpinBox");
    if (pSpinBox) setProperty("Filter/ShrinkFactor", pSpinBox->value());

    pSpinBox                = findChild<QSpinBox*>("FilterThreshold1SpinBox");
    if (pSpinBox) setProperty("Filter/Threshold1", pSpinBox->value());

    pSpinBox                = findChild<QSpinBox*>("FilterThreshold2SpinBox");
    if (pSpinBox) setProperty("Filter/Threshold2", pSpinBox->value());
}
