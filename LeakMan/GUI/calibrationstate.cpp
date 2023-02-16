#include "calibrationstate.h"

/**
 * @brief CalibrationState::CalibrationState Displays calibration screen and camera window
 * @param stateStack
 * @param window
 */
CalibrationState::CalibrationState(StateStack* stateStack, QMainWindow* window) : State(stateStack, window)
{
    // Creates timer
    t = new QTimer(this);

    // GUI creation

    lblVideoDisplay = new QLabel("", this);
    lblVideoDisplay->move(20, height() / 2 - lblVideoDisplay->height() / 2);
    lblVideoDisplay->resize(width() / 2, height() / 2);

    lblVideoDisplayTitle = new QLabel("Vue de la caméra", this);
    lblVideoDisplayTitle->move(20, (height() / 2 - lblVideoDisplay->height() / 5) - 20);

    lblInstuction = new QLabel("Veuillez déplacer la caméra jusqu'à ce que les deux marques rouges soit visible dans la fenêtre de retour.", this);
    lblInstuction->adjustSize();
    lblInstuction->move(width() - lblInstuction->width() -lblInstuction->width() / 2 , height() / 4);

    btnQuit = new QPushButton("Quitter", window);
    btnQuit->move(width() - btnQuit->width() - 10,height() - btnQuit->height() - 10);
    btnQuit->show();

    btnPlay = new QPushButton("Jouer", window);
    btnPlay->move(width() - btnPlay->width() - 10,height() - (btnPlay->height() * 2) - 20);
    btnPlay->show();

    QGroupBox *groupBox = new QGroupBox(tr("Cameras"), this);

    QRadioButton *radioCamera1 = new QRadioButton(tr("&Camera 1"), this);
    radioCamera1->move(10, 20);
    QRadioButton *radioCamera2 = new QRadioButton(tr("C&amera 2"), this);
    radioCamera2->move(10, 40);

    radioCamera1->setChecked(true);

    connect(radioCamera1, &QRadioButton::toggled , this, &CalibrationState::radioChangeCamera1);
    connect(radioCamera2, &QRadioButton::toggled , this, &CalibrationState::radioChangeCamera2);

    connect(btnQuit, &QPushButton::clicked, this, &CalibrationState::quit);
    connect(btnPlay, &QPushButton::clicked, this, &CalibrationState::play);

    t->setInterval(15);

    // Starts the timer
    t->start();

    cap = cv::VideoCapture(0);
    nbCamera = 0;

    // Checks if a camera is opened
    if (!cap.isOpened())
    {
        qDebug() << "cannot open camera";
    }

    connect(t, &QTimer::timeout, this, &CalibrationState::capture);

    w.show();

    lblVideoDisplay->show();

}

CalibrationState::~CalibrationState()
{

}

/**
 * @brief CalibrationState::capture Read the camera and displays video
 */
void CalibrationState::capture()
{
    cap >> image;

    lblVideoDisplay->setPixmap(QPixmap::fromImage(QImage((unsigned char*) image.data, image.cols, image.rows, QImage::Format_BGR888)));
}

/**
 * @brief CalibrationState::play Starts game
 */
void CalibrationState::play()
{
    w.close();

    delete(btnPlay);
    delete(btnQuit);

    cap.~VideoCapture();

    requestStatePush(StateID::Game); // Change current state to game
}

/**
 * @brief CalibrationState::quit Return to main menu
 */
void CalibrationState::quit()
{
    w.close();

    delete(btnPlay);
    delete(btnQuit);

    requestStateRemove(StateID::Calibration);
    requestStatePush(StateID::Menu);
}

/**
 * @brief CalibrationState::radioChangeCamera1 Change camera to camera 1
 */
void CalibrationState::radioChangeCamera1()
{
    cap = cv::VideoCapture(0);
    nbCamera = 0;

    // Checks if a camera is opened
    if (!cap.isOpened())
    {
        qDebug() << "cannot open camera";
    }
}

/**
 * @brief CalibrationState::radioChangeCamera2 Change camera to camera 2
 */
void CalibrationState::radioChangeCamera2()
{
    cap = cv::VideoCapture(1);
    nbCamera = 1;

    // Checks if a camera is opened
    if (!cap.isOpened())
    {
        qDebug() << "cannot open camera";
    }
}

/**
 * @brief CalibrationState::getNbCamera Gets camera chosen
 * @return
 */
int CalibrationState::getNbCamera()
{
    return nbCamera;
}
