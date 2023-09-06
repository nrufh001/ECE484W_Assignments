#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->ContrastSlider, SIGNAL(valueChanged(int)), this, SLOT(updateContrastValue(int))); // Connects the slider value to labels
    connect(ui->BrightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateBrightnessValue(int)));
    connect(ui->BrightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateBrightness(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateContrastValue(int value)
{
    ui->BrightnessValueLabel->setText(QString::number(value)); // updates the UI with the brightness value and places it in the label
}

void MainWindow::updateBrightnessValue(int value)
{
    ui->ContrastValueLabel->setText(QString::number(value)); // updates the UI with the contrast value and places it in the label
}

void MainWindow::updateBrightness(int value)
{
    // qDebug() << "Slider Value:" << value;

    // Load the current image from the label
    QPixmap currentPixmap = ui->label->pixmap();
    QImage image = currentPixmap.toImage(); // grabbing a capture of the image

    double factor = static_cast<double>(value) / 100.0; // calculating a factor ranges from 0 (darkest) to 2.55 (whitest)

    // Now lets go through the entire image
    for (int y = 0; y < image.height(); ++y) // we'll start with height
    {
        for (int x = 0; x < image.width(); ++x) // next is width
        {
            QRgb pixel = image.pixel(x, y); // grab the pixel in the image
            int red = qRed(pixel) * factor; // multiply the red in the image by the factor
            int green = qGreen(pixel) * factor; // multiply the green in the image by the factor
            int blue = qBlue(pixel) * factor; // multiply the blue in the image by the factor

            // Ensure values stay within the 0-255 range - don't want this to go over or under 0 to 255
            red = qBound(0, red, 255);
            green = qBound(0, green, 255);
            blue = qBound(0, blue, 255);

            image.setPixel(x, y, qRgb(red, green, blue)); // now lets set the image with those altered pixel values
        }
    }

    // And finally update the label with the adjusted image
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_pushButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                "C://",
                                                "Bitmap File (*.bmp);;All files (*.*)");
    QPixmap picture(file);
    ui->label->setPixmap(picture);
}
