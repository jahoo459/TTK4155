#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QList>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QByteArray>
#include <iostream>
#include <windows.h>

#define MOTOR_ID 0xff
#define SERVO_ID 0xfe
#define SOLENOID_ID 0xfd

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
