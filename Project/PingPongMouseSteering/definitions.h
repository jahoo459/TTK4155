#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QList>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QByteArray>
#include <iostream>

using namespace std;

typedef struct{
    QString name;
    QString description;
}COM_PORT_INFO;

typedef struct{
    uint8_t posX;
    uint8_t wheelPos;
    uint8_t buttonState;
}STEERING_CMD;

#endif // DEFINITIONS_H
