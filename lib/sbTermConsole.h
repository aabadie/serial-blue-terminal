/*
 * Copyright (C) 2016 <Alexandre.Abadie@inria.fr>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef SBTERMCONSOLE_H
#define SBTERMCONSOLE_H

#include <QTextEdit>

class sbTermConsole : public QTextEdit
{
    Q_OBJECT

public:
    enum eInputSense {
        RX = 0,
        TX
    };
    
    explicit sbTermConsole(QWidget *parent = 0);
    virtual ~sbTermConsole();

    void putData(const QByteArray &data, sbTermConsole::eInputSense sense = RX);
    
    bool datetimeDisplayed() const;
    void setDatetimeDisplayed(bool set);
    
    bool displayHexadecimal() const;
    void setDisplayHexadecimal(bool set);

    void setBackgroundColor(const QString &color);
    void setDateTimeColor(const QString &color);
    void setRXColor(const QString &color);
    void setTXColor(const QString &color);
    
signals:
    void dataReady(const QByteArray &data);

private:
    class Private;
    Private *d;

};

#endif // SBTERMCONSOLE_H
