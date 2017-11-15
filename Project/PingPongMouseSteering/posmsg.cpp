#include "posmsg.h"

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

    uint8_t identifier = 0xff;
    command.posX = mouseXpos;
    command.wheelPos = wheelPos;
    command.buttonState = buttonState;

    QByteArray msg;
    QDataStream streamOut(&msg, QIODevice::WriteOnly);
    //QDataStream streamIn(&msg, QIODevice::QIODevice::ReadWrite);

    streamOut << identifier;
    msg.clear();
    this->serial.write(msg);
    while(receiveData() == 0);

    streamOut << command.posX;
    msg.clear();
    this->serial.write(msg);
    while(receiveData() == 0);

    streamOut << command.wheelPos;
    msg.clear();
    this->serial.write(msg);
    while(receiveData() == 0);

    streamOut << command.buttonState;
    msg.clear();
    this->serial.write(msg);
    while(receiveData() == 0);


//    if(sizeof (msg) == 4)
//    {
//        this->serial.write(msg);
//        //log(msg);
//        cout << (uint8_t)msg.at(0) << " " << (uint8_t)msg.at(1) << " " << (uint8_t)msg.at(2) << " " << (uint8_t)msg.at(3) << endl;
//    }
}


