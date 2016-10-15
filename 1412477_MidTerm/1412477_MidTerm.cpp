// 1412477_MidTerm.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1412477_MidTerm.h"
#include "ItemModel.h"
#include <windowsx.h>
#include <vector>
#include <fstream>
#include <locale>
#include <codecvt>
#include <string>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")


#define MAX_LOADSTRING 100
#define FILE_PATH		L"sample.txt"
#define NUMBER_OF_TYPE	6
#define CHART_HEIGHT	30
#define CHART_WIDTH		600
#define COLOR_A			RGB(72,133,237)
#define COLOR_B			RGB(219,50,54)
#define COLOR_C			RGB(156,39,176)
#define COLOR_D			RGB(244,194,13)
#define COLOR_E			RGB(60,186,84)
#define COLOR_F			RGB(141,110,99)
#define DEFAULT_COLOR	RGB(255,255,255)

#define VK_CHAR_L		0x4C
#define VK_CHAR_N		0x4E

// Global Variables:
HINSTANCE hInst;								// current instance
HWND g_hWnd;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
WCHAR types[NUMBER_OF_TYPE][25] =
{
	L"Ăn uống", L"Di chuyển", L"Nhà cửa",
	L"Xe cộ", L"Nhu yếu phẩm", L"Dịch vụ",
};
COLORREF color[7] = { COLOR_A, COLOR_B, COLOR_C, COLOR_D, COLOR_E, COLOR_F, DEFAULT_COLOR };
int currX[7];
int currY;
long long* typeMoney;
int currTypeIndex = 6;

int defaultWidth = 700, defaultHeight = 550;
int defaultX, defaultY;
int g_ItemCount = 0;
long long g_totalMoney = 0;
bool isAdding = false;
std::vector<CItemModel*> listItem;
int currSelectedIndex = 0;

