#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->ContrastSlider, SIGNAL(valueChanged(int)), this, SLOT(updateContrastValue(int))); // Connects the slider value to labels
    connect(ui->BrightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateBrightnessValue(int)));
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
    ui->ContrastSliderLabel->setText(QString::number(value)); // updates the UI with the contrast value and places it in the label
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
