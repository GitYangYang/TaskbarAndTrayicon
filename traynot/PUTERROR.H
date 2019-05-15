
/*  ************************************************************************  *
 *				   puterror.h				      *
 *  ************************************************************************  */

#pragma     once

#include    "common.h"

/*  ************************************************************************  */

VOID PutMessageBox (HWND hWnd, UINT Icon, PCWSTR Format, va_list Args);

VOID PutError (HWND hWnd, PCWSTR Format, ...);
VOID PutWarning (HWND hWnd, PCWSTR Format, ...);
VOID PutInformation (HWND hWnd, PCWSTR Format, ...);

VOID PutMemoryError (HWND hWnd);

/*  ************************************************************************  */

