#ifndef SBTERMSETTINGS_H
#define SBTERMSETTINGS_H

#include <QtSerialPort>

class QString;

namespace sbTerm
{
    struct ConsoleOptions {
        bool localEcho;
        bool crlf;
        bool hexa;
        bool datetime;
        
        QString rxColor;  // RX text color
        QString txColor;  // TX text color
        QString dtColor;  // Datetime text color
        QString bgColor;  // background color
    };

    struct PortSettings {
        QString portName;
        QSerialPort::BaudRate    baudRate;
        QSerialPort::Parity      parity;
        QSerialPort::DataBits    dataBits;
        QSerialPort::StopBits    stopBits;
        QSerialPort::FlowControl flowControl;
    };
}

#endif // SBTERMSETTINGS_H
