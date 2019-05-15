
/*  ************************************************************************  *
 *				   itraynot.h				      *
 *  ************************************************************************  */

/*  This header exists only for a rough definition of the ITrayNotify
    interface, as implemented by EXPLORER in Windows XP and higher, but not
    documented by Microsoft.

    Copyright (C) 2008. Geoff Chappell. All rights reserved.  */

/*  ========================================================================  */

#pragma     once

#include    <windows.h>
#include    <objbase.h>

/*  ************************************************************************  */
/*  Forward references	*/

/*  Follow Microsoft's practices - in the hope of not frustrating anyone who
    uses this header with an SDK in which Microsoft might define
    ITrayNotify.  */

#ifndef __ITrayNotify_FWD_DEFINED__
#define __ITrayNotify_FWD_DEFINED__
typedef interface ITrayNotify ITrayNotify;
#endif

#ifndef __INotificationCB_FWD_DEFINED__
#define __INotificationCB_FWD_DEFINED__
typedef interface INotificationCB INotificationCB;
#endif

/*  ************************************************************************  */

typedef struct tagNOTIFYITEM
{
    PWSTR pszExeName;
    PWSTR pszTip;
    HICON hIcon;
    HWND hWnd;
    DWORD dwPreference;
    UINT uID;
    GUID guidItem;
} NOTIFYITEM;

#ifndef __INotificationCB_INTERFACE_DEFINED__
#define __INotificationCB_INTERFACE_DEFINED__

[
    uuid ("D782CCBA-AFB0-43F1-94DB-FDA3779EACCB")
]
interface INotificationCB : public IUnknown
{
    virtual HRESULT __stdcall Notify (ULONG, NOTIFYITEM *) = 0;
};

#endif	// #ifndef __INotificationCB_INTERFACE_DEFINED__

#ifndef __ITrayNotify_INTERFACE_DEFINED__
#define __ITrayNotify_INTERFACE_DEFINED__

[
    uuid ("FB852B2C-6BAD-4605-9551-F15F87830935")
]
interface ITrayNotify : public IUnknown
{
    virtual HRESULT __stdcall RegisterCallback (INotificationCB *) = 0;

    virtual HRESULT __stdcall SetPreference (NOTIFYITEM const *) = 0;

    virtual HRESULT __stdcall EnableAutoTray (BOOL) = 0;
};

#endif	// #ifndef __ITrayNotify_INTERFACE_DEFINED__

[
    uuid ("25DEAD04-1EAC-4911-9E3A-AD0A4AB560FD")
]
class TrayNotify : public ITrayNotify
{
};

/*  ************************************************************************  */

