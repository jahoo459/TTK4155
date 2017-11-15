#ifndef POSMSG_H
#define POSMSG_H

#include <QObject>
#include "definitions.h"

class PosMsg : public QObject
{
    Q_OBJECT
public:
    PosMsg();
    ~PosMsg();
    uint8_t getPosX();
    uint8_t getButtonState();
    uint8_t getPosWheel();

    QList<COM_PORT_INFO> listSerialPorts()
    {
        QList<COM_PORT_INFO> tempPorts;
        COM_PORT_INFO currPort;

        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            currPort.name = info.portName();
            currPort.description = info.description();
            qDebug() << "Name  :" << info.portName();
            qDebug() << "Description  :" << info.description();
            qDebug() << "Manufactuer :"  << info.manufacturer();
            tempPorts.append(currPort);
        }

        return tempPorts;
    }

private:
    uint8_t posX = 0;
    uint8_t buttonState = 0;
    uint8_t posWheel = 0;
    QSerialPort serial;
    void openConnection(QString portName);
    void closeConnection();
    void sendData(QByteArray data);
    uint8_t receiveData();

    typedef struct
    {
        uint8_t motorPos;
        uint8_t servoPos;
        uint8_t solenoidState;
    } S_PosMsg;

public slots:
    void updatePortNumber(QString portName);
    void sendMessage(int mouseXpos, int wheelPos, int buttonSTate);

signals:
    void connStatusChanged(QString msg);
    void log(QString msg);
};

#endif // POSMSG_H
