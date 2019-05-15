
/*  ************************************************************************  *
 *				  traynot.cpp				      *
 *  ************************************************************************  */

/*  This is the main source file for a program that demonstrates the
    ITrayNotify interface for information about notification-area icons.

    Compile and link to taste. Do not forget the RC file.

    Run on Windows XP or higher.

    Copyright (C) 2008. Geoff Chappell. All rights reserved.  */

/*  ************************************************************************  */

#include    "common.h"

/*  Additional SDK/CRT headers	*/

#include    <stdio.h>		// for swprintf_s

/*  Things that are relatively general, such that they may find use if
    implemented separately from this source file  */

#include    "cominit.h"
#include    "globals.h"
#include    "itraynot.h"
#include    "listview.h"
#include    "puterror.h"

/*  ************************************************************************  */
/*  Global Data and Forward References	*/

WCHAR g_szCaption [] = L"Tray Notification Reporter";
HINSTANCE g_hInstance = NULL;

/*  ************************************************************************  */

/*  The program has a main window whose whole client area is a List-View
    control. Notifications about notification-area items are shown in the
    list-view.	*/

class CNotificationList :
    public CListView,
    public INotificationCB
{
    enum Column
    {
	Column_Tip,
	Column_Event,
	Column_Preference,
	Column_hWnd,
	Column_uID,
	Column_Guid,
	Column_Exe,
	NumColumns
    };

    HIMAGELIST m_ImageList;
    ITrayNotify *m_ITrayNotify;

    BOOL DefineColumns (VOID);
    BOOL PrepareImageList (VOID);
    BOOL EnableNotifications (BOOL);

    CNotificationList (VOID)
    {
	m_ImageList = NULL;
	m_ITrayNotify = NULL;
    };

    public:

    ~CNotificationList (VOID)
    {
	EnableNotifications (FALSE);
    };

    static CNotificationList *Create (DWORD, DWORD, LPCRECT, HWND, UINT);

    /*	INotificationCB methods  */

    HRESULT __stdcall QueryInterface (REFIID, PVOID *);
    ULONG __stdcall AddRef (VOID);
    ULONG __stdcall Release (VOID);
    HRESULT __stdcall Notify (ULONG, NOTIFYITEM *);
};

CNotificationList *
CNotificationList :: Create (
    DWORD dwExStyle,
    DWORD dwStyle,
    LPCRECT prc,
    HWND hWndParent,
    UINT id)
{
    CNotificationList *list = new CNotificationList;
    if (list != NULL) {

	/*  Create the basic list-view window. We choose to work only in
	    Details view.  */

	dwStyle = (dwStyle & ~LVS_TYPEMASK) | LVS_REPORT;

	if (list -> CListView :: Create (
		dwExStyle,
		dwStyle,
		prc -> left,
		prc -> top,
		prc -> right - prc -> left,
		prc -> bottom - prc -> top,
		hWndParent,
		id)) {

	    /*	Define the columns and prepare an image list so that one
		column can show the notification-area icons.  */

	    if (list -> DefineColumns ()) {
		if (list -> PrepareImageList ()) {

		    /*	Register with EXPLORER for callbacks about
			notification-area items.  */

		    if (list -> EnableNotifications (TRUE)) {
			return list;
		    }
		}
	    }
	}
	delete list;
    }
    return NULL;
}

BOOL CNotificationList :: DefineColumns (VOID)
{
    static PWSTR names [NumColumns] = {NULL};
    if (names [0] == NULL) {
	names [Column_Tip] = L"Icon";
	names [Column_Event] = L"Event";
	names [Column_Preference] = L"Preference";
	names [Column_hWnd] = L"Window Handle";
	names [Column_uID] = L"Identifier";
	names [Column_Guid] = L"GUID";
	names [Column_Exe] = L"Program";
    }

    LVCOLUMN lvc = {0};

    for (int n = 0; n < NumColumns; n ++) {

	lvc.mask = LVCF_TEXT;
	lvc.pszText = names [n];

	if (InsertColumn (n, &lvc) == -1) return FALSE;
    }

    return TRUE;
}

