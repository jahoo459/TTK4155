#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "posmsg.h"
#include "definitions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updatePortSelection(QList<COM_PORT_INFO> ports);

private slots:
    void on_pushButton_port_select_clicked();

    void on_comboBox_port_select_currentIndexChanged(int index);

    void on_pushButton_fire_clicked();


public slots:
    void updateConnStatus(QString msg);
    void logInfo(QString msg);

private:
    Ui::MainWindow *ui;
    QList<COM_PORT_INFO> com_ports_list;
    uint8_t wheelPosition;
    uint8_t mouseX_position;
    uint8_t buttonState;

signals:
    void comPortSelected(QString portNo);
    void gameStateChanged(int mouseXpos, int wheelPos, int buttonState);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // MAINWINDOW_H
