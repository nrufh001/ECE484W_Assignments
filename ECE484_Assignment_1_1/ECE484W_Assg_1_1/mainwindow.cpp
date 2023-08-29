#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QPixmap picture("E:/Documents/484W_Assg_1/test1.bmp");
    //ui->label->setPixmap(picture);
    //TESTING PHASE
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                "C://",
                                                "Bitmap File (*.bmp);;All files (*.*)");
    QMessageBox::information(this, tr("File Name"), "You have selected\n" + file);

    QPixmap picture(file);
    ui->label->setPixmap(picture);
}


void MainWindow::on_pushButton_2_clicked() {
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                "C://",
                                                "Bitmap File (*.bmp);;All files (*.*)");
    QMessageBox::information(this, tr("File Name"), "You have selected as the overlay\n" + file);

    QPixmap picture(file);
    ui->label_2->setPixmap(picture);
}
