#ifndef API_H
#define API_H
#include <wx/dlimpexp.h>

#ifdef API_CREATING_DLL
#define API_EXPORT WXEXPORT
#else
#define API_EXPORT WXIMPORT
#endif

#endif // API_H
