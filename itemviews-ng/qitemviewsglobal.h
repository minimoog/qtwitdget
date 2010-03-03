#ifndef QITEMVIEWSNGGLOBAL_H
#define QITEMVIEWSNGGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(Q_OS_WIN)
#   if defined(QT_BUILD_ITEMVIEWS_LIB)
#       ifndef Q_ITEMVIEWSNG_EXPORT
#           define Q_ITEMVIEWSNG_EXPORT
#       endif
#   else
#       ifndef Q_ITEMVIEWSNG_EXPORT
#           define Q_ITEMVIEWSNG_EXPORT
#       endif
#   endif
#else
#       ifndef Q_ITEMVIEWSNG_EXPORT
#           define Q_ITEMVIEWSNG_EXPORT
#		endif
#endif

#endif //QITEMVIEWSNGGLOBAL_H

