#ifndef SBTERMEXPORT_H
#define SBTERMEXPORT_H

#include <QtCore/qglobal.h>

#ifdef _WIN32
    #ifdef SERIALBLUETERM_EXPORTS
        #define SERIALBLUETERM_EXPORT Q_DECL_EXPORT
    #else
        #define SERIALBLUETERM_EXPORT Q_DECL_IMPORT
    #endif
#else
    #define SERIALBLUETERM_EXPORT
#endif

#endif // SBTERMEXPORT_H

