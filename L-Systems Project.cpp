// L-Systems Project.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
#include "L-Systems Project.h"
#include <locale>
#include <codecvt>
#include "L-Systems Core.h"
using std::string;
using std::wstring;
typedef std::wstring_convert < std::codecvt_utf8_utf16<wchar_t> > converter;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND presetButton, drawButton;
HWND presetBox, rulesBox, angleBox, iterationsBox, startStringBox;
vector<pair<point, point> > points;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	LoadString(hInstance, IDC_LSYSTEMSPROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LSYSTEMSPROJECT));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LSYSTEMSPROJECT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LSYSTEMSPROJECT);
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_MINIMIZEBOX |WS_SYSMENU,
      CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700, NULL, NULL, hInstance, NULL);

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
	//Setup Stuff
	converter con;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR buttonType[] = _T("BUTTON");
	TCHAR editableBoxType[] = _T("EDIT");

	//Relevant Menu Variables
	float degreeAngle=90.0;
	int iterations=5;
	string description;
	string presetBoxTxt,startStrBox,ruleBox,angleBoxTxt,iterBox;
	wstring pBT,sSB,rB,aBT,iB;

	//Presets Text Box, Description, Button Text Values
	presetBoxTxt = "0";
	pBT = con.from_bytes(presetBoxTxt);
	TCHAR presetButtonText[] = _T("Set Preset");
	TCHAR presetDirTextL1[] = _T("This Box takes an integer bewteen 0 and 10.");
	TCHAR presetDirTextL2[] = _T("This is for preset settings.");	
	//Sets initial values for the items in the listed boxes
	//Commented out because of linker error
	init(0, degreeAngle, ruleBox, startStrBox, iterations, description);
	//Start
	sSB = con.from_bytes(startStrBox);
	TCHAR startBoxText[] = _T("Put the starting Chars for the L-System here");
	//Rules
	rB = con.from_bytes(ruleBox);
	TCHAR ruleTxt1[] = _T("Rule start w/ one char, then ':' then the transform, then ; between rules");
	TCHAR ruleTxt2[] = _T("A–F : Draw forward; G–L : Move forward; M–Z : Used only in rule construction");
	TCHAR ruleTxt3[] = _T("- + : Rotate left / right; [ ] : Push / Pop current position; 0-9 : Color Presets");
	//Angle
	angleBoxTxt = std::to_string(degreeAngle);
	aBT = con.from_bytes(angleBoxTxt);
	TCHAR angleTxt[] = _T("Angle of rotations in degrees");
	//Iterations
	iterBox = std::to_string(iterations);
	iB = con.from_bytes(iterBox);
	TCHAR iterTxt[] = _T("Nummber of rule iterations on the string");
	//Draw
	TCHAR drawButtonText[] = _T("Draw");
	//Used for initializing the draw functions
	pair<point, point> gdiCorners;
	vector<pair<char, string> > ruleList;
	string output;

	switch (message)
	{
	case WM_CREATE:
		//Preset text box and button initialize
		presetBox = CreateWindow(editableBoxType, pBT.c_str() , WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 5, 30, 20, hWnd, NULL, NULL, NULL);
		presetButton = CreateWindow(buttonType,presetButtonText,WS_VISIBLE | WS_CHILD, 5, 75,80,20,hWnd,(HMENU) IDM_PRESETBUT,NULL,NULL);
		//Starting String text box ititialize
		startStringBox = CreateWindow(editableBoxType, sSB.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 105, 100, 20, hWnd, NULL, NULL, NULL);
		//Rule box initialize
		rulesBox = CreateWindow(editableBoxType, rB.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 145, 500, 20, hWnd, NULL, NULL, NULL);
		//Angle Box initialize
		angleBox = CreateWindow(editableBoxType, aBT.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 225, 100, 20, hWnd, NULL, NULL, NULL);
		//Iteration Box initialize
		iterationsBox = CreateWindow(editableBoxType, iB.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 265, 30, 20, hWnd, NULL, NULL, NULL);
		//Draw button ititialize
		drawButton = CreateWindow(buttonType, drawButtonText, WS_VISIBLE | WS_CHILD, 5, 315, 80, 20, hWnd, (HMENU)IDM_DRAWBUT, NULL, NULL);

		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_PRESETBUT:	//Runs the code associated with setting a new preset
			int buttonstat;
			int tmp;
			wchar_t text[4];
			buttonstat = GetWindowText(presetBox, &text[0], 4);
			pBT = text;
			presetBoxTxt = con.to_bytes(pBT);
			tmp = std::stoi(presetBoxTxt);
			init(tmp, degreeAngle, ruleBox, startStrBox, iterations, description);
			//Start
			sSB = con.from_bytes(startStrBox);
			SetWindowText(startStringBox, sSB.c_str());
			//Rules
			rB = con.from_bytes(ruleBox);
			SetWindowText(rulesBox, rB.c_str());
			//Angle
			angleBoxTxt = std::to_string(degreeAngle);
			aBT = con.from_bytes(angleBoxTxt);
			SetWindowText(angleBox, aBT.c_str());
			//Iterations
			iterBox = std::to_string(iterations);
			iB = con.from_bytes(iterBox);
			SetWindowText(iterationsBox, iB.c_str());

			ruleParser(ruleBox, ruleList);
			output = buildString(startStrBox, ruleList, iterations);
			pointSetter(points, output, degreeAngle*degree);
			gdiCorners.first.x = 500;
			gdiCorners.first.y = 700;
			gdiCorners.second.x = 1200;
			gdiCorners.second.y = 0;
			justifyPoints(points, gdiCorners);
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_DRAWBUT:	//Runs the code associated with running a new draw call
			int st,rt,at,it;
			wchar_t starttext[20], ruletext[60], angletext[12], itertext[8];
			//Start Box Pull
			st = GetWindowText(startStringBox, &starttext[0], 20);
			sSB = starttext;
			startStrBox = con.to_bytes(sSB);
			//Rule Box Pull
			rt = GetWindowText(rulesBox, &ruletext[0], 60);
			rB = ruletext;
			ruleBox = con.to_bytes(rB);
			//Angle Box Pull
			at = GetWindowText(angleBox, &angletext[0], 12);
			aBT = angletext;
			angleBoxTxt= con.to_bytes(aBT);
			degreeAngle = std::stof(angleBoxTxt);
			//Iteration Box Pull
			it = GetWindowText(iterationsBox, &itertext[0], 8);
			iB = itertext;
			iterBox = con.to_bytes(iB);
			iterations = std::stoi(iterBox);
			//make block into "run" function?
			ruleList.clear();
			ruleParser(ruleBox, ruleList);
			output = buildString(startStrBox, ruleList, iterations);
			pointSetter(points, output, degreeAngle*degree);
			gdiCorners.first.x = 500;
			gdiCorners.first.y = 700;
			gdiCorners.second.x = 1200;
			gdiCorners.second.y = 0;
			justifyPoints(points, gdiCorners);
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//Preset Text
		TextOut(hdc, 5, 25, presetDirTextL1, _tcslen(presetDirTextL1));
		TextOut(hdc, 5, 45, presetDirTextL2, _tcslen(presetDirTextL2));
		//Start Text
		TextOut(hdc, 5, 125, startBoxText, _tcslen(startBoxText));
		//Rules Text
		TextOut(hdc, 5, 165, ruleTxt1, _tcslen(ruleTxt1));
		TextOut(hdc, 5, 185, ruleTxt2, _tcslen(ruleTxt2));
		TextOut(hdc, 5, 205, ruleTxt3, _tcslen(ruleTxt3));
		//Angle Text
		TextOut(hdc, 5, 245, angleTxt, _tcslen(angleTxt));
		//Iteration Text
		TextOut(hdc, 5, 285, iterTxt, _tcslen(iterTxt));
		//gdiRenderFunc(points,hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
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
