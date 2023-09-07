#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connects the slider value to labels
    connect(ui->ContrastSlider, SIGNAL(valueChanged(int)), this, SLOT(updateContrastValue(int)));
    connect(ui->ContrastSlider, SIGNAL(valueChanged(int)), this, SLOT(updateContrast(int)));
    connect(ui->BrightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateBrightnessValue(int)));
    connect(ui->BrightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateBrightness(int)));
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
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

    QPixmap currentPixmap = ui->label->pixmap(); // Load the current image from the label
    QImage image = currentPixmap.toImage(); // grabbing a capture of the image

    double factor = static_cast<double>(value); // calculating a factor ranges from -1.27 (darkest) to 1.27 (whitest)

    qDebug() << factor;
    // Now lets go through the entire image
    for (int y = 0; y < image.height(); ++y) // we'll start with height
    {
        for (int x = 0; x < image.width(); ++x) // next is width
        {
            QRgb pixel = image.pixel(x, y); // grab the pixel in the image
            int red = qRed(pixel) + factor; // add the red in the image by the factor
            int green = qGreen(pixel) + factor; // add the green in the image by the factor
            int blue = qBlue(pixel) + factor; // add the blue in the image by the factor

            // Ensure values stay within the 0-255 range - don't want this to go over or under 0 to 255
            red = qBound(0, red, 255);
            green = qBound(0, green, 255);
            blue = qBound(0, blue, 255);

            image.setPixel(x, y, qRgb(red, green, blue)); // now lets set the image with those altered pixel values
        }
    }
    ui->label_2->setPixmap(QPixmap::fromImage(image)); // and update the NEW image with the brightness
}

void MainWindow::updateContrast(int value)
{
    QPixmap currentPixmap = ui->label->pixmap(); // Load the current image from the label
    QImage image = currentPixmap.toImage(); // grabbing a capture of the image

    double factor = static_cast<double>(value); // this is from the slider
    double newFactor;

    int red;
    int green;
    int blue;

    qDebug() << factor;
    // Now lets go through the entire image
    for (int y = 0; y < image.height(); ++y) // we'll start with height
    {
        for (int x = 0; x < image.width(); ++x) // next is width
        {
            QRgb pixel = image.pixel(x, y); // grab the pixel in the image
            newFactor = (259 * (factor + 255)) / (255 * (259 - factor));

            red = (newFactor * (qRed(pixel)) - 128) + 128;

            green = (newFactor * (qGreen(pixel)) - 128) + 128;

            blue = (newFactor * (qBlue(pixel)) - 128) + 128;
            // Ensure values stay within the 0-255 range - don't want this to go over or under 0 to 255
            red = qBound(0, red, 255);
            green = qBound(0, green, 255);
            blue = qBound(0, blue, 255);

            image.setPixel(x, y, qRgb(red, green, blue)); // now lets set the image with those altered pixel values
        }
    }
    ui->label_2->setPixmap(QPixmap::fromImage(image)); // and update the NEW image with the contrast
}

void MainWindow::saveImage()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save Image", "", "Bitmap File (*.bmp)"); // lets get the file path

    if (filePath.isEmpty() == false) { // make sure that the path isn't empty
        QPixmap currentPixmap = ui->label_2->pixmap(); // create a pix map so we can save it as .bmp
        if (currentPixmap.isNull() == false) { // make sure that there is actually an image to save
            currentPixmap.save(filePath, "BMP"); // save the pixmap as a bmp file
        }
    }
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
