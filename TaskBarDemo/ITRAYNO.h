#pragma once

#include <windows.h>
#include <objbase.h>

#ifndef __ITrayNotify_FWD_DEFINED__
#define __ITrayNotify_FWD_DEFINED__

typedef interface ITrayNotify ITrayNotify;

#endif

#ifndef __INotificationCB_FWD_DEFINED__
#define __INotificationCB_FWD_DEFINED__
typedef  interface INotificationCB INotificationCB;
#endif

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

/* interface ITaskbarList */
/* [object][uuid] */

EXTERN_C const IID IID_INotificationCB;

#if defined(__cplusplus) && !defined(CINTERFACE)
MIDL_INTERFACE("D782CCBA-AFB0-43F1-94DB-FDA3779EACCB")

INotificationCB : public IUnknown
{
public:
	virtual ~INotificationCB() = default;
	virtual HRESULT STDMETHODCALLTYPE Notify(ULONG, NOTIFYITEM*) = 0;
};
#endif
#endif  /* __INotificationCB_INTERFACE_DEFINED__ */

#ifndef  __ITrayNotify_INTERFACE_DEFINED__
#define __ITrayNotify_INTERFACE_DEFINED__

EXTERN_C const IID IID_ITrayNotify;

#if defined(__cplusplus) && !defined(CINTERFACE)
MIDL_INTERFACE("FB852B2C-6BAD-4605-9551-F15F87830935")

ITrayNotify:public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE RegisterCallback(INotificationCB*) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetPreference(NOTIFYITEM const*) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnableAutoTray(BOOL) = 0;
};
#endif
#endif  /* __ITrayNotify_INTERFACE_DEFINED__ */

EXTERN_C const CLSID CLSID_TrayNotify;
#if defined(__cplusplus)

class DECLSPEC_UUID("25DEAD04-1EAC-4911-9E3A-AD0A4AB560FD")
TrayNotify : public ITrayNotify
{

};
#endif

class CNotification : public INotificationCB
{
	
public:
	ITrayNotify* m_ITrayNotify;
	CNotification();
	~CNotification();

	HRESULT __stdcall QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;
	HRESULT __stdcall Notify(ULONG, NOTIFYITEM*) override;

};


