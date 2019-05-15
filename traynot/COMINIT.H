
/*  ************************************************************************  *
 *				   cominit.h				      *
 *  ************************************************************************  */

#pragma once

#include    "common.h"

#include    <objbase.h>

#pragma     comment (lib, "ole32.lib")

/*  ************************************************************************  */

/*  This class is perhaps a little unnatural, but having a destructor eases
    cleanup (and even makes it more certain).  */

class CComInitialisation
{
    static BOOL s_Initialised;

    public:

    CComInitialisation (VOID)
    {
	s_Initialised = FALSE;
    };

    BOOL Init (VOID)
    {
	if (NOT s_Initialised) {
	    HRESULT hr = CoInitialize (NULL);
	    if (FAILED (hr)) return FALSE;
	    s_Initialised = TRUE;
	}
	return TRUE;
    };

    ~CComInitialisation (VOID)
    {
	if (s_Initialised) CoUninitialize ();
    };
};

BOOL CComInitialisation :: s_Initialised = FALSE;

/*  ************************************************************************  */