BOOL CNotificationList :: PrepareImageList (VOID)
{
    DWORD ilc = ILC_MASK | ILC_COLOR32;
    if (GetWindowExStyle () & WS_EX_LAYOUTRTL) ilc |= ILC_MIRROR;

    m_ImageList = ImageList_Create (
			GetSystemMetrics (SM_CXSMICON),
			GetSystemMetrics (SM_CYSMICON),
			ilc, 0, 1);

    if (m_ImageList == NULL) return FALSE;

    SetImageList (m_ImageList, LVSIL_SMALL);

    return TRUE;
}

/*  ========================================================================  */
/*  INotificationCB methods  */

HRESULT
__stdcall
CNotificationList :: QueryInterface (
    REFIID riid,
    PVOID *ppv)
{
    if (ppv == NULL) return E_POINTER;

    if (riid == __uuidof (INotificationCB)) {
	*ppv = (INotificationCB *) this;
    }
    else if (riid == IID_IUnknown) {
	*ppv = (IUnknown *) this;
    }
    else {
	return E_NOINTERFACE;
    }

    ((IUnknown *) *ppv) -> AddRef ();

    return S_OK;
}

ULONG __stdcall CNotificationList :: AddRef (VOID)
{
    return 1;
}

ULONG __stdcall CNotificationList :: Release (VOID)
{
    return 1;
}

HRESULT
__stdcall
CNotificationList :: Notify (
    ULONG Event,
    NOTIFYITEM *NotifyItem)
{
    /*	Insert into the List-View one item (with subitems) to report what we
	have just been notified.  */

    LVITEM lvi = {0};

    lvi.mask = LVIF_TEXT;
    lvi.iItem = MAXLONG;
    lvi.iSubItem = Column_Tip;
    lvi.pszText = NotifyItem -> pszTip;

    if (m_ImageList != NULL) {
	lvi.mask |= LVIF_IMAGE;
	lvi.iImage = NotifyItem -> hIcon != NULL
			? ImageList_AddIcon (m_ImageList, NotifyItem -> hIcon)
			: -1;
    }

    int i = InsertItem (&lvi);
    if (i == -1) return E_FAIL;

    lvi.mask = LVIF_TEXT;
    lvi.iItem = i;
    lvi.iSubItem = Column_Event;
    switch (Event) {
	case 0: {
	    lvi.pszText = L"Added";
	    break;
	}
	case 1: {
	    lvi.pszText = L"Modified";
	    break;
	}
	case 2: {
	    lvi.pszText = L"Deleted";
	    break;
	}
	default: {
	    return E_FAIL;
	}
    }

    if (NOT SetItem (&lvi)) return E_FAIL;

    WCHAR buf [0x40];

    lvi.mask = LVIF_TEXT;
    lvi.iItem = i;
    lvi.iSubItem = Column_Preference;
    switch (NotifyItem -> dwPreference) {
	case 0: {
	    lvi.pszText = L"Hide if inactive";
	    break;
	}
	case 1: {
	    lvi.pszText = L"Always show";
	    break;
	}
	case 2: {
	    lvi.pszText = L"Always hide";
	    break;
	}
	default: {
	    swprintf_s (buf, RTL_NUMBER_OF (buf),
		    L"%08X", NotifyItem -> dwPreference);
	    lvi.pszText = buf;
	    break;
	}
    }

    if (NOT SetItem (&lvi)) return E_FAIL;

    swprintf_s (buf, RTL_NUMBER_OF (buf), L"%08X", NotifyItem -> hWnd);

    lvi.mask = LVIF_TEXT;
    lvi.iItem = i;
    lvi.iSubItem = Column_hWnd;
    lvi.pszText = buf;

    if (NOT SetItem (&lvi)) return E_FAIL;

    swprintf_s (buf, RTL_NUMBER_OF (buf), L"%08X", NotifyItem -> uID);

    lvi.mask = LVIF_TEXT;
    lvi.iItem = i;
    lvi.iSubItem = Column_uID;
    lvi.pszText = buf;

    if (NOT SetItem (&lvi)) return E_FAIL;

    if (NotifyItem -> guidItem != GUID_NULL) {

	GUID const *guid = &NotifyItem -> guidItem;

	swprintf_s (buf, RTL_NUMBER_OF (buf),
		L"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		guid -> Data1, guid -> Data2, guid -> Data3,
		guid -> Data4 [0], guid -> Data4 [1],
		guid -> Data4 [2], guid -> Data4 [3],
		guid -> Data4 [4], guid -> Data4 [5],
		guid -> Data4 [6], guid -> Data4 [7]);

	lvi.mask = LVIF_TEXT;
	lvi.iItem = i;
	lvi.iSubItem = Column_Guid;
	lvi.pszText = buf;

	if (NOT SetItem (&lvi)) return E_FAIL;
    }

    lvi.mask = LVIF_TEXT;
    lvi.iItem = i;
    lvi.iSubItem = Column_Exe;
    lvi.pszText = NotifyItem -> pszExeName;

    if (NOT SetItem (&lvi)) return E_FAIL;

    /*	Make the columns wide enough to show the data. We might just use
	LVS_EX_AUTOSIZECOLUMNS if we cared to check at run-time for whether
	we've got COMCTL32 version 6.10.  */

    for (ULONG n = 0; n < NumColumns; n ++) {
	if (NOT SetColumnWidth (n, LVSCW_AUTOSIZE)) return FALSE;
    }

    /*	If the List-View was already scrolled to its end, so that the new
	item either straddles the bottom of the List-View or is immediately
	below it, assume that the user is watching the additions and would
	like the list to keep scrolling.  */

    RECT rclv;
    if (GetSize (&rclv)) {
	RECT rci;
	if (GetItemRect (i, &rci, LVIR_BOUNDS)) {
	    if (rci.top <= rclv.bottom AND rclv.bottom < rci.bottom) {
		Scroll (0, rci.bottom - rci.top);
	    }
	}
    }

    return S_OK;
}

