#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QColor>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "state.h"
#include "stateid.h"
#include "notGUI/block.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    QImage getImg();

    bool isRed(QColor);
    bool isGreen(QColor);
    bool isBlue(QColor);
    bool isYellow(QColor);
    void setNbCamera(int nbCamera);

private:
    void resizeEvent(QResizeEvent* event);
    void capture();

    cv::Mat image;
    cv::VideoCapture cap;
    QTimer *t;

    QLabel* lblVideoDisplay;
    QLabel* lblVideoDisplayTitle;
    QLabel* lblInstuction;

    QImage img;
};
