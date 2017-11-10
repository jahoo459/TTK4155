#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->horizontalSlider->setMinimum(0);
    this->ui->horizontalSlider->setMaximum(1000);

    this->ui->dial->setMinimum(0);
    this->ui->dial->setMaximum(1000);

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

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug() << "Button clicked!";
    this->ui->pushButton_fire->setStyleSheet("* { background-color: rgb(255,125,100) }");
    this->buttonState = 0;
    this->ui->lcdNumber_Solenoid->display(buttonState);
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "Button clicked!";
    this->ui->pushButton_fire->setStyleSheet("* { background-color: rgb(125,255,100) }");
    this->buttonState = 1;

    this->ui->lcdNumber_Solenoid->display(buttonState);
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "Position " << event->pos();
    this->mouseX_position = event->pos().x();

    if(mouseX_position > 1000) mouseX_position = 1000;
    if(mouseX_position < 0) mouseX_position = 0;

    this->ui->lcdNumber_motor->display(mouseX_position);
    this->ui->horizontalSlider->setValue(this->mouseX_position);
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    wheelPosition += delta / 4;
    if(wheelPosition > 1000) wheelPosition = 1000;
    if(wheelPosition < 0) wheelPosition = 0;

    this->ui->dial->setValue(wheelPosition);
    this->ui->lcdNumber_Servo->display(wheelPosition);
    qDebug() << "Wheel: " << wheelPosition;
    emit gameStateChanged(this->mouseX_position, this->wheelPosition, this->buttonState);
}

