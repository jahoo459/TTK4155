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
    int getPosX();
    int getButtonState();
    int getPosWheel();

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
    int posX;
    bool buttonState;
    int posWheel;
    QSerialPort serial;
    void openConnection(QString portName);
    void closeConnection();
    void sendData(QByteArray data);
    QByteArray receiveData();

    typedef struct
    {
        int motorPos;
        int servoPos;
        int solenoidState;
    } S_PosMsg;

public slots:
    void updatePortNumber(QString portName);
    void sendMessage(int mouseXpos, int wheelPos, int buttonSTate);

signals:
    void connStatusChanged(QString msg);
};

#endif // POSMSG_H
