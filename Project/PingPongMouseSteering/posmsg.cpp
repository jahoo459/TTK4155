#include "posmsg.h"
#include <windows.h>

PosMsg::PosMsg()
{
    listSerialPorts();
}

PosMsg::~PosMsg()
{
    closeConnection();
}

uint8_t PosMsg::getPosX()
{
    return this->posX;
}

uint8_t PosMsg::getButtonState()
{
    return this->buttonState;
}

uint8_t PosMsg::getPosWheel()
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
    qDebug() << "Sending Data";
    serial.write(data);
}

uint8_t PosMsg::receiveData()
{
    QByteArray data = serial.readAll();
    return data.at(0);
}

void PosMsg::updatePortNumber(QString portName)
{
    qDebug() << "Connecting...." << portName;
    emit connStatusChanged("Connecting....");
    closeConnection();
    openConnection(portName);
}


void PosMsg::sendMessage(int mouseXpos, int wheelPos, int buttonState)
{
    STEERING_CMD command;
    qDebug() << "Sending Message...";

    uint8_t identifier = 0xff;
    command.posX = mouseXpos;
    command.wheelPos = wheelPos;
    command.buttonState = buttonState;

    QByteArray msg;
    QDataStream streamOut(&msg, QIODevice::WriteOnly);

    streamOut << identifier;
    this->serial.write(msg);
    Sleep(1);
    msg.clear();

    streamOut << command.posX;
    this->serial.write(msg);
    Sleep(1);
    msg.clear();

    streamOut << command.wheelPos;
    this->serial.write(msg);
    Sleep(1);
    msg.clear();

    streamOut << command.buttonState;
    this->serial.write(msg);
    Sleep(1);
    msg.clear();


//    if(sizeof (msg) == 4)
//    {
//        this->serial.write(msg);
//        //log(msg);
//        cout << (uint8_t)msg.at(0) << " " << (uint8_t)msg.at(1) << " " << (uint8_t)msg.at(2) << " " << (uint8_t)msg.at(3) << endl;
//    }
}


