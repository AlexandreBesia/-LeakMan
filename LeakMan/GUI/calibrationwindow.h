#pragma once

#include <QMainWindow>
#include <qdebug.h>
#include <QKeyEvent>
#include <QLabel>
#include <QHBoxLayout>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

class CalibrationWindow : public QMainWindow
{
    Q_OBJECT

public:
    CalibrationWindow(QWidget *parent = nullptr);
    ~CalibrationWindow();

     QLabel* lblTick;
     QLabel* lblTickReverse;

private:
    void keyReleaseEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent* event);
};
