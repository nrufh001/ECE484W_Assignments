#include "mainwindow.h"
#include "./ui_mainwindow.h"

QUdpSocket udpSocket;

int brightnessCurrentValue = 0;
int contrastCurrentValue = 0;

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
    connect(ui->transferImageButton2, &QPushButton::clicked, this, &MainWindow::transferOverlayImage);
    connect(ui->pushButton_open_overlay, &QPushButton::clicked, this, &MainWindow::OpenOverlay);

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
    QPixmap currentPixmap = ui->label->pixmap(); // Load the current image from the label
    QImage image = currentPixmap.toImage(); // grabbing a capture of the image

    double factor = static_cast<double>(value); // calculating a factor ranges from -1.27 (darkest) to 1.27 (whitest)

    currentBrightness = value;

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

    modifiedImage = QPixmap::fromImage(image);

    // just throw all values together
    QString brightnessAndContrast = QString("%1").arg(value, 2, 10, QChar('0')) + // just makes sure that whenever < 10 we add a 0
                                    QString("%1").arg(currentContrast, 2, 10, QChar('0')); // ensures that we send 4 bits at all time
    QByteArray data = brightnessAndContrast.toUtf8();
    udpSocket.writeDatagram(data, QHostAddress(ipaddress), 80); // send data
    qDebug() << data;
}

void MainWindow::updateContrast(int value)
{
    QPixmap currentPixmap = ui->label->pixmap(); // Load the current image from the label
    QImage image = currentPixmap.toImage(); // grabbing a capture of the image

    double factor = static_cast<double>(value); // this is from the slider
    double newFactor;

    //int remappedValue = 0; // for remapping the -127-128 contrast values to a better 0-99

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

    modifiedImage = QPixmap::fromImage(image);

    // Convert values from -127-128 to 0-99 bc that's the range we need for the LED's
    //remappedValue = ((value + 127) * (99 - 0)) / (128 + 127) + 0;
    // currentContrast = remappedValue
    currentContrast = value;

    // just throw all values together
    QString brightnessAndContrast = QString("%1").arg(currentBrightness, 2, 10, QChar('0')) +
                                    QString("%1").arg(value, 2, 10, QChar('0'));
    QByteArray data = brightnessAndContrast.toUtf8();
    udpSocket.writeDatagram(data, QHostAddress(ipaddress), 80);
    qDebug() << data;
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
    originalImage = picture;

    // gonna send an initial data packet to get everything going
    QString data = QString("%1").arg(currentBrightness, 2, 10, QChar('0')) +
                                    QString("%1").arg(currentContrast, 2, 10, QChar('0'));
    QByteArray packet = data.toUtf8();
    udpSocket.writeDatagram(packet, QHostAddress(ipaddress), 80);
    qDebug() << packet;
}

void MainWindow::transferOverlayImage()
{
    // set up UDP socket
    QUdpSocket udpSocket;
    udpSocket.bind(QHostAddress(ipaddress), port);

    // convert the image
    QImage image = overlayImage.toImage();

    // convert the image
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");

    int totalSize = imageData.size();
    int bytesSent = 0;
    QByteArray done = "transferred_overlay"; // set end transferred message so program knows when transfer is done

    qDebug() << "totalSize: " << totalSize;

    while (bytesSent < totalSize) {
        QByteArray chunk = imageData.mid(bytesSent, 1024);
        udpSocket.writeDatagram(chunk, QHostAddress(ipaddress), port);
        bytesSent += chunk.size();
        qDebug() << "chunk: " << chunk.size() << " --> " << "bytesSent: " << bytesSent;
    }

    udpSocket.writeDatagram(done, QHostAddress(ipaddress), port);
}

void MainWindow::OpenOverlay() {
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                "C://",
                                                "Bitmap File (*.bmp);;All files (*.*)");
    QPixmap picture(file);
    ui->label_overlay->setPixmap(picture);
    overlayImage = picture;
}
