
/*  ************************************************************************  *
 *				  puterror.cpp				      *
 *  ************************************************************************  */

/*  Standard inclusion	*/

#include    "common.h"

/*  Extra C Run-Time inclusion	*/

#include    <stdio.h>

/*  Extra SDK library  */

#pragma     comment (lib, "user32.lib")

/*  Dependencies on other source files	*/

#include    "globals.h"

/*  Correlation with what other source files expect of this one  */

#include    "puterror.h"

/*  ************************************************************************  */

VOID PutMessageBox (HWND hWnd, UINT Icon, PCWSTR Format, va_list Args)
{
    WCHAR buf [0x0100];
    int cch = vswprintf_s (buf, RTL_NUMBER_OF (buf), Format, Args);
    if (cch ++ > 0) {
	PWCHAR text = buf;
	if (cch >= RTL_NUMBER_OF (buf)) {
	    PWCHAR p = new WCHAR [cch];
	    if (p != NULL) {
		vswprintf_s (p, cch, Format, Args);
		text = p;
	    }
	}
	MessageBox (hWnd, text, g_szCaption, MB_OK | Icon);
	if (text != buf) delete text;
    }
}

VOID PutError (HWND hWnd, PCWSTR Format, ...)
{
    va_list args;
    va_start (args, Format);
    PutMessageBox (hWnd, MB_ICONERROR, Format, args);
    va_end (args);
}

VOID PutWarning (HWND hWnd, PCWSTR Format, ...)
{
    va_list args;
    va_start (args, Format);
    PutMessageBox (hWnd, MB_ICONWARNING, Format, args);
    va_end (args);
}

VOID PutInformation (HWND hWnd, PCWSTR Format, ...)
{
    va_list args;
    va_start (args, Format);
    PutMessageBox (hWnd, MB_ICONINFORMATION, Format, args);
    va_end (args);
}

VOID PutMemoryError (HWND hWnd)
{
    PutError (hWnd, L"Insufficient memory");
}

/*  ************************************************************************  */

