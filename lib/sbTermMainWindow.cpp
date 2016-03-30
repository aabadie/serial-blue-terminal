/*
 * Copyright (C) 2016 <Alexandre.Abadie@inria.fr>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3. See the file LICENSE in the top level
 * directory for more details.
 */

#include "sbTermMainWindow.h"

#include <QMessageBox>
#include <QPointer>
#include <QLabel>
#include <QDir>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QToolButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "sbTermConsole.h"
#include "sbTermSettingsController.h"

class sbTermMainWindow::Private
{
public:
    QPointer<QLabel>         status;
    QPointer<QToolButton>    connectButton;
    QPointer<QLineEdit>      sendTextEdit;
    QPointer<QWidget>        commandWidget;
    QPointer<QMenu>          portMenu;
    QPointer<QMenu>          baudrateMenu;
    
    sbTerm::PortSettings     portSettings;
    sbTerm::ConsoleOptions   consoleOptions;
    QSerialPort              serial;
    sbTermConsole            console;
    sbTermSettingsController settingsController;
    
    QTimer watchPortsTimer;
    QStringList availablePorts;
    
    void showStatusMessage(const QString &message);
    void refreshAvailableSerialPorts();
};

void sbTermMainWindow::Private::showStatusMessage(const QString &message)
{
    this->status->setText(message);
}

void sbTermMainWindow::Private::refreshAvailableSerialPorts()
{
    QActionGroup * portActionGroup = new QActionGroup(this->portMenu);
    bool portFound = false;
    QStringList tmpAvailablePorts;
    foreach(const QSerialPortInfo &port, QSerialPortInfo::availablePorts()) {
        if (!this->availablePorts.contains(port.portName())) {
            QAction * portAction = new QAction(port.portName(), this->portMenu);
            portAction->setData(port.portName());
            portAction->setCheckable(true);
            portAction->setActionGroup(portActionGroup);
            this->availablePorts.append(port.portName());
            this->portMenu->addAction(portAction);
        }
        tmpAvailablePorts.append(port.portName());
    }

    foreach(QAction * action, this->portMenu->actions()) {
        if (!tmpAvailablePorts.contains(action->data().toString())) {
            this->availablePorts.removeAll(action->data().toString());
            this->portMenu->removeAction(action);
            continue;
        }

        if (action->data().toString() == this->portSettings.portName) {
            action->trigger();
            portFound = true;
        }
    }

    if (QSerialPortInfo::availablePorts().empty()) {
        this->portMenu->setTitle(tr("No port available"));
    }

    if (!portFound) {
        this->portSettings.portName = "";
    }
}