/*  ------------------------------------------------------------------------  */
/*  Enabling notifications  */

BOOL CNotificationList :: EnableNotifications (BOOL bEnable)
{
    HRESULT hr;

    if (bEnable AND m_ITrayNotify == NULL) {

	hr = CoCreateInstance (
		__uuidof (TrayNotify),
		NULL,
		CLSCTX_LOCAL_SERVER,
		__uuidof (ITrayNotify),
		(PVOID *) &m_ITrayNotify);

	if (FAILED (hr)) {
	    PutError (NULL, L"Error 0x%08X obtaining ITrayNotify", hr);
	    return FALSE;
	}
    }

    if (m_ITrayNotify == NULL) return FALSE;

    hr = m_ITrayNotify -> RegisterCallback (bEnable ? this : NULL);
    if (FAILED (hr)) {
	PutError (NULL, L"Error 0x%08X registering callback", hr);
    }

    if (NOT bEnable) {
	m_ITrayNotify -> Release ();
	m_ITrayNotify = NULL;
    }

    return SUCCEEDED (hr);
}

/*  ************************************************************************  */
/*  Main Window  */

class CMainWindow
{
    ATOM m_aWndClass;
    CNotificationList *m_List;

    CMainWindow (VOID);

    public:

    ~CMainWindow (VOID);

    static CMainWindow *Create (int);

    private:

    static LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

    BOOL OnCreate (HWND);
    VOID OnDestroy (VOID);
    VOID OnSize (HWND);
    VOID OnSysColorChange (WPARAM, LPARAM);
    VOID OnSettingChange (WPARAM, LPARAM);
};

CMainWindow :: CMainWindow (VOID)
{
    WNDCLASS wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof (CMainWindow *);
    wc.hInstance = g_hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szCaption;

    m_aWndClass = RegisterClass (&wc);
    m_List = NULL;
}

CMainWindow :: ~CMainWindow (VOID)
{
    if (m_List != NULL) delete m_List;
    if (m_aWndClass != 0) UnregisterClass ((LPCWSTR) m_aWndClass, g_hInstance);
}

CMainWindow *CMainWindow :: Create (int nCmdShow)
{
    CMainWindow *mw = new CMainWindow;
    if (mw == NULL) {
	PutMemoryError (NULL);
    }
    else if (mw -> m_aWndClass == 0) {
	PutError (NULL, L"Error registering window class");
    }
    else {

	HWND hwnd = CreateWindowEx (0, g_szCaption, g_szCaption,
				    WS_OVERLAPPEDWINDOW,
				    CW_USEDEFAULT, CW_USEDEFAULT,
				    CW_USEDEFAULT, CW_USEDEFAULT,
				    NULL, NULL, g_hInstance, mw);
	if (hwnd == NULL) {
	    PutError (NULL, L"Error creating main window");
	}
	else {

	    ShowWindow (hwnd, nCmdShow);
	    UpdateWindow (hwnd);

	    return mw;
	}
	delete mw;
    }
    return NULL;
};

