#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->horizontalSlider->setMinimum(0);
    this->ui->horizontalSlider->setMaximum(100);

    this->ui->dial->setMinimum(0);
    this->ui->dial->setMaximum(100);

    this->wheelPosition = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePortSelection(QList<COM_PORT_INFO> ports)
{
    this->com_ports_list = ports;
    foreach(COM_PORT_INFO item, ports)
    {
        this->ui->comboBox_port_select->addItem(item.name);
    }
}

void MainWindow::on_pushButton_port_select_clicked()
{
    emit comPortSelected(this->com_ports_list.at(this->ui->comboBox_port_select->currentIndex()).name);
}

void MainWindow::on_comboBox_port_select_currentIndexChanged(int index)
{
    this->ui->label_description->setText(com_ports_list.at(this->ui->comboBox_port_select->currentIndex()).description);
}

void MainWindow::updateConnStatus(QString msg)
{
    this->ui->label_status->setText(msg);
}

void MainWindow::logInfo(QString msg)
{
    this->ui->label_status->setText(msg);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "Position " << event->pos();
    int currMousePos = event->pos().x();

    if(currMousePos > 1000) mouseX_position = 100;
    else if(currMousePos < 0) mouseX_position = 0;
    else mouseX_position = (uint8_t)(currMousePos/10);

    this->ui->lcdNumber_motor->display(mouseX_position);
    this->ui->horizontalSlider->setValue(this->mouseX_position);
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState, MOTOR_ID);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    int currWheelPos = wheelPosition;

    if(currWheelPos + delta / 4 > 100) currWheelPos = 100;
    else if (currWheelPos + delta / 4 < 0) currWheelPos = 0;
    else currWheelPos += delta / 4;

    wheelPosition = (uint8_t)currWheelPos;

    this->ui->dial->setValue(wheelPosition);
    this->ui->lcdNumber_Servo->display(wheelPosition);
    //qDebug() << "Wheel: " << wheelPosition;
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState, SERVO_ID);
}


void MainWindow::on_pushButton_fire_pressed()
{
    qDebug() << "Button clicked!";
    this->buttonState = 1;
    //this->ui->lcdNumber_Solenoid->display(buttonState);
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState, SOLENOID_ID);
    this->buttonState = 0;

    if(flagPL)
    {
        this->ui->pushButton_fire->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.33 rgba(0, 0, 0, 255), stop:0.34 rgba(255, 30, 30, 255), stop:0.66 rgba(255, 0, 0, 255), stop:0.67 rgba(255, 255, 0, 255), stop:1 rgba(255, 255, 0, 255)); color: rgb(255, 255, 255)");
        flagPL = false;
    }
    else
    {
        this->ui->pushButton_fire->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 255, 255), stop:0.495 rgba(255, 255, 255, 255), stop:0.505 rgba(255, 0, 0, 255), stop:1 rgba(255, 0, 0, 255)); color: rgb(0, 0, 0); color: rgb(0, 0, 0)");
        flagPL = true;
    }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    this->mouseX_position = (uint8_t)position;
    this->ui->lcdNumber_motor->display(position);
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState, MOTOR_ID);
}

void MainWindow::on_dial_sliderMoved(int position)
{
    this->wheelPosition = (uint8_t)position;
    this->ui->lcdNumber_Servo->display(position);
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState, SERVO_ID);
}

void MainWindow::on_pushButton_fire_released()
{
    qDebug() << "Button released";
    this->buttonState = 0;
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState, SOLENOID_ID);
}