sbTermMainWindow::sbTermMainWindow(QWidget *parent) :
    QMainWindow(parent),
    d(new sbTermMainWindow::Private)
{
    d->console.setFocusPolicy(Qt::NoFocus);
    
    d->sendTextEdit = new QLineEdit(this);
    d->sendTextEdit->setPlaceholderText(tr("type text to send here"));
    
    QHBoxLayout * commandLayout = new QHBoxLayout;
    commandLayout->addWidget(new QLabel(tr("Input:"), this));
    commandLayout->addWidget(d->sendTextEdit);
    
    d->commandWidget = new QWidget(this);
    d->commandWidget->setLayout(commandLayout);
    d->commandWidget->setEnabled(false);
    
    d->connectButton = new QToolButton(this);
    d->connectButton->setMinimumSize(25, 25);
    d->connectButton->setIcon(QIcon(":images/disconnect.png"));
    d->connectButton->setFocusPolicy(Qt::NoFocus);
    
    QHBoxLayout * bottomLayout = new QHBoxLayout;
    bottomLayout->setMargin(0);
    bottomLayout->setContentsMargins(2, 0, 0, 0);
    bottomLayout->addWidget(d->connectButton);
    bottomLayout->addWidget(d->commandWidget);
    
    QVBoxLayout * layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(&d->console);
    layout->addLayout(bottomLayout);
    
    QWidget * centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    
    this->setCentralWidget(centralWidget);
    
    this->setWindowIcon(QIcon(":/images/terminal.png"));
    this->setWindowTitle(tr("Serial Blue Terminal"));
    this->setMinimumSize(QSize(640, 480));
    
    d->settingsController.readSerialSettings(d->portSettings);
    d->settingsController.readConsoleOptions(d->consoleOptions);
    
    d->console.setBackgroundColor(d->consoleOptions.bgColor);
    d->console.setDateTimeColor(d->consoleOptions.dtColor);
    d->console.setRXColor(d->consoleOptions.rxColor);
    d->console.setTXColor(d->consoleOptions.txColor);
    
    /* Menu bar initialization */
    QMenuBar * menuBar = new QMenuBar;
    d->portMenu = menuBar->addMenu(tr("&Ports"));
    d->baudrateMenu = menuBar->addMenu(tr("&Baudrates"));
    connect(d->portMenu.data(),     &QMenu::triggered,
            this,                   &sbTermMainWindow::onSerialPortSelected);
    connect(d->baudrateMenu.data(), &QMenu::triggered,
            this,                   &sbTermMainWindow::onSerialSpeedSelected);

    QMenu * optionMenu = menuBar->addMenu(tr("&Options"));

    d->refreshAvailableSerialPorts();

    QActionGroup * speedActionGroup = new QActionGroup(this);
    foreach(const QString &speed, QStringList()
            << "1200"
            << "2400"
            << "4800"
            << "9600"
            << "19200"
            << "38400"
            << "57600"
            << "115200") {
        QAction * speedAction = new QAction(speed, d->baudrateMenu);
        speedAction->setData(speed);
        speedAction->setCheckable(true);
        speedAction->setActionGroup(speedActionGroup);
        d->baudrateMenu->addAction(speedAction);
        if (speed == QString::number(d->portSettings.baudRate)) {
            speedAction->setChecked(true);
            this->onSerialSpeedSelected(speedAction);
        }
    }
    
    // Options menu
    QAction* localEchoAction = new QAction(tr("Local echo"), optionMenu);
    localEchoAction->setCheckable(true);
    localEchoAction->setChecked(d->consoleOptions.localEcho);
    optionMenu->addAction(localEchoAction);
    connect(localEchoAction, &QAction::changed,
            this,            &sbTermMainWindow::onOptionLocalEchoChanged);
    QAction* crlfAction      = new QAction(tr("Add CRLF"), optionMenu);
    crlfAction->setCheckable(true);
    crlfAction->setChecked(d->consoleOptions.crlf);
    optionMenu->addAction(crlfAction);
    connect(crlfAction, &QAction::changed,
            this,       &sbTermMainWindow::onOptionCRLFChanged);
    QAction* hexaAction      = new QAction(tr("Hexadecimal"), optionMenu);
    hexaAction->setCheckable(true);
    hexaAction->setChecked(d->consoleOptions.hexa);
    optionMenu->addAction(hexaAction);
    d->console.setDisplayHexadecimal(d->consoleOptions.hexa);
    connect(hexaAction, &QAction::changed,
            this,       &sbTermMainWindow::onOptionDisplayHexaChanged);
    QAction* datetimeAction  = new QAction(tr("Display datetime"), optionMenu);
    datetimeAction->setCheckable(true);
    datetimeAction->setChecked(d->consoleOptions.datetime);
    optionMenu->addAction(datetimeAction);
    d->console.setDatetimeDisplayed(d->consoleOptions.datetime);
    connect(datetimeAction, &QAction::changed,
            this,           &sbTermMainWindow::onOptionDatetimeChanged);
    
    this->setMenuBar(menuBar);
    
    /* Status bar initialization */
    d->connectButton->setEnabled(!d->portSettings.portName.isEmpty());
    d->status = new QLabel(tr("Disconnected"),this);
    
//    this->statusBar()->addWidget(d->connectButton);
    this->statusBar()->addWidget(d->status);
    
    /* Connections */
    connect(d->connectButton,       &QToolButton::clicked,
            this,                   &sbTermMainWindow::onConnectButtonClicked);
    connect(d->sendTextEdit.data(), &QLineEdit::returnPressed,
            this,                   &sbTermMainWindow::onSendTextValidated);
    connect(&d->serial,             &QSerialPort::readyRead,    
            this,                   &sbTermMainWindow::readData);
    connect(&d->console,            &sbTermConsole::dataReady, 
            this,                   &sbTermMainWindow::writeData);
    connect(&d->serial, SIGNAL(error(QSerialPort::SerialPortError)), 
            this,       SLOT(handleError(QSerialPort::SerialPortError)));
    
    d->watchPortsTimer.setInterval(2000);
    connect(&d->watchPortsTimer, &QTimer::timeout,
            this,                &sbTermMainWindow::onPortWatcherTimerTimeout);
    d->watchPortsTimer.start();

    if (d->availablePorts.contains(d->portSettings.portName)) {
        this->openSerialPort();
    }
}

sbTermMainWindow::~sbTermMainWindow()
{
    closeSerialPort();
    delete d;
}

void sbTermMainWindow::onSerialPortSelected(QAction* action)
{
    if (!action) {
        return;
    }

    if (d->portSettings.portName != action->data().toString()) {
        d->portSettings.portName = action->data().toString();
        d->portMenu->setTitle(QString("Port: %1").arg(d->portSettings.portName));
        qDebug() << "Using serial port" << d->portSettings.portName;

        d->connectButton->setEnabled(true);
        if (!d->serial.isOpen()) {
            d->connectButton->setToolTip(tr("Connect to '%1'").arg(d->portSettings.portName));
        }

        d->settingsController.writeSerialSettings(d->portSettings);
    }
}

void sbTermMainWindow::onSerialSpeedSelected(QAction* action)
{
    if (!action) {
        return;
    }
    
    d->portSettings.baudRate = static_cast<QSerialPort::BaudRate>(action->data().toInt());
    d->baudrateMenu->setTitle(QString("Baudrate: %1").arg(action->data().toInt()));
    qDebug() << "Using baudrate" << d->portSettings.baudRate << "bps";
    
    d->settingsController.writeSerialSettings(d->portSettings);
    
    if (!d->portSettings.portName.isEmpty()) {
        d->connectButton->setEnabled(true);
    }
}

