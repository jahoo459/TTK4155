#include "mainwindow.h"
#include <QApplication>
#include "definitions.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    PosMsg newMsg;
    w.show();

    //SIGNALS & SLOTS

    QObject::connect(&w, &MainWindow::comPortSelected, &newMsg, &PosMsg::updatePortNumber);
    QObject::connect(&newMsg, &PosMsg::connStatusChanged, &w, &MainWindow::updateConnStatus);
    QObject::connect(&w, &MainWindow::gameStateChanged, &newMsg, &PosMsg::sendMessage);

    QList<COM_PORT_INFO> com_port_info_list = newMsg.listSerialPorts();

    //Prepare GUI for Port selection
    w.updatePortSelection(com_port_info_list);

    return a.exec();
}
