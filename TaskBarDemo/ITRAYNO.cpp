#include "pch.h"
#include "ITRAYNO.h"

CNotification::CNotification()
	: m_ITrayNotify(NULL)
{
	
}

CNotification::~CNotification()
{
}

HRESULT CNotification::QueryInterface(const IID& riid, void** ppvObject)
{
	if (ppvObject == NULL) return E_POINTER;

	if (riid == __uuidof (INotificationCB)) {
		*ppvObject = (INotificationCB*)this;
	}
	else if (riid == IID_IUnknown) {
		*ppvObject = (IUnknown*)this;
	}
	else {
		return E_NOINTERFACE;
	}

	((IUnknown*)* ppvObject)->AddRef();

	return S_OK;
}

ULONG CNotification::AddRef()
{
	return 1;
}

ULONG CNotification::Release()
{
	return 1;
}

HRESULT CNotification::Notify(ULONG Event, NOTIFYITEM* NotifyItem)
{
	if (StrStrW(NotifyItem->pszExeName, L"agent.exe"))
	{
		if (m_ITrayNotify)
		{
			NotifyItem->dwPreference = 2;
			m_ITrayNotify->SetPreference(NotifyItem);
		}
	}
	return S_OK;
}
