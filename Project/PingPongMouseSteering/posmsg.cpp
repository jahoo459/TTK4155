#include "posmsg.h"

PosMsg::PosMsg()
{
    listSerialPorts();
}

PosMsg::~PosMsg()
{
    closeConnection();
}

int PosMsg::getPosX()
{
    return this->posX;
}

int PosMsg::getButtonState()
{
    return this->buttonState;
}

int PosMsg::getPosWheel()
{
    return this->posWheel;
}


void PosMsg::openConnection(QString portName)
{
    serial.setPortName(portName);
    serial.setBaudRate(9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::TwoStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    if (serial.open(QIODevice::ReadWrite)) {
        qDebug() << "Connected";
        emit connStatusChanged("Connected!");
    } else {
        qDebug() <<"Error" << serial.errorString();
        emit connStatusChanged("Error while connecting!");
    }
}

void PosMsg::closeConnection()
{
    if (serial.isOpen())
        serial.close();
    emit connStatusChanged("Disconnected");
}

void PosMsg::sendData(QByteArray data)
{
    serial.write(data);
}

QByteArray PosMsg::receiveData()
{
    QByteArray data = serial.readAll();
    return data;
}

void PosMsg::updatePortNumber(QString portName)
{
    qDebug() << "Connecting...." << portName;
    emit connStatusChanged("Connecting....");
    openConnection(portName);
}


void PosMsg::sendMessage(int mouseXpos, int wheelPos, int buttonState)
{
    STEERING_CMD command;

    int identifier = 0xffff;
    command.posX = mouseXpos;
    command.wheelPos = wheelPos;
    command.buttonState = buttonState;

    QByteArray msg;
    QDataStream streamOut(&msg, QIODevice::WriteOnly);
    //QDataStream streamIn(&msg, QIODevice::QIODevice::ReadWrite);

    streamOut << identifier;
    streamOut << command.posX;
    streamOut << command.wheelPos;
    streamOut << command.buttonState;

    this->serial.write(msg);
//    int a,b,c;
//    streamIn >> a;
//    streamIn >> b;
//    streamIn >> c;

//    qDebug() << "After Ser: " << a << " " << b << " " << c ;
}


