/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QSlider *ContrastSlider;
    QSlider *BrightnessSlider;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *ContrastSliderLabel;
    QLabel *BrightnessSliderLabel;
    QLabel *BrightnessValue;
    QLabel *ContrastValue;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        ContrastSlider = new QSlider(centralwidget);
        ContrastSlider->setObjectName("ContrastSlider");
        ContrastSlider->setGeometry(QRect(140, 390, 160, 16));
        ContrastSlider->setMaximum(255);
        ContrastSlider->setSliderPosition(128);
        ContrastSlider->setOrientation(Qt::Horizontal);
        BrightnessSlider = new QSlider(centralwidget);
        BrightnessSlider->setObjectName("BrightnessSlider");
        BrightnessSlider->setEnabled(true);
        BrightnessSlider->setGeometry(QRect(430, 390, 160, 16));
        BrightnessSlider->setMaximum(255);
        BrightnessSlider->setSliderPosition(128);
        BrightnessSlider->setOrientation(Qt::Horizontal);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(130, 10, 511, 361));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(660, 170, 80, 24));
        ContrastSliderLabel = new QLabel(centralwidget);
        ContrastSliderLabel->setObjectName("ContrastSliderLabel");
        ContrastSliderLabel->setGeometry(QRect(170, 430, 161, 16));
        BrightnessSliderLabel = new QLabel(centralwidget);
        BrightnessSliderLabel->setObjectName("BrightnessSliderLabel");
        BrightnessSliderLabel->setGeometry(QRect(460, 430, 131, 16));
        BrightnessValue = new QLabel(centralwidget);
        BrightnessValue->setObjectName("BrightnessValue");
        BrightnessValue->setGeometry(QRect(480, 410, 49, 16));
        ContrastValue = new QLabel(centralwidget);
        ContrastValue->setObjectName("ContrastValue");
        ContrastValue->setGeometry(QRect(180, 410, 49, 16));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QString());
        pushButton->setText(QCoreApplication::translate("MainWindow", "Open Picture", nullptr));
        ContrastSliderLabel->setText(QCoreApplication::translate("MainWindow", "Contrast Slider", nullptr));
        BrightnessSliderLabel->setText(QCoreApplication::translate("MainWindow", "Brightness Slider", nullptr));
        BrightnessValue->setText(QString());
        ContrastValue->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
