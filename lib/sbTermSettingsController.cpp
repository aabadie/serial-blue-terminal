#include "sbTermSettingsController.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

#include "sbTermSettings.h"

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
    qDebug() << "Initializing default settings in file" << d->settings.fileName();
    sbTerm::PortSettings portInit;
    portInit.portName    = "";
    portInit.baudRate    = static_cast<QSerialPort::BaudRate>(9600);
    portInit.dataBits    = static_cast<QSerialPort::DataBits>(8);
    portInit.parity      = static_cast<QSerialPort::Parity>(0);
    portInit.stopBits    = static_cast<QSerialPort::StopBits>(1);
    portInit.flowControl = static_cast<QSerialPort::FlowControl>(0);
    
    this->writeSerialSettings(portInit);
    
    sbTerm::ConsoleOptions consoleInit;
    consoleInit.localEcho = true;
    consoleInit.crlf      = true;
    consoleInit.hexa      = false;
    consoleInit.datetime  = true;
    consoleInit.rxColor   = "#A9D0F5";
    consoleInit.txColor   = "#F5A9A9";
    consoleInit.dtColor   = "#F2F5A9";
    consoleInit.bgColor   = "#1C1C1C";
    
    this->writeConsoleOptions(consoleInit);
}

void sbTermSettingsController::readSerialSettings(sbTerm::PortSettings &settings) const
{
    qDebug() << "Read serial port settings";
    settings.portName    = d->settings.value("serial/portName", "").toString();
    settings.baudRate    = static_cast<QSerialPort::BaudRate>(d->settings.value("serial/baudRate", 9600).toInt());
    settings.dataBits    = static_cast<QSerialPort::DataBits>(d->settings.value("serial/dataBits", 8).toInt());
    settings.parity      = static_cast<QSerialPort::Parity>(d->settings.value("serial/parity", 0).toInt());
    settings.stopBits    = static_cast<QSerialPort::StopBits>(d->settings.value("serial/stopBits", 1).toInt());
    settings.flowControl = static_cast<QSerialPort::FlowControl>(d->settings.value("serial/flowControl", 0).toInt());
}

void sbTermSettingsController::writeSerialSettings(const sbTerm::PortSettings &settings) const
{
    d->settings.beginGroup("serial");
    d->settings.setValue("portName",    settings.portName);
    d->settings.setValue("baudRate",    settings.baudRate);
    d->settings.setValue("dataBits",    settings.dataBits);
    d->settings.setValue("parity",      settings.parity);
    d->settings.setValue("stopBits",    settings.stopBits);
    d->settings.setValue("flowControl", settings.flowControl );
    d->settings.endGroup();
}

void sbTermSettingsController::readConsoleOptions(sbTerm::ConsoleOptions &options) const
{
    qDebug() << "Read console options";
    options.localEcho = d->settings.value("console/localEcho", true).toBool();
    options.crlf      = d->settings.value("console/crlf", true).toBool();
    options.hexa      = d->settings.value("console/hexa", false).toBool();
    options.datetime  = d->settings.value("console/datetime", true).toBool();
    options.rxColor   = d->settings.value("console/rxColor", "#A9D0F5").toString();
    options.txColor   = d->settings.value("console/txColor", "#F5A9A9").toString();
    options.dtColor   = d->settings.value("console/dtColor", "#F2F5A9").toString();
    options.bgColor   = d->settings.value("console/bgColor", "#1C1C1C").toString();
}

void sbTermSettingsController::writeConsoleOptions(const sbTerm::ConsoleOptions &options) const
{
    d->settings.beginGroup("console");
    d->settings.setValue("localEcho", options.localEcho);
    d->settings.setValue("crlf",      options.crlf);
    d->settings.setValue("hexa",      options.hexa);
    d->settings.setValue("datetime",  options.datetime);
    d->settings.setValue("rxColor",   options.rxColor);
    d->settings.setValue("txColor",   options.txColor);
    d->settings.setValue("dtColor",   options.dtColor);
    d->settings.setValue("bgColor",   options.bgColor);
    d->settings.endGroup();
}
