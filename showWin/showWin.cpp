// showWin.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "showWin.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;			// current instance
    TCHAR szTitle[MAX_LOADSTRING];		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

//positions and sizes
int xPos=12, yPos = 48;
int xWidth=460, yHeight=40;
int screenWidth=240, screenHeight=320;
int xProgress=2, yProgress=2;
int xProgressWidth=196, yProgressHeight=12;

DWORD dwTextalign=DT_LEFT;

//progressbar
HWND hProgress=NULL;
BOOL bUseProgress=FALSE;
int  iProgressVal=0;
int  iProgressValNew=-1;

//colors
HBRUSH hBackcolor;
BYTE backcolorRed=255, backcolorGreen=207, backcolorBlue=0;
BYTE fontColorRed=0, fontColorGreen=0, fontColorBlue=0;

//the text properties
TCHAR szMessageText[MAX_PATH]=L"Installing";
long fontHeight=10;
HFONT hFont=NULL;

//text message replacement
TCHAR szMessageTextNew[MAX_PATH]=L"";

//timeout stuff using a background thread
int iTimeOut=0;
HANDLE hStopThread=NULL;
HANDLE hThreadHandle=NULL;
DWORD threadID=0;

//structure to hold data for interprocess communication
typedef struct {
	int		iVal;				// 0=replace message text
	TCHAR	szText[MAX_PATH];
} myMsg;

struct cmdList{
	TCHAR value[MAX_PATH];
	struct cmdList *next;
};

void append(struct cmdList **lst, TCHAR value[MAX_PATH])
{
    struct cmdList *neuesElement;
    struct cmdList *lst_iter = *lst;
    neuesElement = (struct cmdList*) malloc(sizeof(*neuesElement)); // erzeuge ein neues Element
    wcscpy(neuesElement->value, value);
    neuesElement->next = NULL; // Wichtig für das Erkennen des Listenendes
    if ( lst_iter != NULL ) { // sind Elemente vorhanden
        while (lst_iter->next != NULL ) // suche das letzte Element
            lst_iter=lst_iter->next;
        lst_iter->next=neuesElement; // Hänge das Element hinten an
    }
    else // wenn die liste leer ist, bin ich das erste Element
        *lst=neuesElement;
}

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int getString(CmdLineArgs a, int idx, TCHAR* str){
	int iRet=0;
	if(a.size()<idx+1)
		return 0;
	wcscpy(a[idx+1], str);
	return wcslen(str);
}

void getOptions(struct cmdList *l){
	struct cmdList *liste;
	liste=l;
	if(l==NULL)
		return;
	int iVal;
	do{
        DEBUGMSG(1, (L"%s\r\n", liste->value));
		if(wcsicmp(liste->value, L"-t")==0){		// message text
			if(liste->next != NULL){
				liste=liste->next;
				wsprintf(szMessageText, L"%s", liste->value);
			}
		}
		else if(wcsicmp(liste->value, L"-m")==0){		// message text
			if(liste->next != NULL){
				liste=liste->next;
				wsprintf(szMessageTextNew, L"%s", liste->value);
			}
		}
		else if(wcsicmp(liste->value, L"-r")==0){	// rgb r value
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					backcolorRed=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-g")==0){	// rgb g value
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					backcolorGreen=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-b")==0){	// rgb b value
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					backcolorBlue=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-rt")==0){	// text rgb r value
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					fontColorRed=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-gt")==0){	// text rgb g value
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					fontColorGreen=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-bt")==0){	// text rgb b value
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					fontColorBlue=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-w")==0){	// width
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					xWidth=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-h")==0){	// height
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					yHeight=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-x")==0){	// x pos
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					xPos=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-y")==0){	// y pos
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0)
					yPos=iVal;
			}
		}
		else if(wcsicmp(liste->value, L"-s")==0){	// font hight
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0){
					if(iVal>=7 && iVal<=24)
						fontHeight=iVal;
				}
			}
		}
		else if(wcsicmp(liste->value, L"-ti")==0){	// timeout
			if(liste->next != NULL){
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0){
					if(iVal>=0 && iVal<=3600)
						iTimeOut=iVal;
				}
			}
		}
		else if(wcsicmp(liste->value, L"-align")==0){	// text alignment
			if(liste->next != NULL){
				liste=liste->next;
				if(wcsicmp(liste->value,L"center")==0)
					dwTextalign=DT_CENTER;
				else if(wcsicmp(liste->value,L"left")==0)
					dwTextalign=DT_LEFT;
				else if(wcsicmp(liste->value,L"right")==0)
					dwTextalign=DT_RIGHT;
			}
		}
		else if(wcsicmp(liste->value, L"-progr")==0){	// progressbar and value
			if(liste->next != NULL){
				bUseProgress=TRUE;
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0){
					if(iVal>=0 && iVal<=100)
						iProgressVal=iVal;
				}
			}
		}
		else if(wcsicmp(liste->value, L"-prval")==0){	// progressbar and value
			if(liste->next != NULL){
				bUseProgress=TRUE;
				liste=liste->next;
				iVal=_wtoi(liste->value);
				if(iVal!=0){
					if(iVal>=0 && iVal<=100)
						iProgressValNew=iVal;
				}
			}
		}
		liste=liste->next;
	}while(liste != NULL);
}

