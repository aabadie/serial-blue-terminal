/*
 * Copyright (C) 2016 <Alexandre.Abadie@inria.fr>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3. See the file LICENSE in the top level
 * directory for more details.
 */

#include <QApplication>

#include "sbTermMainWindow.h"

#define NO_COLOR       "\x1b[0m"
#define DEBUG_COLOR    "\033[36m"
#define WARNING_COLOR  "\033[33m"
#define CRITICAL_COLOR "\033[31;1m"
#define MESSAGE_COLOR  "\033[0m"

int main(int argc, char *argv[])
{
    const QString messagePattern = QString("%1[%{time yyyy-MM-dd h:mm:ss.zzz}] "
                                           "%{if-debug}%2DEB %{endif}%{if-warning}%3WAR %{endif}%{if-critical}%4ERROR%{endif} %1"
                                           "%2 %{file}:%{line} %5 %{message} %1")
            .arg(NO_COLOR)
            .arg(DEBUG_COLOR)
            .arg(WARNING_COLOR)
            .arg(CRITICAL_COLOR)
            .arg(MESSAGE_COLOR);
    
    qSetMessagePattern(messagePattern);
    
    QCoreApplication::setOrganizationName("SerialBlueTerminamSoft");
    QCoreApplication::setOrganizationDomain("inria.fr");
    QCoreApplication::setApplicationName("SerialBlueTerminal");
    
    QApplication application(argc, argv);
    sbTermMainWindow mw;
    mw.show();
    
    return application.exec(); 
}
