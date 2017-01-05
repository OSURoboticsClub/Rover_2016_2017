#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "commgen.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_serial = new SerialHandler();
    m_controller = new ControllerHandler();

    threadarray.push(m_serial);
    threadarray.push(m_controller);
    _serialRunning = false;
}

MainWindow::~MainWindow()
{

}



void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    qDebug() << "start close";

    threadarray.clear();
    delete ui;

    qDebug() << "closed";
    event->accept();
}