DWORD myThread(LPVOID lpParam){
	BOOL bExit=FALSE;
	HWND hwndMain=(HWND)lpParam;
	DWORD dwWaitResult=0;
	int iCountSeconds=0;
	DEBUGMSG(1, (L"myThread hWndMain=%i\r\n", hwndMain));
	do{
		dwWaitResult = WaitForSingleObject(hStopThread, 1000);
		switch(dwWaitResult){
			case WAIT_OBJECT_0:
				bExit=TRUE;
				break;
			case WAIT_TIMEOUT:
				iCountSeconds++;
				if(iCountSeconds>=iTimeOut)
				{
					PostMessage(hwndMain, WM_QUIT, 99, iTimeOut);
					bExit=TRUE;
				}
				break;
		}
	}while(!bExit);
	return 0;
}

void startThread(HWND hwndMain){
	DEBUGMSG(1, (L"startThread hWndMain=%i\r\n", hwndMain));
	if(hStopThread==NULL)
		hStopThread = CreateEvent(NULL, FALSE, FALSE, L"stop showWin");
	if(hThreadHandle==NULL)
		hThreadHandle = CreateThread(NULL, 0, myThread, (LPVOID)hwndMain, 0, &threadID);
}

void stopThread(){
	if(hStopThread==NULL)
		SetEvent(hStopThread);
}

