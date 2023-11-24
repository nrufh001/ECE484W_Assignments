#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QLabel>
#include <QUdpSocket>
#include <iostream>
#include <QBuffer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void updateContrastValue(int value);
    void updateBrightnessValue(int value);
    void updateBrightness(int value);
    void updateContrast(int value);
    void saveImage();
    void transferOverlayImage();
    void OpenOverlay();

private:
    Ui::MainWindow *ui;
    int currentBrightness = 0;
    int currentContrast = 0;
    QPixmap modifiedImage;
    QPixmap originalImage;
    QPixmap overlayImage;
    int port = 12345;
    QString ipaddress = "192.168.0.151";
};
#endif // MAINWINDOW_H
