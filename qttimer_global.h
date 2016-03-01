#ifndef QTTIMER_GLOBAL_H
#define QTTIMER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTTIMER_LIBRARY)
#  define QTTIMERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTTIMERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTTIMER_GLOBAL_H
