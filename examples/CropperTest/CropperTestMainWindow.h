#ifndef CROPPERTESTMAINWINDOW_H
#define CROPPERTESTMAINWINDOW_H

/**@file
 * Cropper test gui tool
 *
 * Author:  Christian Daehn (c) Schwerin 2015
 */

#include <QVariant>
#include <QLabel>
#include <QImage>
#include <QMainWindow>

namespace Ui {
    class CropperTestMainWindow;
}

class ImageProcessing;

/**
 * Mainwindow of cropper test tool
 */
class CropperTestMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CropperTestMainWindow(QWidget *parent = 0);
    ~CropperTestMainWindow();

public slots:
    void        processImage();

protected slots:
    void        on_OpenImageButton_clicked();
    void        on_FilterComboBox_currentTextChanged(const QString& text) { setProperty("Filter/ModeName", text); processImage(); }
    void        on_KernelSizeSpinBox_valueChanged(int i)   { setProperty("Filter/KernelSize", i); processImage(); }
    void        on_ShrinkFactorSpinBox_valueChanged(int i) { setProperty("Filter/ShrinkFactor", i); processImage(); }
    void        on_FilterThreshold1SpinBox_valueChanged(int i) { setProperty("Filter/Threshold1", i); processImage(); }
    void        on_FilterThreshold2SpinBox_valueChanged(int i) { setProperty("Filter/Threshold2", i); processImage(); }
    void        on_ThresholdSpinBox_valueChanged(int i)    { setProperty("Segmentation/Threshold", i); processImage(); }
    void        on_MinPixelSpinBox_valueChanged(int i)     { setProperty("Segmentation/MinPixel", i); processImage(); }

    void        filterImage(const QImage& input, QImage& filtered);
    void        cropImage(const QImage& input, QImage& cropped);

    void        setupUi();

private:
    Ui::CropperTestMainWindow*  m_pMainWindow;          ///< pointer to mainwindow
    ImageProcessing*            m_pProcessing;          ///< object for filter and cropping operations
    QLabel*                     m_pInputImageViewer;    ///< label to display input image
    QLabel*                     m_pFilteredImageViewer; ///< label to display filtered image
    QLabel*                     m_pCroppedImageViewer;  ///< label to display cropped image
    QImage                      m_InputImage;           ///< original image to process
    QImage                      m_OriginalImage;        ///< original image to process
};

#endif // CROPPERTESTMAINWINDOW_H
