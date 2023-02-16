#include "calibrationwindow.h"

using namespace cv;

/**
 * @brief CalibrationWindow::CalibrationWindow Displays calibration window
 * @param parent
 */
CalibrationWindow::CalibrationWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // GUI creation

    lblTick = new QLabel(this);
    lblTickReverse = new QLabel(this);

    lblTick->move(10,10);
    lblTickReverse->move(width() - lblTickReverse->width() - 10,height() - lblTickReverse->height() - 10);

    // Prepares the tick images
    QImage myImage;
    QImage myImageReverse;
    myImage.load("://img//Tick.png");
    myImageReverse.load("://img//TickReverse.png");
    QPixmap pm = QPixmap::fromImage(myImage);
    QPixmap pmReverse = QPixmap::fromImage(myImageReverse);

    lblTick->setPixmap(pm.scaled(50, 50, Qt::KeepAspectRatio));
    lblTickReverse->setPixmap(pmReverse.scaled(50, 50, Qt::KeepAspectRatio));
    lblTick->adjustSize();
    lblTickReverse->adjustSize();
    lblTick->show();
    lblTickReverse->show();

    this->setStyleSheet("background-color: black;");
}

CalibrationWindow::~CalibrationWindow()
{

}

/**
 * @brief CalibrationWindow::keyReleaseEvent Add the possibility to take of the toolbar or not
 * @param event
 */
void CalibrationWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_C)
    {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        show();
    }

    if(event->key() == Qt::Key_D)
    {
        setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
        show();
    }
}

/**
 * @brief CalibrationWindow::resizeEvent Triggers the resize event
 * @param event
 */
void CalibrationWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   lblTickReverse->move(width() - lblTickReverse->width() - 10,height() - lblTickReverse->height() - 10);
}