HWND hComboBox, hMoneyInput, hDescriptionInput, hTotalMoney;
HWND hAddButton, hClose;
HWND g_hListview;
HWND  hPercentA, hPercentB, hPercentC, hPercentD, hPercentE, hPercentF;
HWND hStaticType, hWelcome;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Clear(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//Create listView
HWND createListView(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle);
bool insertItemToList(HWND m_hListview, int mItemCount); //Create new item from input data, and insert to listview and list item.
void writeListItemToFile(std::wstring path); //File I/O
void loadListItemFromFile(std::wstring path); //File I/O
void loadAllItemToListview(HWND m_hListview); //Load all item in List Item to listview
void drawStatistics(HDC hdc); //Calculate and draw the CHART
void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color); //Draw line
void fillRectangle(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color); //Fill rectangle with color
void setWindowText(HWND hWnd, long long value, std::wstring textBefore, std::wstring textAfter); //Set text of a window
int getTypeIndex(CItemModel* item); //Compare item type and get the index in list Type
void enableInputWindow(); //Enable input controls
void disableInputWindow(); //Disable input controls
int detectCoord(int x, int y); //Detect coordination and specified the cursor is hover the charts or not
void showTypeLabel(int index, HDC hdc); //Show type
void garbageCollector(); //GC
void addItemProc(HWND m_hListview); //Adding item procedure
void handleKeyDown(HWND hWnd, int type); //Handle keydown event out-inside listView

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY1412477_MIDTERM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412477_MIDTERM));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1412477_MIDTERM));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW +1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY1412477_MIDTERM);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, CW_USEDEFAULT, defaultWidth, defaultHeight, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc = NULL;

	switch (message)
	{
	case WM_CREATE:
	{
					  //Init extended common controls
					  INITCOMMONCONTROLSEX icc;
					  icc.dwSize = sizeof(icc);
					  icc.dwICC = ICC_WIN95_CLASSES;
					  InitCommonControlsEx(&icc);

					  //Get main window DC
					  hdc = GetDC(hWnd);
					  g_hWnd = hWnd;

					  //Default anchor
					  defaultX = 20;
					  defaultY = 20;
					 
					  //Control initialize
					  HFONT hFont;
					//First groupBox
					  hFont = CreateFont(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
					  hClose = CreateWindowEx(0, L"BUTTON", L"X", WS_CHILD | BS_PUSHBUTTON, defaultX + 145, defaultY + 7, 15, 15, hWnd, (HMENU)IDC_BUTTON_CLOSE, hInst, NULL);
					  SendMessage(hClose, WM_SETFONT, WPARAM(hFont), TRUE);
					  
					  hAddButton = CreateWindowEx(0, L"BUTTON", L"THÊM", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, defaultX + 20, defaultY + 250, 120, 32, hWnd, (HMENU)IDC_BUTTON_ADD, hInst, NULL);
					  SendMessage(hAddButton, WM_SETFONT, WPARAM(hFont), TRUE);

					  hFont = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
					  hComboBox = CreateWindowEx(0, WC_COMBOBOX, TEXT(""),
						  CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
						  defaultX + 20, defaultY + 30, 120, 100, hWnd, NULL, hInst,
						  NULL);
					  SendMessage(hComboBox, WM_SETFONT, WPARAM(hFont), TRUE);

					  hMoneyInput = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, defaultX + 20, defaultY + 60, 120, 20, hWnd, NULL, hInst, NULL);
					  SendMessage(hMoneyInput, WM_SETFONT, WPARAM(hFont), TRUE);
					  SendMessage(hMoneyInput, EM_SETCUEBANNER, TRUE, (LPARAM)L"Nhập số tiền...");

					  hDescriptionInput = CreateWindowEx(0, L"EDIT", L"Ăn sáng...", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, defaultX + 20, defaultY + 90, 120, 150, hWnd, NULL, hInst, NULL);
					  SendMessage(hDescriptionInput, WM_SETFONT, WPARAM(hFont), TRUE);
					  //SendMessage(hDescriptionInput, EM_SETCUEBANNER, TRUE, (LPARAM)L"TEST");

					  HWND hGroupboxA = CreateWindowEx(0, L"BUTTON", L"Thông tin", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, defaultX, defaultY, 160, 300, hWnd, (HMENU)IDC_GROUPBOX, hInst, NULL);
					  SendMessage(hGroupboxA, WM_SETFONT, WPARAM(hFont), TRUE);



						//Second groupbox
					  HWND hGroupboxB = CreateWindowEx(0, L"BUTTON", L"Danh sách", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, defaultX + 170, defaultY, 470, 300, hWnd, (HMENU)IDC_GROUPBOX, hInst, NULL);
					  SendMessage(hGroupboxB, WM_SETFONT, WPARAM(hFont), TRUE);

					  //Init listview properties
					  long extStyle = WS_EX_CLIENTEDGE;
					  long style = LVS_REPORT | LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS;

					  g_hListview = createListView(extStyle, hWnd, IDL_LISTVIEW, hInst, defaultX + 190, defaultY + 30, 430, 250, style);



					  //Third groupbox
					  HWND hGroupboxC = CreateWindowEx(0, L"BUTTON", L"Thống kê", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, defaultX, defaultY + 310, 640, 140, hWnd, (HMENU)IDC_GROUPBOX, hInst, NULL);
					  SendMessage(hGroupboxC, WM_SETFONT, WPARAM(hFont), TRUE);
					  
					  hTotalMoney = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, defaultX + 295, defaultY + 340, 120, 20, hWnd, NULL, hInst, NULL);
					  SendMessage(hTotalMoney, WM_SETFONT, WPARAM(hFont), TRUE);

					  hFont = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
					  HWND hTemp = CreateWindowEx(0, L"STATIC", L"Tổng tiền", WS_CHILD | WS_VISIBLE, defaultX + 225, defaultY + 342, 60, 15, hWnd, NULL, hInst, NULL);
					  SendMessage(hTemp, WM_SETFONT, WPARAM(hFont), TRUE);
					 
					  hFont = CreateFont(14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
					  hStaticType = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, defaultX + 390, defaultY + 417, 230, 15, hWnd, (HMENU)NULL, hInst, NULL);
					  SendMessage(hStaticType, WM_SETFONT, WPARAM(hFont), TRUE);


					  //How???
					  hFont = CreateFont(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

					  hPercentA = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, defaultX, defaultY + 377, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
					  SendMessage(hPercentA, WM_SETFONT, WPARAM(hFont), TRUE);

					  hPercentB = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, defaultX, defaultY + 377, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
					  SendMessage(hPercentB, WM_SETFONT, WPARAM(hFont), TRUE);

					  hPercentC = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, defaultX, defaultY + 377, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
					  SendMessage(hPercentC, WM_SETFONT, WPARAM(hFont), TRUE);

					  hPercentD = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, defaultX, defaultY + 377, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
					  SendMessage(hPercentD, WM_SETFONT, WPARAM(hFont), TRUE);

					  hPercentE = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, defaultX, defaultY + 377, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
					  SendMessage(hPercentE, WM_SETFONT, WPARAM(hFont), TRUE);

					  hPercentF = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, defaultX, defaultY + 377, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
					  SendMessage(hPercentF, WM_SETFONT, WPARAM(hFont), TRUE);

					  hFont = CreateFont(45, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

					  hWelcome = CreateWindowEx(0, L"STATIC", L"MY MONEY LOVER", WS_CHILD, defaultX + 180, defaultY + 377, 300, 50, hWnd, (HMENU)NULL, hInst, NULL);
					  SendMessage(hWelcome, WM_SETFONT, WPARAM(hFont), TRUE);

					  //Initialize combobox
					  TCHAR A[16];
					  int  k = 0;

					  memset(&A, 0, sizeof(A));

					  for (k = 0; k < NUMBER_OF_TYPE; k += 1)
					  {
						  wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)types[k]);

						  // Add string to combobox.
						  SendMessage(hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
					  }


					  // Send the CB_SETCURSEL message to display an initial item 
					  // in the selection field  
					  SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

					  //File I/O
					  loadListItemFromFile(FILE_PATH);
					  loadAllItemToListview(g_hListview);
					  g_ItemCount = listItem.size();
					  
					  //Disable
					 disableInputWindow();

					  setWindowText(hTotalMoney, g_totalMoney, L"", L"");
	}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		//Take HIWORD of wParam to get ComboBox notification codes
		if (wmEvent == CBN_SELCHANGE)
		{
			//Happen when user select another item in combobox
			WCHAR buffer[25];
			int i = SendMessage(hComboBox, CB_GETCURSEL, NULL, NULL);
			wsprintf(buffer, L"%s...", types[i]);
			SetWindowText(hDescriptionInput, buffer);
		}
		 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_BUTTON_ADD:
			//Add button procedure
			addItemProc(g_hListview);
			break;

		case IDC_BUTTON_CLOSE:
			//Close the add button
			isAdding = false;
			disableInputWindow();
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case ID_FILE_CLEAR:
			//Show Clear dialog
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CLEAR_DIALOG), hWnd, Clear);
			break; 

		case ID_FILE_NEW:
			//New item in menu
			addItemProc(g_hListview);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CTLCOLORSTATIC:
	{
							  //TODO: Draw the static text fore color and back color
							  wmId = LOWORD(wParam);
							  HDC hdcStatic = (HDC)wParam;
							  COLORREF color;

							  //Choose color
							  if ((HWND)lParam == hPercentA) //If desired control
							  {
								  color = COLOR_A;
							  }
							  else if ((HWND)lParam == hPercentB)
							  {
								  color = COLOR_B;
							  }
							  else if ((HWND)lParam == hPercentC)
							  {
								  color = COLOR_C;
							  }
							  else if ((HWND)lParam == hPercentD)
							  {
								  color = COLOR_D;
							  }
							  else if ((HWND)lParam == hPercentE)
							  {
								  color = COLOR_E;
							  }
							  else if ((HWND)lParam == hPercentF)
							  {
								  color = COLOR_F;
							  }
							  else if ((HWND)lParam == hWelcome)
							  {
								  //Handle the Welcome text
								  SetTextColor(hdcStatic, COLOR_B);
								  return (BOOL)GetSysColorBrush(COLOR_WINDOW);
							  }
							  else
							  {
								  //Ignore other static control
								  return (BOOL)GetSysColorBrush(COLOR_WINDOW);
							  }

							  //Set color
							  SetTextColor(hdcStatic, RGB(255, 255, 255));
							  SetBkColor(hdcStatic, color);

							  return (BOOL)GetSysColorBrush(COLOR_WINDOW);
	}

	case WM_NOTIFY:
		//TODO: Listen to ListView notifications
		switch (((NMHDR*)lParam)->code)
		{
		case LVN_KEYDOWN:
			if (LPNMLVKEYDOWN(lParam)->wVKey == VK_DELETE && currSelectedIndex >= 0 && currSelectedIndex < listItem.size() && listItem.size() > 0) {
				//Delete listview item
				ListView_DeleteItem(g_hListview, currSelectedIndex);

				//Minus total money
				g_totalMoney -= listItem[currSelectedIndex]->mMoney;
				g_ItemCount--;
				setWindowText(hTotalMoney, g_totalMoney, L"", L"");

				//Remove item out of List Item
				listItem.erase(listItem.begin() + currSelectedIndex);
				
				//Redraw
				RedrawWindow(g_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ERASENOW | RDW_INVALIDATE);
			}
			else if (LPNMLVKEYDOWN(lParam)->wVKey == VK_CHAR_N)
			{
				//Type == 0 mean Ctrl N
				handleKeyDown(hWnd, 0);
			}
			else if (LPNMLVKEYDOWN(lParam)->wVKey == VK_CHAR_L)
			{
				//Type == 1 mean Ctrl L
				handleKeyDown(hWnd, 1);
			}
			break;

		case NM_CLICK:
		{
						 //Get current selected item index
						 int position = ListView_GetNextItem(g_hListview, -1, LVNI_SELECTED);
						 if (position >= 0 && position < listItem.size())
						 {
							 //Update to input pane
							 currSelectedIndex = position;
							 CItemModel* item = listItem[position];
							 setWindowText(hMoneyInput, item->mMoney, L"", L"");
							 SetWindowText(hDescriptionInput, item->mDescription.c_str());
							 SendMessage(hComboBox, CB_SETCURSEL, WPARAM(getTypeIndex(item)), NULL);
						 }
		}
			break;

		case NM_DBLCLK:
			//Do nothing
			break;
		default:
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		drawStatistics(hdc);
		//Draw type 
		fillRectangle(hdc, defaultX + 380, defaultY + 415, defaultX + 385, defaultY + 410 + (5*CHART_HEIGHT/6), color[currTypeIndex]);
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
	{
						 //Get mouse coord
						  int x = GET_X_LPARAM(lParam);
						  int y = GET_Y_LPARAM(lParam);

						  //Get type index
						  int typeIndex = detectCoord(x, y);

						  if (typeIndex != -1 && typeIndex != currTypeIndex)
						  {
							  //Update
							  currTypeIndex = typeIndex;
							  showTypeLabel(currTypeIndex, hdc);
							  RedrawWindow(g_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ERASENOW | RDW_INVALIDATE);
						  }
						  else
						  {
							  //ShowWindow(hStaticType, SW_HIDE);
						  }
						  
	}
		break;

	case WM_KEYDOWN:
	{
					   //Handle Ctrl+N event to create new item
					   if (wParam == VK_CHAR_N) 
					   {
						   //Type == 0 mean Ctrl N
						   handleKeyDown(hWnd, 0);
					   }
					   else if (wParam == VK_CHAR_L) 
					   {
						   handleKeyDown(hWnd, 1);
					   }
	}
		break;
	case WM_DESTROY:
	{
					   //
					   writeListItemToFile(FILE_PATH);
					   garbageCollector();
					   PostQuitMessage(0);
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Clear(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND hParent;
	HWND hWnd;
	RECT rect;
	switch (message)
	{
	case WM_INITDIALOG:
		//Center the dialog in parent
		if ((hParent = GetParent(hDlg)) == NULL)
		{
			hParent = GetDesktopWindow();
		}

		GetWindowRect(hParent, &rect);

		SetWindowPos(hDlg,
			HWND_TOP,
			rect.left + defaultWidth/2 - 250,
			rect.top + defaultHeight/2 - 100,
			0, 0,
			SWP_NOSIZE);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			//Clear
			g_totalMoney = 0;
			g_ItemCount = 0;
			ListView_DeleteAllItems(g_hListview);
			setWindowText(hTotalMoney, 0, L"", L"");
			listItem.clear();
			RedrawWindow(g_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ERASENOW | RDW_INVALIDATE);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			//Do nothing
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

HWND createListView(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle)
{
	//Create
	HWND m_hListView = CreateWindowEx(lExtStyle, WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | lStyle,
		x, y, nWidth, nHeight, parentWnd, (HMENU)ID, hParentInst, NULL);

	//Init 3 columns
	LVCOLUMN lvCol;

	//Let the LVCOLUMN know that we will set the format, header text and width of it
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;

	//Insert type, money and description column
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 100;
	lvCol.pszText = _T("Loại");
	ListView_InsertColumn(m_hListView, 0, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = _T("Số tiền");
	lvCol.cx = 100;
	ListView_InsertColumn(m_hListView, 1, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 207;
	lvCol.pszText = _T("Mô tả");
	ListView_InsertColumn(m_hListView, 2, &lvCol);

	return m_hListView;
}

bool insertItemToList(HWND m_hListview, int mItemCount)
{
	//Get item model data
	CItemModel* item = new CItemModel();
	WCHAR* buffer;

	//Get description
	int len = GetWindowTextLength(hDescriptionInput);
	if (len > 0)
	{
		buffer = new WCHAR[len + 1];
		GetWindowText(hDescriptionInput, buffer, len + 1);
		item->mDescription = std::wstring(buffer);
	}
	else
	{
		MessageBox(g_hWnd, L"Mô tả không được để trống!", L"Thông báo", MB_ICONWARNING | MB_OK);
		return false;
	}

	//Get money
	len = GetWindowTextLength(hMoneyInput);
	if (len > 0)
	{
		buffer = new WCHAR[len + 1];
		GetWindowText(hMoneyInput, buffer, len + 1);
		item->mMoney = _wtoi64(buffer);
	}
	else
	{
		MessageBox(g_hWnd, L"Số tiền không được để trống!", L"Thông báo", MB_ICONWARNING | MB_OK);
		return false;
	}

	//Get type
	buffer = new WCHAR[20];
	GetWindowText(hComboBox, buffer, 20);
	wcscpy_s(item->mType, buffer);

	//Insert to list Item
	listItem.push_back(item);

	//Add to total money
	g_totalMoney += item->mMoney;

	//Add to ListView
	LV_ITEM lv;

	lv.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;

	//Insert type to first column
	lv.iItem = mItemCount;
	lv.iSubItem = 0;
	lv.pszText = item->mType;
	//lv.iImage
	//lv.lParam
	ListView_InsertItem(m_hListview, &lv);

	//Load (Type, Money, Description)
	lv.mask = LVIF_TEXT;

	//Load amount of money to second column
	lv.iSubItem = 1;
	buffer = new WCHAR[20];
	wsprintf(buffer, L"%I64d", item->mMoney);
	lv.pszText = buffer;
	ListView_SetItem(m_hListview, &lv); //Sets some or all of a list - view item's attributes.

	//Load description to third column
	lv.iSubItem = 2;
	lv.pszText = (WCHAR*)item->mDescription.c_str();
	ListView_SetItem(m_hListview, &lv);

	//SendMessage(g_hWnd, WM_PAINT, NULL, NULL);
	//Set total money
	setWindowText(hTotalMoney, g_totalMoney, L"", L"");

	//Redraw the window to update the charts
	RedrawWindow(g_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ERASENOW | RDW_INVALIDATE);

	return true;
}


void writeListItemToFile(std::wstring path)
{
	//Open file
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	std::wofstream f(path);
	f.imbue(utf8_locale);
	
	//Write total money
	f << g_totalMoney << std::endl;

	//Write every single item to file in 3 lines
	for (int i = 0; i < listItem.size(); i++)
	{
		f << std::wstring(listItem[i]->mType) << std::endl;
		f << listItem[i]->mMoney << std::endl;
		f << std::wstring(listItem[i]->mDescription) << std::endl;
	}

	//Close file
	f.close();
}

void loadListItemFromFile(std::wstring path)
{
	//Open file
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	std::wfstream f;
	f.imbue(utf8_locale);
	f.open(path, std::ios::in);

	std::wstring buffer;
	if (f.is_open())
	{
		//Get total money
		if (getline(f, buffer))
		{
			g_totalMoney = _wtoi64(buffer.c_str());
		}

		//Get items
		while (getline(f, buffer))
		{
			CItemModel* item = new CItemModel();

			wcscpy_s(item->mType, buffer.c_str());
			getline(f, buffer);
			item->mMoney = _wtoi64(buffer.c_str());
			getline(f, buffer);
			item->mDescription = buffer;

			listItem.push_back(item);
		}
	}
	
	//Close file
	f.close();
}

void loadAllItemToListview(HWND m_hListview)
{
	LV_ITEM lv;
	WCHAR* buffer = new WCHAR[20];

	for (int i = 0; i < listItem.size(); i++)
	{
		lv.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;

		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = listItem[i]->mType;
		//lv.iImage
		//lv.lParam
		ListView_InsertItem(m_hListview, &lv);

		//Load (Type, Size, Free Space)
		lv.mask = LVIF_TEXT;

		//Load Drives's Type to second column
		lv.iSubItem = 1;
		buffer = new WCHAR[20];
		wsprintf(buffer, L"%I64d", listItem[i]->mMoney);
		lv.pszText = buffer;
		ListView_SetItem(m_hListview, &lv); //Sets some or all of a list - view item's attributes.

		lv.iSubItem = 2;
		lv.pszText = (WCHAR*)listItem[i]->mDescription.c_str();
		ListView_SetItem(m_hListview, &lv);
	}
}

void drawStatistics(HDC hdc)
{
	//If total amount of money is 0, hide the Charts
	if (g_totalMoney <= 0)
	{
		//Clear the charts
		fillRectangle(hdc, defaultX + 20, defaultY + 375, defaultX + 20 + CHART_WIDTH, defaultY + 375 + CHART_HEIGHT, color[6]);
		ShowWindow(hPercentA, SW_HIDE);
		ShowWindow(hPercentB, SW_HIDE);
		ShowWindow(hPercentC, SW_HIDE);
		ShowWindow(hPercentD, SW_HIDE);
		ShowWindow(hPercentE, SW_HIDE);
		ShowWindow(hPercentF, SW_HIDE);
		ShowWindow(hWelcome, SW_SHOW);

		//Show the welcome text
		currTypeIndex = 6;
		ShowWindow(hStaticType, SW_HIDE);
		return;
	}

	//Hide the welcome text
	ShowWindow(hWelcome, SW_HIDE);

	//Init
	typeMoney = new long long[6];
	for (int i = 0; i < 6; i++)
	{
		typeMoney[i] = 0;
	}
	
	int graphWidth = CHART_WIDTH;
	WCHAR buffer[5];
	currY = defaultY + 375;
	currX[7];
	currX[0] = defaultX + 20;
	float percent;

	//Calculate the total money of each type
	for (int i = 0; i < listItem.size(); i++)
	{
		//Why switch case can be used with string T.T
		typeMoney[getTypeIndex(listItem[i])] += listItem[i]->mMoney;
	}
	

	//Draw the chart
	//A
	percent = (typeMoney[0]*1.0 / g_totalMoney); //The percentage of the first type in total amount of money
	currX[1] = currX[0] + percent*graphWidth; //Calculate the start X and end X of percentA chart
	if (percent >= 0.07)
	{
		ShowWindow(hPercentA, SW_SHOW);
		MoveWindow(hPercentA, currX[0] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		setWindowText(hPercentA, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentA, SW_HIDE);
	}

	//Do the same from B -> F
	//B
	percent = (typeMoney[1]* 1.0 / g_totalMoney);
	currX[2] = currX[1] + percent*graphWidth;
	if (percent >= 0.07)
	{
		MoveWindow(hPercentB, currX[1] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		ShowWindow(hPercentB, SW_SHOW);
		setWindowText(hPercentB, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentB, SW_HIDE);
	}

	

	//C
	percent = (typeMoney[2]* 1.0 / g_totalMoney);
	currX[3] = currX[2] + percent*graphWidth;
	if (percent >= 0.07)
	{
		ShowWindow(hPercentC, SW_SHOW);
		MoveWindow(hPercentC, currX[2] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		setWindowText(hPercentC, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentC, SW_HIDE);
	}
	

	//D
	percent = (typeMoney[3]* 1.0 / g_totalMoney);
	currX[4] = currX[3] + percent*graphWidth;
	if (percent >= 0.07)
	{
		MoveWindow(hPercentD, currX[3] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		ShowWindow(hPercentD, SW_SHOW);
		setWindowText(hPercentD, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentD, SW_HIDE);
	}


	//E
	percent = (typeMoney[4]* 1.0 / g_totalMoney);
	currX[5] = currX[4] + percent*graphWidth;
	if (percent >= 0.07)
	{
		MoveWindow(hPercentE, currX[4] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		ShowWindow(hPercentE, SW_SHOW);
		setWindowText(hPercentE, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentE, SW_HIDE);
	}


	//F
	percent = (typeMoney[5]* 1.0 / g_totalMoney);
	currX[6] = currX[5] + percent*graphWidth;
	if (percent >= 0.07)
	{
		MoveWindow(hPercentF, currX[5] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		ShowWindow(hPercentF, SW_SHOW);
		setWindowText(hPercentF, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentF, SW_HIDE);
	}

	//Fill rectangles of the Percent chart
	for (int i = 0; i < 6; i++)
	{
		fillRectangle(hdc, currX[i], currY, currX[i + 1], currY + CHART_HEIGHT, color[i]);
	}
}


void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
	//Create pen with color provided
	HPEN hpen = CreatePen(PS_SOLID, 10, color);
	SelectObject(hdc, hpen);

	//Drawline
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

void fillRectangle(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
	//Create rectangle
	RECT* rect = new RECT;
	rect->left = x1;
	rect->top = y1;
	rect->right = x2;
	rect->bottom = y2;

	//Create new brush
	HBRUSH hbrush = CreateSolidBrush(color);

	//Fill
	FillRect(hdc, rect, hbrush);
}

void setWindowText(HWND hWnd, long long value, std::wstring textBefore, std::wstring textAfter)
{
	//Create the string
	WCHAR buffer[255];
	if (value < 10)
	{
		wsprintf(buffer, L"0%I64d", value);
	}
	else
	{
		wsprintf(buffer, L"%I64d", value);
	}

	//Set text
	SetWindowText(hWnd, (textBefore + std::wstring(buffer) + textAfter).c_str());
}

int getTypeIndex(CItemModel* item)
{
	if (wcscmp(item->mType, L"Ăn uống") == 0)
	{
		return 0;
	}
	else if (wcscmp(item->mType, L"Di chuyển") == 0)
	{
		return 1;
	}
	else if (wcscmp(item->mType, L"Nhà cửa") == 0)
	{
		return 2;
	}
	else if (wcscmp(item->mType, L"Xe cộ") == 0)
	{
		return 3;
	}
	else if (wcscmp(item->mType, L"Nhu yếu phẩm") == 0)
	{
		return 4;
	}
	else
	{
		return 5;
	}
}

void enableInputWindow()
{
		//Enable input controls
		EnableWindow(hComboBox, true);
		EnableWindow(hMoneyInput, true);
		EnableWindow(hDescriptionInput, true);

		//Change button text to Save
		SetWindowText(hAddButton, L"LƯU");

		//Show the close button
		ShowWindow(hClose, SW_SHOW);

		//isAdding = true;
}

void disableInputWindow()
{
		//Disable input controls
		EnableWindow(hComboBox, false);
		EnableWindow(hMoneyInput, false);
		EnableWindow(hDescriptionInput, false);

		//Change button text to Add
		SetWindowText(hAddButton, L"THÊM");

		//Hide the close button
		ShowWindow(hClose, SW_HIDE);

		//isAdding = false;
}

int detectCoord(int x, int y)
{
	//If total money <= 0, disable detect
	if (g_totalMoney <= 0)
	{
		return -1;
	}

	//Detect coordination
	if (y > currY && y < currY + CHART_HEIGHT)
	{
		if (x > currX[0] && x < currX[1])
		{
			return 0;
		}
		else if (x > currX[1] && x < currX[2])
		{
			return 1;
		}
		else if (x > currX[2] && x < currX[3])
		{
			return 2;
		}
		else if (x > currX[3] && x < currX[4])
		{
			return 3;
		}
		else if (x > currX[4] && x < currX[5])
		{
			return 4;
		}
		else if (x > currX[5] && x < currX[6])
		{
			return 5;
		}
	}

	return -1;
}

void showTypeLabel(int index, HDC hdc)
{
	//Get the total money of this type
	std::wstring ws = std::to_wstring(typeMoney[index]);

	//
	switch (index)
	{
	case 0:
		ws = L"Ăn Uống (Ăn sáng, Uống cà phê,..): " + ws;
		break;
	case 1:
		ws = L"Di Chuyển (Xe bus, Xăng, Taxi,..): " + ws;
		break; 
	case 2:
		ws = L"Nhà Cửa (Thuê nhà, Điện, Nước,..): " + ws;
		break;
	case 3:
		ws = L"Xe Cộ (Thay nhớt, Vá xe, CA phạt,..): " + ws;
		break;
	case 4:
		ws = L"Nhu Yếu Phẩm (Xà bông, Sữa tắm,..): " + ws;
		break;
	case 5:
		ws = L"Dịch Vụ (Internet, Card điện thoại,..): " + ws;
		break;
	default:
		break;
	}
	
	//Show the explain text
	ShowWindow(hStaticType, SW_SHOW);
	SetWindowText(hStaticType, ws.c_str());
}

void garbageCollector()
{
	for (int i = 0; i < listItem.size(); i++)
	{
		delete listItem[i];
	}
}

void addItemProc(HWND m_hListview)
{
	if (isAdding)
	{
		//Finish adding, press to save
		if (insertItemToList(m_hListview, g_ItemCount))
		{
			//Success
			g_ItemCount++;
			isAdding = false;
			disableInputWindow();
		}
	}
	else
	{
		isAdding = true;
		enableInputWindow();
	}
}

void handleKeyDown(HWND hWnd, int type)
{
	//0: Ctrl-N
	//1: Ctrl-L

	if (type == 0)
	{
		int ctrl = GetKeyState(VK_LCONTROL);
		if (ctrl & 0x8000) { // 1: DOWN, 0: UP
			//Ctrl + N
			addItemProc(g_hListview);
		}
	}
	else if (type == 1)
	{
		//Handle Ctrl+L event to clear list
		int ctrl = GetKeyState(VK_LCONTROL);
		if (ctrl & 0x8000) { // 1: DOWN, 0: UP
			//Ctrl + N
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CLEAR_DIALOG), hWnd, Clear);
		}
	}
}