#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    f_haveJoystick = input.initInput(0);
    if (!f_haveJoystick)
    {
        this->setWindowTitle(QString("%1 :: %2").arg(this->windowTitle()).arg("Joystick not found"));
    }
    else
    {
        this->setWindowTitle(QString("%1 :: %2").arg(this->windowTitle()).arg("Joystick present"));
    }

    // refresh timer interval (reads new values from joystick)
    if (f_haveJoystick)
    {
        tmr.setInterval(15);
        connect(&tmr,SIGNAL(timeout()),this,SLOT(readJoystickState()));
        tmr.start();
    }
}

void MainWindow::readJoystickState()
{
    if (!input.updateState()) return;

    // Update main axes
    ui->leftYAxis->setValue((input.getLeftY()+1.0f)*50.0f);
    ui->leftXAxis->setValue((input.getLeftX()+1.0f)*50.0f);
    ui->rightYAxis->setValue((input.getRightY()+1.0f)*50.0f);
    ui->rightXAxis->setValue((input.getRightX()+1.0f)*50.0f);

    // Update other axis
    ui->leftThrottle->setValue((input.getLeftThrottle()+1.0f)*50.0f);
    ui->rightThrottle->setValue((input.getRightThrottle()+1.0f)*50.0f);
    ui->axisS1->setValue((input.getS1()+1.0f)*50.0f);
    ui->axisS2->setValue((input.getS2()+1.0f)*50.0f);

    // Update SA button state (3-way button)
    if(input.isKeyPressed(1)) {
        ui->btnSA->setIcon((QIcon)":/resButtonIcons/Up Arrow.png");
    } else if (input.isKeyPressed(2)) {
        ui->btnSA->setIcon((QIcon)":/resButtonIcons/Down Arrow.png");
    } else {
        ui->btnSA->setIcon(QIcon());
    }

    // Update SB button state (3-way button)
    if(input.isKeyPressed(3)) {
        ui->btnSB->setIcon((QIcon)":/resButtonIcons/Up Arrow.png");
    } else if (input.isKeyPressed(4)) {
        ui->btnSB->setIcon((QIcon)":/resButtonIcons/Down Arrow.png");
    } else {
        ui->btnSB->setIcon(QIcon());
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
