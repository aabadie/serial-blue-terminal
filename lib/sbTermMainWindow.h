/*
 * Copyright (C) 2016 <Alexandre.Abadie@inria.fr>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef SBTERMMAINWINDOW_H
#define SBTERMMAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

class sbTermMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum eDeviceType {
        Serial = 0,
        Bluetooth
    };

    explicit sbTermMainWindow(QWidget *parent = 0);
    ~sbTermMainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);
    void onSerialPortSelected(QAction* action);
    void onSerialSpeedSelected(QAction* action);
    void onOptionLocalEchoChanged();
    void onOptionCRLFChanged();
    void onOptionDatetimeChanged();
    void onOptionDisplayHexaChanged();
    void onConnectButtonClicked();
    void onSendTextValidated();
    void onPortWatcherTimerTimeout();

private:
    class Private;
    Private *d;
};

#endif // SBTERMMAINWINDOW_H
