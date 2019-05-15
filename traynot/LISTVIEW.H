
/*  ************************************************************************  *
 *				   listview.h				      *
 *  ************************************************************************  */

#pragma     once

/*  Standard inclusion	*/

#include    "common.h"

/*  Additional SDK inclusion  */

#include    <commctrl.h>

/*  Extra SDK libraries  */

#pragma     comment (lib, "comctl32.lib")
#pragma     comment (lib, "user32.lib")

/*  Dependence on the main source file to record the HINSTANCE	*/

#include    "globals.h"

/*  ************************************************************************  */

class CListView
{
    HWND m_hWnd;

    protected:

    CListView (VOID)
    {
	m_hWnd = NULL;
    };

    ~CListView (VOID)
    {
	if (m_hWnd != NULL) DestroyWindow (m_hWnd);
    };

    BOOL
    Create (
	DWORD dwExStyle,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	UINT id)
    {
	m_hWnd = CreateWindowEx (
		    dwExStyle,
		    WC_LISTVIEW,
		    L"",
		    dwStyle,
		    x, y, nWidth, nHeight,
		    hWndParent,
		    (HMENU) id,
		    g_hInstance,
		    NULL);

	return m_hWnd != NULL ? TRUE : FALSE;
    };

    public:

    BOOL GetSize (LPRECT prc)
    {
	return :: GetClientRect (m_hWnd, prc);
    };

    BOOL SetSize (LPCRECT prc)
    {
	int x = prc -> left;
	int y = prc -> top;
	int cx = prc -> right - prc -> left;
	int cy = prc -> bottom - prc -> top;
	return :: MoveWindow (m_hWnd, x, y, cx, cy, TRUE);
    };

    DWORD GetWindowExStyle (VOID)
    {
	return :: GetWindowLong (m_hWnd, GWL_EXSTYLE);
    };

    /*	Some messages received by the control's parent must be sent to the
	control.  */

    LRESULT OnSysColorChange (WPARAM wParam, LPARAM lParam)
    {
	return :: SendMessage (m_hWnd, WM_SYSCOLORCHANGE, wParam, lParam);
    };

    LRESULT OnSettingChange (WPARAM wParam, LPARAM lParam)
    {
	return :: SendMessage (m_hWnd, WM_SETTINGCHANGE, wParam, lParam);
    };

    /*	The following functions each do one operation on the List-View
	control without having to know the window handle. Each is just the
	standard macro from COMMCTRL.H but with type checking.

	Add functions as the need for them arises.  */

    BOOL GetItemRect (int i, RECT *prc, int code)
    {
	return ListView_GetItemRect (m_hWnd, i, prc, code);
    };

    int InsertColumn (int iCol, LVCOLUMN * const pcol)
    {
	return ListView_InsertColumn (m_hWnd, iCol, pcol);
    };

    int InsertItem (LVITEM * const pitem)
    {
	return ListView_InsertItem (m_hWnd, pitem);
    };

    BOOL Scroll (int dx, int dy)
    {
	return ListView_Scroll (m_hWnd, dx, dy);
    };

    BOOL SetColumnWidth (int iCol, int cx)
    {
	return ListView_SetColumnWidth (m_hWnd, iCol, cx);
    };

    HIMAGELIST SetImageList (HIMAGELIST himl, int iImageList)
    {
	return ListView_SetImageList (m_hWnd, himl, iImageList);
    };

    BOOL SetItem (LVITEM * const pitem)
    {
	return ListView_SetItem (m_hWnd, pitem);
    };
};

/*  ************************************************************************  */