RECT shrinkRect(RECT* rect, int iVal){
	rect->top+=iVal;
	rect->left+=iVal;
	rect->bottom-=iVal;
	rect->right-=iVal;
	return *rect;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_SHOWWIN, szWindowClass, MAX_LOADSTRING);

	HWND hwndOld=FindWindow(szWindowClass,NULL);
	// kill request?
	if(hwndOld!=NULL){
		if(wcsicmp(lpCmdLine, L"-kill")==0){
			PostMessage(hwndOld, WM_QUIT, 0, 0);
			return 11;
		}
	}

	DEBUGMSG(1, (L"CmdLine parsing #1: \r\n"));
	//command parsing
	struct cmdList *Liste;
	Liste=NULL;
	CmdLineArgs args;
	for (UINT i = 0; i < args.size(); i++){
		DEBUGMSG(1, (L"%20i: '%s'\r\n", i, args[i]));
		append(&Liste, args[i]);
	}
	getOptions(Liste);
	args.~CmdLineArgs();
	//end of parsing

	// already running?
	if(hwndOld!=NULL){
		//check if new message text?
		if(wcslen(szMessageTextNew) > 0){
			myMsg _mymsg;
			memset(&_mymsg,0,sizeof(myMsg));
			wsprintf( _mymsg.szText, L"%s", szMessageTextNew );
			_mymsg.iVal=0;	//the text message identifier
			//prepare WM_COPYDATA
			COPYDATASTRUCT copyData;
			copyData.dwData=1234;
			copyData.cbData=sizeof(myMsg);
			copyData.lpData=&_mymsg;
			SendMessage(hwndOld, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&copyData);
		}
		if(iProgressValNew!=-1){
			myMsg _mymsg;
			memset(&_mymsg,0,sizeof(myMsg));
			wsprintf( _mymsg.szText, L"%i", iProgressValNew );
			_mymsg.iVal=1;	//the progress message identifier
			//prepare WM_COPYDATA
			COPYDATASTRUCT copyData;
			copyData.dwData=1234;
			copyData.cbData=sizeof(myMsg);
			copyData.lpData=&_mymsg;
			SendMessage(hwndOld, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&copyData);
		}
		ShowWindow(hwndOld, SW_SHOWNORMAL);
		return -1;
	}
	
	//client size
	int maxX = GetSystemMetrics(SM_CXSCREEN);
	int maxY = GetSystemMetrics(SM_CYSCREEN);		//640 ??
	int borderSize = GetSystemMetrics(SM_CXBORDER);
	int minSize = GetSystemMetrics(SM_CYMENU);
	//int yFull = GetSystemMetrics(SM_CYFULLSCREEN);	//554 ??
	RECT rectMax;
	GetWindowRect(GetDesktopWindow(), &rectMax);
	if(xWidth<100 || xWidth>maxX)	// secure width setting
		xWidth=maxX-2*borderSize;
	if(yHeight<minSize)
		yHeight=minSize+2*borderSize;

	if(xPos<borderSize)	//secure x pos
		xPos=borderSize;
	if(yPos<rectMax.top)	//secure y pos
		yPos=rectMax.top;

	//progressBar is attached to bottom of window
	if(bUseProgress){
		//extend window
		xProgressWidth=xWidth;
		yHeight+=yProgressHeight;
		yProgress=yHeight-yProgressHeight;

	}
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHOWWIN));

	// Main message loop:
	MSG msg;
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
//  COMMENTS:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	hBackcolor = CreateSolidBrush(RGB(backcolorRed,backcolorGreen,backcolorBlue));

	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHOWWIN));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) hBackcolor;// GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
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

    g_hInst = hInstance; // Store instance handle in our global variable



    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }

  //  hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
  //      CW_USEDEFAULT,  //x
		//CW_USEDEFAULT,  //y	
		//CW_USEDEFAULT,  //width
		//CW_USEDEFAULT,  //height
		//NULL,			//parent window handle
		//NULL,			//menu handle
		//hInstance, 
		//NULL			//createWindow args
		//);

	hWnd = CreateWindowEx( 
		WS_EX_TOPMOST | WS_EX_ABOVESTARTUP,	//exStyle
		szWindowClass,	//wndClass
		NULL, //L"Installer",	//title
		WS_VISIBLE, // ^ WS_CAPTION,		//dwStyle
        xPos, // CW_USEDEFAULT,  //x
		yPos, //CW_USEDEFAULT,  //y	
		xWidth,// 200,//CW_USEDEFAULT,  //width
		yHeight,// 60,//CW_USEDEFAULT,  //height
		NULL,	//hParent
		NULL,	//hMenu
		hInstance,
		NULL
		);

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
    PAINTSTRUCT ps;
    HDC hdc;
	RECT rect;
	SHORT vkShift=0;
	
	HFONT hfOld;
	long lfHeight;
	int iDevCap;
	LOGFONT logfont = { 0 };
    COLORREF oldTextColor;
	COPYDATASTRUCT* copyData;
	TCHAR* szTemp=new TCHAR(MAX_PATH);

	INITCOMMONCONTROLSEX InitCtrlEx;

	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC  = ICC_PROGRESS_CLASS;
	InitCommonControlsEx(&InitCtrlEx);

	HPEN myPen, oldPen;

	switch (message) 
    {
        case WM_CREATE:
			//do font calculation
			hdc=GetWindowDC(hWnd);
			iDevCap=GetDeviceCaps(hdc, LOGPIXELSY);	//pixels per inch
			lfHeight = -((long)fontHeight * (long)iDevCap) / 72L;
			GetObject (GetStockObject (SYSTEM_FONT), sizeof (LOGFONT), (PTSTR) &logfont) ;
			//	HFONT hf = CreateFontIndirect(&logfont);
			logfont.lfHeight=lfHeight;
			hFont=CreateFontIndirect(&logfont);
			ReleaseDC(NULL,hdc);

			DEBUGMSG(1, (L"Create hWnd=%i\r\n", hWnd));
			if(iTimeOut>0)
				startThread(hWnd);

			if(bUseProgress){
				//progressBar
				hProgress = CreateWindowEx(0, PROGRESS_CLASS, NULL,
								WS_CHILD | WS_VISIBLE,
								xProgress, yProgress, xProgressWidth, yProgressHeight,
								hWnd, NULL, g_hInst, NULL);
				SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
				SendMessage(hProgress, PBM_SETPOS, iProgressVal, 0);
			}
			break;
		case WM_COPYDATA:
			copyData=(COPYDATASTRUCT*)lParam;
			myMsg _mymsg;
			if(copyData->dwData==1234)	//that's right
			{
				memcpy(&_mymsg, copyData->lpData, sizeof(myMsg));
			}
			if(_mymsg.iVal==0){		//text message
				if(wcslen(_mymsg.szText)>0)
					wcscpy(szMessageText, _mymsg.szText);
				GetClientRect(hWnd, &rect);
				InvalidateRect(hWnd, &rect, TRUE);
			}
			else if(_mymsg.iVal==1){		//progress message
				if(wcslen(_mymsg.szText)>0)
					wcscpy(szTemp, _mymsg.szText);
				iProgressVal=_wtoi(szTemp);
				SendMessage(hProgress, PBM_SETPOS, iProgressVal, 0);
			}
			break;			
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            
            // TODO: Add any drawing code here...
			GetClientRect(hWnd, &rect);
			//shrink text area if progressbar is there
			if(bUseProgress && hProgress!=NULL){
				rect.bottom-=yProgressHeight;
			}
			//draw rectangle
			myPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
			oldPen = (HPEN)SelectObject(hdc,myPen);
			SelectObject(hdc, hBackcolor);
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			//shrinkRect(&rect, 1);
			//Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			//a drop shadow
			rect.right-=1;rect.bottom-=1;
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			rect.right-=1;rect.bottom-=1;
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			SelectObject(hdc, oldPen);

			SetBkMode(hdc, TRANSPARENT);
			oldTextColor = SetTextColor(hdc, RGB(fontColorRed, fontColorGreen, fontColorBlue));
			hfOld=(HFONT)SelectObject(hdc, hFont);
            DrawText(hdc, 
				szMessageText,	//text to draw
				-1,				//length of text
				&rect, 
				dwTextalign | DT_END_ELLIPSIS | DT_EXTERNALLEADING | DT_VCENTER // | DT_SINGLELINE		//text formatting
				);

            EndPaint(hWnd, &ps);
			SelectObject(hdc, hfOld);
			SetTextColor(hdc, oldTextColor);

			//if(hProgress!=NULL)

			DeleteObject(hFont);
            break;
		case WM_LBUTTONDBLCLK:
			vkShift=GetKeyState(VK_CAPITAL);
			if( (vkShift & 0x80) == 0x80 || (vkShift & 0x01) == 0x01 ){
				if(MessageBox(hWnd, L"Exit?", L"showWin", MB_OKCANCEL)==IDOK)
					DestroyWindow(hWnd);
			}
			break;
		case WM_QUIT:
			DEBUGMSG(1, (L"WM_QUIT \r\n"));
			break;
        case WM_DESTROY:
			stopThread();
            PostQuitMessage(0);
            break;


        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
	delete szTemp;
    return 0;
}

