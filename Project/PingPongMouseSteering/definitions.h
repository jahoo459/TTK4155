#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QList>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QByteArray>

typedef struct{
    QString name;
    QString description;
}COM_PORT_INFO;

typedef struct{
    uint16_t posX;
    uint16_t wheelPos;
    uint16_t buttonState;
}STEERING_CMD;

#endif // DEFINITIONS_H
