#ifndef SBTERMPORTSETTINGS_H
#define SBTERMPORTSETTINGS_H

#include <QtSerialPort>

class QString;

namespace sbTerm
{
    struct PortSettings {
        QString portName;
        QSerialPort::BaudRate    baudRate;
        QSerialPort::Parity      parity;
        QSerialPort::DataBits    dataBits;
        QSerialPort::StopBits    stopBits;
        QSerialPort::FlowControl flowControl;
        bool localEcho;
        bool crlf;
        bool hexa;
        bool datetime;
    };
}

#endif // SBTERMPORTSETTINGS_H