void sbTermMainWindow::onOptionLocalEchoChanged()
{
    QAction * action = qobject_cast<QAction*>(this->sender());
    if (!action) {
        return;
    }

    d->consoleOptions.localEcho = action->isChecked();
    d->settingsController.writeConsoleOptions(d->consoleOptions);
}

void sbTermMainWindow::onOptionCRLFChanged()
{
    QAction * action = qobject_cast<QAction*>(this->sender());
    if (!action) {
        return;
    }

    d->consoleOptions.crlf = action->isChecked();
    d->settingsController.writeConsoleOptions(d->consoleOptions);
}

void sbTermMainWindow::onOptionDatetimeChanged()
{
    QAction * action = qobject_cast<QAction*>(this->sender());
    if (!action) {
        return;
    }

    d->consoleOptions.datetime = action->isChecked();
    d->console.setDatetimeDisplayed(d->consoleOptions.datetime);
    d->settingsController.writeConsoleOptions(d->consoleOptions);
}

void sbTermMainWindow::onOptionDisplayHexaChanged()
{
    QAction * action = qobject_cast<QAction*>(this->sender());
    if (!action) {
        return;
    }

    d->consoleOptions.hexa = action->isChecked();
    d->console.setDisplayHexadecimal(d->consoleOptions.hexa);
    d->settingsController.writeConsoleOptions(d->consoleOptions);
}

void sbTermMainWindow::onConnectButtonClicked()
{
    if (d->serial.isOpen()) {
        this->closeSerialPort();
    }
    else {
        this->openSerialPort();
    }
}

void sbTermMainWindow::onSendTextValidated()
{
    this->writeData(d->sendTextEdit->text().toLocal8Bit());
    d->sendTextEdit->clear();
}

void sbTermMainWindow::onPortWatcherTimerTimeout()
{
    d->refreshAvailableSerialPorts();
}

void sbTermMainWindow::openSerialPort()
{   
    if (d->serial.isOpen()) {
        d->serial.close();
    }
    
    qDebug() << "Connecting serial port" << d->portSettings.portName 
             << "at speed" << d->portSettings.baudRate << "bps.";
    
    d->serial.setPortName(d->portSettings.portName);
    d->serial.setBaudRate(d->portSettings.baudRate);
    d->serial.setDataBits(d->portSettings.dataBits);
    d->serial.setParity(d->portSettings.parity);
    d->serial.setStopBits(d->portSettings.stopBits);
    d->serial.setFlowControl(d->portSettings.flowControl);
    if (d->serial.open(QIODevice::ReadWrite)) {
        d->commandWidget->setEnabled(true);
        d->showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                .arg(d->portSettings.portName)
                                .arg(d->portSettings.baudRate)
                                .arg(d->portSettings.dataBits)
                                .arg(d->portSettings.parity)
                                .arg(d->portSettings.stopBits)
                                .arg(d->portSettings.flowControl));
        d->connectButton->setIcon(QIcon(":images/connect.png"));
        d->connectButton->setToolTip(tr("Disconnect from '%1'").arg(d->portSettings.portName));
        qDebug() << "Success: Now connected to serial port" << d->portSettings.portName;
    }
    else {
        qCritical() << d->serial.errorString();
        d->commandWidget->setEnabled(false);
        d->showStatusMessage(QString(tr("Error: ") + d->serial.errorString()));
        d->connectButton->setIcon(QIcon(":images/disconnect.png"));
    }
}

void sbTermMainWindow::closeSerialPort()
{
    if (d->serial.isOpen()) {
        d->serial.close();
    }
    d->connectButton->setIcon(QIcon(":images/disconnect.png"));
    d->commandWidget->setEnabled(false);
    d->showStatusMessage(tr("Disconnected"));
    d->connectButton->setToolTip(tr("Connect to '%1'").arg(d->portSettings.portName));
    
    qDebug() << "Connection to serial port" << d->portSettings.portName << "closed";
}

void sbTermMainWindow::writeData(const QByteArray &data)
{   
    QString toSend(data);
    if (d->consoleOptions.crlf) {
        toSend += "\r\n";
    }
    
    d->serial.write(toSend.toLocal8Bit());
    
    if (d->consoleOptions.localEcho) {
        d->console.putData(toSend.toLocal8Bit(), sbTermConsole::TX);
    }
}

void sbTermMainWindow::readData()
{
    if (d->serial.canReadLine()) {
        const QByteArray data = d->serial.readLine();
        d->console.putData(data, sbTermConsole::RX);
    }
}

void sbTermMainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qCritical() << tr("Critical Error:") << d->serial.errorString();
        d->connectButton->setIcon(QIcon(":images/disconnect.png"));
        d->commandWidget->setEnabled(false);
        d->showStatusMessage(tr("Disconnected"));
    }
}
