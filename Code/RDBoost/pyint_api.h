#include <RDGeneral/export.h>
#ifndef RDKIX_PYINT_API_INCLUDED
#define RDKIX_PYINT_API_INCLUDED

#if PY_MAJOR_VERSION >= 3
#define PyInt_FromLong PyLong_FromLong
#define PyInt_AsLong PyLong_AsLong
#endif

#endif
