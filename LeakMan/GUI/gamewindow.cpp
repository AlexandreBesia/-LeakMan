#include "gamewindow.h"

using namespace cv;

/**
 * @brief GameWindow::GameWindow Displays the camera game window
 * @param parent
 */
GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent)
{

    // Creates timer
    t = new QTimer(this);

    // GUI creation

    lblVideoDisplay = new QLabel("", this);
    lblVideoDisplay->move(20, height() / 2 - lblVideoDisplay->height() / 2);
    lblVideoDisplay->resize(width() / 2, height() / 2);

    lblVideoDisplayTitle = new QLabel("Vue de la caméra", this);
    lblVideoDisplayTitle->move(20, (height() / 2 - lblVideoDisplay->height() / 2) - 20);

    lblInstuction = new QLabel("Légendes:\n Rouge = saut\n Vert = sprint\n Bleu = demi-tour", this);
    lblInstuction->adjustSize();
    lblInstuction->move(width() - lblInstuction->width() -lblInstuction->width() / 4 , height() / 2);

    t->setInterval(25);

    // Starts timer
    t->start();
    connect(t, &QTimer::timeout, this, &GameWindow::capture);
}

GameWindow::~GameWindow()
{
}

/**
 * @brief GameWindow::capture Capture the camera flow and displays it in hsv color, can recognize wanted colors
 */
void GameWindow::capture()
{
    cap >> image;

    img = QImage((unsigned char*) image.data, image.cols, image.rows, QImage::Format_BGR888);

    // displays the cam in gray
    // Recognize wanted colors
    for(int i=0;i<img.width();i++)
    {
        for(int j=0;j<img.height();j++)
        {
            QColor color = img.pixelColor(i,j);
            img.setPixelColor(i, j, color);
            color = color.convertTo(QColor::Hsv);
            int valueColor=color.value();

            if( isRed(color) || isGreen(color) || isBlue(color) || isYellow(color)) // Red, Green , Blue, Yellow
            {
             color.setHsv(color.hue(),color.saturation(),valueColor);
            }
            else
            {
                color.setHsv(0,0,valueColor);
            }

            img.setPixelColor(i, j, color);
        }
    }
    lblVideoDisplay->setPixmap(QPixmap::fromImage(img));
}

/**
 * @brief GameWindow::resizeEvent Resizes as wanted
 * @param event
 */
void GameWindow::resizeEvent(QResizeEvent* event)
{
   lblVideoDisplay->resize(width() / 2, height() / 2);
   lblVideoDisplay->move(20, height() / 2 - lblVideoDisplay->height() / 2);
   lblVideoDisplay->setScaledContents(true);

   lblVideoDisplayTitle->move(20, (height() / 2 - lblVideoDisplay->height() / 2) - 20);

   lblInstuction->adjustSize();
   lblInstuction->move(width() - lblInstuction->width() -lblInstuction->width() / 4 , height() / 2);
}

/**
 * @brief GameWindow::getImg Get the image
 * @return
 */
QImage GameWindow::getImg()
{
    return img;
}

/**
 * @brief GameWindow::isRed Checks if the color is red
 * @param color
 * @return
 */
bool GameWindow::isRed(QColor color)
{
    return color.hue() >= 320;
}

/**
 * @brief GameWindow::isGreen Checks if the color is green
 * @param color
 * @return
 */
bool GameWindow::isGreen(QColor color)
{
    return color.hue() <= 175 && color.hue() >= 170;
}

/**
 * @brief GameWindow::isBlue Checks if the color is blue
 * @param color
 * @return
 */
bool GameWindow::isBlue(QColor color)
{
    return color.hue() <= 220 && color.hue() >= 200;
}

/**
 * @brief GameWindow::isYellow Checks if the color is yellow
 * @param color
 * @return
 */
bool GameWindow::isYellow(QColor color)
{
    return color.hue() <= 60 && color.hue() >= 45;
}

/**
 * @brief GameWindow::setNbCamera Sets the camera choosen numero
 * @param nbCamera
 */
void GameWindow::setNbCamera(int nbCamera)
{
    cap = cv::VideoCapture(nbCamera);

    // Checks if a camera is opened
    if (!cap.isOpened())
    {
        qDebug() << "cannot open camera";
    }
}
