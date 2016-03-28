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

namespace {
    const int MAX_BUFFER_SIZE = 1024;
    const char * rxColor = "green";
    const char * txColor = "red";
}

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
    p.setColor(QPalette::Base, Qt::black);
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
        prefix += QString("<span style='color:%1'>[ %2 ] </span> ")
                .arg("white")
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    }

    QString color;
    QString senseStr;
    switch(sense) {
    case RX:
        senseStr = "RX";
        color = QString(rxColor);
        break;
    case TX:
        senseStr = "TX";
        color = QString(txColor);
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

    QString htmlDisplay = QString("%1 <span style=""color:%3;font-weight:bold"">%2 # </span><span style=""color:%3>%4</span>")
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
