#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QColor>
#include <QRadioButton>
#include <QGroupBox>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "calibrationwindow.h"
#include "state.h"
#include "stateid.h"

class CalibrationState : public State
{
    Q_OBJECT
public:
    explicit CalibrationState(StateStack* stateStack, QMainWindow* window);
    ~CalibrationState();

    void capture();
    void play();
    void quit();

    int getNbCamera();

private:

    int nbCamera;

    bool isQuit;

    cv::Mat image;
    cv::VideoCapture cap;

    QTimer *t;

    QLabel* lblVideoDisplay;
    QLabel* lblVideoDisplayTitle;
    QLabel* lblInstuction;

    QPushButton* btnQuit;
    QPushButton* btnPlay;

    CalibrationWindow w;

private slots:

    void radioChangeCamera1();
    void radioChangeCamera2();
};
