/*
 * Copyright (C) 2016 <Alexandre.Abadie@inria.fr>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3. See the file LICENSE in the top level
 * directory for more details.
 */

#include "sbTermConsole.h"

#include <QScrollBar>
#include <QDateTime>
#include <QDebug>
#include <QTextCursor>
#include <QKeyEvent>

#include "sbTermSettings.h"

class sbTermConsole::Private
{
public:
    Private() 
        : localEcho(true),
          crlf(true),
          displayHexa(false),
          datetime(true){}
    bool localEcho;
    bool crlf;
    bool datetime;
    bool displayHexa;
    QString rxColor;
    QString txColor;
    QString dtColor;
    
    QString convertToHex(const QString& string) const;
};

QString sbTermConsole::Private::convertToHex(const QString &string) const
{
    QString result;
    for(int i = 0; i < string.size(); ++i) {
        result += QString(string.at(i)).toLatin1().toHex().toUpper() + " ";
    }
    
    return result;
}

sbTermConsole::sbTermConsole(QWidget *parent)
    : QTextEdit(parent)
    , d(new sbTermConsole::Private)
{   
    this->setAcceptRichText(true);
    this->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    this->document()->setMaximumBlockCount(1024);
    
    QPalette p = palette();
    p.setColor(QPalette::Base, "#1C1C1C");
    this->setPalette(p);
}

sbTermConsole::~sbTermConsole()
{
    delete d;
}

void sbTermConsole::putData(const QByteArray &data, sbTermConsole::eInputSense sense)
{   
    QString prefix("");
    if (d->datetime) {
        prefix += QString("<span style='color:%1;font-family:Courier'>[ %2 ] </span> ")
                .arg(d->dtColor)
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    }

    QString color;
    QString senseStr;
    switch(sense) {
    case RX:
        senseStr = "RX";
        color = QString(d->rxColor);
        break;
    case TX:
        senseStr = "TX";
        color = QString(d->txColor);
        break;
    default:
        senseStr = "NO-GO";
        color = "white";
        break;
    }
    QString toDisplay = d->displayHexa ? d->convertToHex(QString(data)) : QString(data);
    // replacing html entities:
    toDisplay.replace("&", "&amp;");
    toDisplay.replace(">", "&gt;");
    toDisplay.replace("<", "&lt;");
    toDisplay.replace(" ", "&nbsp;");

    QString htmlDisplay = QString("%1 <span style='color:%3;font-weight:bold;font-family:Courier'>%2 # </span><span style='color:%3;font-family:Courier'>%4</span>")
            .arg(prefix)
            .arg(senseStr)
            .arg(color)
            .arg(toDisplay);
    this->insertHtml(htmlDisplay);

    QTextCursor cursor = this->textCursor();
    cursor.insertBlock();
    this->setTextCursor(cursor);

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

bool sbTermConsole::datetimeDisplayed() const
{
    return d->datetime;
}

void sbTermConsole::setDatetimeDisplayed(bool set)
{
    d->datetime = set;
}

bool sbTermConsole::displayHexadecimal() const
{
    return d->displayHexa;
}

void sbTermConsole::setDisplayHexadecimal(bool set)
{
    d->displayHexa = set;
}

void sbTermConsole::setBackgroundColor(const QString &color)
{
    QPalette p = palette();
    p.setColor(QPalette::Base, color);
    this->setPalette(p);
}

void sbTermConsole::setDateTimeColor(const QString &color)
{
    d->dtColor = color;
}

void sbTermConsole::setRXColor(const QString &color)
{
    d->rxColor = color;
}

void sbTermConsole::setTXColor(const QString &color)
{
    d->txColor = color;
}