LRESULT
CALLBACK
CMainWindow :: WndProc (
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    CMainWindow *mw;
    if (uMsg == WM_NCCREATE) {
	LPCREATESTRUCT cs = (LPCREATESTRUCT) lParam;
	mw = (CMainWindow *) cs -> lpCreateParams;
	if (mw != NULL) {
	    SetWindowLongPtr (hwnd, 0, (LONG_PTR) mw);
	}
    }
    else {
	mw = (CMainWindow *) GetWindowLongPtr (hwnd, 0);
    }
    if (mw != NULL) {

	switch (uMsg) {
	    case WM_CREATE: {
		return mw -> OnCreate (hwnd) ? 0 : -1;
	    }
	    case WM_DESTROY: {
		mw -> OnDestroy ();
		PostQuitMessage (0);
		return 0;
	    }
	    case WM_SIZE: {
		mw -> OnSize (hwnd);
		break;
	    }
	    case WM_SYSCOLORCHANGE: {
		mw -> OnSysColorChange (wParam, lParam);
		break;
	    }
	    case WM_SETTINGCHANGE: {
		mw -> OnSettingChange (wParam, lParam);
		break;
	    }
	}
    }
    return DefWindowProc (hwnd, uMsg, wParam, lParam);
}

BOOL CMainWindow :: OnCreate (HWND hWnd)
{
    /*	Create a List-View control as the main window's one child, taking
	the whole of the main window's client area.  */

    DWORD wsex = 0;
    DWORD ws = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    RECT rc;
    GetClientRect (hWnd, &rc);

    m_List = CNotificationList :: Create (wsex, ws, &rc, hWnd, 1);
    return m_List != NULL;
}

VOID CMainWindow :: OnDestroy (VOID)
{
    if (m_List != NULL) {
	delete m_List;
	m_List = NULL;
    }
}

/*  If being resized, keep the List-View control as the whole of our client
    area.  */

VOID CMainWindow :: OnSize (HWND hWnd)
{
    CListView *lv = m_List;
    if (lv != NULL) {
	RECT rc;
	GetClientRect (hWnd, &rc);
	lv -> SetSize (&rc);
    }
}

/*  All windows that have common controls must forward WM_SYSCOLORCHANGE to
    each of the controls. Microsoft's documentation even makes a point of
    spelling this out in a few places.	*/

VOID CMainWindow :: OnSysColorChange (WPARAM wParam, LPARAM lParam)
{
    CListView *lv = m_List;
    if (lv != NULL) lv -> OnSysColorChange (wParam, lParam);
}

/*  The same seems just as true of WM_SETTINGCHANGE, no matter that
    Microsoft's documentation doesn't say.  */

VOID CMainWindow :: OnSettingChange (WPARAM wParam, LPARAM lParam)
{
    CListView *lv = m_List;
    if (lv != NULL) lv -> OnSettingChange (wParam, lParam);
}

/*  ************************************************************************  */
/*  Effective Entry Point to Program  */

int
WINAPI
wWinMain (
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR lpCmdLine,
    int nCmdShow)
{
    g_hInstance = hInstance;

    /*	Tell OLE32 that we expect to use a COM interface.  */

    CComInitialisation com;
    if (NOT com.Init ()) {
	PutError (NULL, L"COM initialisation failed");
	return -1;
    }

    /*	Create the main window and start processing window messages.  */

    CMainWindow *mw = CMainWindow :: Create (nCmdShow);
    if (mw == NULL) return -1;

    int exitcode = -1;
    for (;;) {
	MSG msg;
	int result = GetMessage (&msg, NULL, 0, 0);
	if (result == -1) break;
	if (result == 0) {
	    exitcode = msg.wParam;
	    break;
	}
	TranslateMessage (&msg);
	DispatchMessage (&msg);
    }

    delete mw;
    return exitcode;
}

/*  ************************************************************************  */

