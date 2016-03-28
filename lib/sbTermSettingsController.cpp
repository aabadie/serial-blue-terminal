#include "sbTermSettingsController.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

#include "sbTermPortSettings.h"

class sbTermSettingsController::Private
{
public:
    QSettings settings;
    
    Private() : settings(QDir::homePath() + "/.serial-blue-terminal", QSettings::IniFormat) {}
};

sbTermSettingsController::sbTermSettingsController() 
    : d(new sbTermSettingsController::Private)
{
    if (!QFile(d->settings.fileName()).exists()) {
        this->initialize();
    }
}

sbTermSettingsController::~sbTermSettingsController()
{
    delete d;
}

void sbTermSettingsController::initialize() const
{
    sbTerm::PortSettings toInit;
    qDebug() << "Initializing default settings in file" << d->settings.fileName();
    toInit.portName    = "";
    toInit.baudRate    = static_cast<QSerialPort::BaudRate>(9600);
    toInit.dataBits    = static_cast<QSerialPort::DataBits>(8);
    toInit.parity      = static_cast<QSerialPort::Parity>(0);
    toInit.stopBits    = static_cast<QSerialPort::StopBits>(1);
    toInit.flowControl = static_cast<QSerialPort::FlowControl>(0);
    toInit.localEcho   = true;
    toInit.crlf        = true;
    toInit.hexa        = false;
    toInit.datetime    = true;
    
    this->write(toInit);
}

void sbTermSettingsController::read(sbTerm::PortSettings &toRead) const
{
    qDebug() << "Read settings from file" << d->settings.fileName();
    toRead.portName    = d->settings.value("default/portName", "").toString();
    toRead.baudRate    = static_cast<QSerialPort::BaudRate>(d->settings.value("default/baudRate", 9600).toInt());
    toRead.dataBits    = static_cast<QSerialPort::DataBits>(d->settings.value("default/dataBits", 8).toInt());
    toRead.parity      = static_cast<QSerialPort::Parity>(d->settings.value("default/parity", 0).toInt());
    toRead.stopBits    = static_cast<QSerialPort::StopBits>(d->settings.value("default/stopBits", 1).toInt());
    toRead.flowControl = static_cast<QSerialPort::FlowControl>(d->settings.value("default/flowControl", 0).toInt());
    toRead.localEcho   = d->settings.value("default/localEcho", true).toBool();
    toRead.crlf        = d->settings.value("default/crlf", true).toBool();
    toRead.hexa        = d->settings.value("default/hexa", false).toBool();
    toRead.datetime    = d->settings.value("default/datetime", true).toBool();
}

void sbTermSettingsController::write(const sbTerm::PortSettings &toWrite) const
{
    d->settings.beginGroup("default");
    d->settings.setValue("portName",    toWrite.portName);
    d->settings.setValue("baudRate",    toWrite.baudRate);
    d->settings.setValue("dataBits",    toWrite.dataBits);
    d->settings.setValue("parity",      toWrite.parity);
    d->settings.setValue("stopBits",    toWrite.stopBits);
    d->settings.setValue("flowControl", toWrite.flowControl );
    d->settings.setValue("localEcho",   toWrite.localEcho);
    d->settings.setValue("crlf",        toWrite.crlf);
    d->settings.setValue("hexa",        toWrite.hexa);
    d->settings.setValue("datetime",    toWrite.datetime);
    d->settings.endGroup();
}
