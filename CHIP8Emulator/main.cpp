#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <iostream>
#include <commdlg.h>
#include <string>
#include <chrono>
#include "chip8.cpp"

#define ID_OPEN 1

HDC hDC;				/* device context */
HPALETTE hPalette = 0;			/* custom palette (if needed) */

const int updateTime = 2000; //time between each frame (makes for approx 60 fps)
int timeToNextUpdate = 2000; //actual time to next update

auto time2 = std::chrono::system_clock::now();;
auto time1 = time2;
unsigned long deltaTime;

void glInit(){
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, 640, 320, 0);
}

void initEmu(const char* fileName){
	CHIP8::LoadROM(fileName);
    CHIP8::InitDefaultData();
    time2 = std::chrono::system_clock::now();
	time1 = time2;
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count();
}

void CheckKeys(){
	//check for input
	if (GetAsyncKeyState(49) || GetAsyncKeyState(33)){CHIP8::input[0x1] = 1;}
	else{CHIP8::input[0x1] = 0;}
	
	if (GetAsyncKeyState(50) || GetAsyncKeyState(64)){CHIP8::input[0x2] = 1;}
	else{CHIP8::input[0x2] = 0;}
	
	if (GetAsyncKeyState(51) || GetAsyncKeyState(35)){CHIP8::input[0x3] = 1;}
	else{CHIP8::input[0x3] = 0;}
	
	if (GetAsyncKeyState(52) || GetAsyncKeyState(36)){CHIP8::input[0xC] = 1;}
	else{CHIP8::input[0xC] = 0;}
	
	if (GetAsyncKeyState(113) || GetAsyncKeyState(81)){CHIP8::input[0x4] = 1;}
	else{CHIP8::input[0x4] = 0;}
	
	if (GetAsyncKeyState(119) || GetAsyncKeyState(87)){CHIP8::input[0x5] = 1;}
	else{CHIP8::input[0x5] = 0;}
	
	if (GetAsyncKeyState(101) || GetAsyncKeyState(69)){CHIP8::input[0x6] = 1;}
	else{CHIP8::input[0x6] = 0;}
	
	if (GetAsyncKeyState(114) || GetAsyncKeyState(82)){CHIP8::input[0xD] = 1;}
	else{CHIP8::input[0xD] = 0;}
	
	if (GetAsyncKeyState(97) || GetAsyncKeyState(65)){CHIP8::input[0x7] = 0;}
	else{CHIP8::input[0x7] = 0;}
	
	if (GetAsyncKeyState(115) || GetAsyncKeyState(83)){CHIP8::input[0x8] = 0;}
	else{CHIP8::input[0x8] = 0;}
	
	if (GetAsyncKeyState(100) || GetAsyncKeyState(68)){CHIP8::input[0x9] = 0;}
	else{CHIP8::input[0x9] = 0;}
	
	if (GetAsyncKeyState(102) || GetAsyncKeyState(70)){CHIP8::input[0xE] = 0;}
	else{CHIP8::input[0xE] = 0;}
	
	if (GetAsyncKeyState(122) || GetAsyncKeyState(90)){CHIP8::input[0xA] = 0;}
	else{CHIP8::input[0xA] = 0;}
	
	if (GetAsyncKeyState(120) || GetAsyncKeyState(88)){CHIP8::input[0x0] = 0;}
	else{CHIP8::input[0x0] = 0;}
	
	if (GetAsyncKeyState(99) || GetAsyncKeyState(67)){CHIP8::input[0xB] = 0;}
	else{CHIP8::input[0xB] = 0;}
	
	if (GetAsyncKeyState(118) || GetAsyncKeyState(86)){CHIP8::input[0xF] = 0;}
	else{CHIP8::input[0xF] = 0;}
	
}

void drawScreen(){
	glClear(GL_COLOR_BUFFER_BIT);
	for (int xPos = 0; xPos < 64; xPos++){
		for (int yPos = 0; yPos < 32; yPos++){
			if (CHIP8::display[yPos * 64 + xPos]){
				glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glVertex2i(xPos * 10, yPos * 10);
				glVertex2i((xPos + 1) * 10, yPos * 10);
				glVertex2i((xPos + 1) * 10, (yPos + 1) * 10);
				glVertex2i(xPos * 10, (yPos + 1) * 10);
				glEnd();
			}
		}
	}
	glFlush();
    SwapBuffers(hDC);
}

void display(){
	time1 = time2;
    time2 = std::chrono::system_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count();
    timeToNextUpdate -= deltaTime;
    CheckKeys();
    if (timeToNextUpdate <= 0) {
    	timeToNextUpdate += updateTime;
    	CHIP8::update();
    	drawScreen();
    }
}

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		case WM_CREATE:{
			
			HMENU menuBar = CreateMenu();
			HMENU mFile = CreateMenu();
			
			AppendMenu(menuBar, MF_POPUP, (UINT_PTR)mFile, "File");
			AppendMenu(mFile, MF_STRING, (UINT_PTR) ID_OPEN, "Open");
			
			SetMenu(hwnd, menuBar);
			
			break;
		}
		
		case WM_SIZE:{
			glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
			PostMessage(hwnd, WM_PAINT, 0, 0);
			break;
		}
		case WM_COMMAND:{
			
			if (LOWORD(wParam) == ID_OPEN){
				
				char fileName[500];
				OPENFILENAMEA open;
				ZeroMemory(&open, sizeof(open));
				open.lStructSize = sizeof(open);
				open.hwndOwner = NULL; 
				open.lpstrFile = fileName;
				open.lpstrFile[0] = '\0';
				open.nMaxFile = 1000000;
				open.lpstrTitle = "Select File";
				open.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER;
				if (GetOpenFileNameA(&open)) {
					//MessageBox(hwnd, fileName, "Open", MB_ICONINFORMATION);
					initEmu(fileName);
				}
			}
			
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		case WM_CLOSE: {
			PostQuitMessage(0);
			break;
    	}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

HWND
CreateOpenGLWindow(char* title, int x, int y, int width, int height, 
		   BYTE type, DWORD flags)
{
    int         n, pf;
    HWND        hWnd;
    WNDCLASS    wc;
    LOGPALETTE* lpPal;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInstance) {
	hInstance = GetModuleHandle(NULL);
	wc.style         = CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "OpenGL";

	if (!RegisterClass(&wc)) {
	    MessageBox(NULL, "RegisterClass() failed:  "
		       "Cannot register window class.", "Error", MB_OK);
	    return NULL;
	}
    }

    hWnd = CreateWindow("OpenGL", title, WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
	MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
		   "Error", MB_OK);
	return NULL;
    }

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
    pfd.iPixelType   = type;
    pfd.cColorBits   = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
	MessageBox(NULL, "ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", "Error", MB_OK); 
	return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	MessageBox(NULL, "SetPixelFormat() failed:  "
		   "Cannot set format specified.", "Error", MB_OK);
	return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE ||
	pfd.iPixelType == PFD_TYPE_COLORINDEX) {

	n = 1 << pfd.cColorBits;
	if (n > 256) n = 256;

	lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
				    sizeof(PALETTEENTRY) * n);
	memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
	lpPal->palVersion = 0x300;
	lpPal->palNumEntries = n;

	GetSystemPaletteEntries(hDC, 0, n, &lpPal->palPalEntry[0]);
    
	/* if the pixel type is RGBA, then we want to make an RGB ramp,
	   otherwise (color index) set individual colors. */
	if (pfd.iPixelType == PFD_TYPE_RGBA) {
	    int redMask = (1 << pfd.cRedBits) - 1;
	    int greenMask = (1 << pfd.cGreenBits) - 1;
	    int blueMask = (1 << pfd.cBlueBits) - 1;
	    int i;

	    /* fill in the entries with an RGB color ramp. */
	    for (i = 0; i < n; ++i) {
		lpPal->palPalEntry[i].peRed = 
		    (((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
		lpPal->palPalEntry[i].peGreen = 
		    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
		lpPal->palPalEntry[i].peBlue = 
		    (((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;
		lpPal->palPalEntry[i].peFlags = 0;
	    }
	} else {
	    lpPal->palPalEntry[0].peRed = 0;
	    lpPal->palPalEntry[0].peGreen = 0;
	    lpPal->palPalEntry[0].peBlue = 0;
	    lpPal->palPalEntry[0].peFlags = PC_NOCOLLAPSE;
	    lpPal->palPalEntry[1].peRed = 255;
	    lpPal->palPalEntry[1].peGreen = 0;
	    lpPal->palPalEntry[1].peBlue = 0;
	    lpPal->palPalEntry[1].peFlags = PC_NOCOLLAPSE;
	    lpPal->palPalEntry[2].peRed = 0;
	    lpPal->palPalEntry[2].peGreen = 255;
	    lpPal->palPalEntry[2].peBlue = 0;
	    lpPal->palPalEntry[2].peFlags = PC_NOCOLLAPSE;
	    lpPal->palPalEntry[3].peRed = 0;
	    lpPal->palPalEntry[3].peGreen = 0;
	    lpPal->palPalEntry[3].peBlue = 255;
	    lpPal->palPalEntry[3].peFlags = PC_NOCOLLAPSE;
	}

	hPalette = CreatePalette(lpPal);
	if (hPalette) {
	    SelectPalette(hDC, hPalette, FALSE);
	    RealizePalette(hDC);
	}

	free(lpPal);
    }

    ReleaseDC(hWnd, hDC);

    return hWnd;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HGLRC hRC;
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */
	DWORD buffer = PFD_DOUBLEBUFFER;	/* buffering type */
    BYTE  color  = PFD_TYPE_RGBA;	/* color type */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateOpenGLWindow("animate", 0, 0, 640, 320, color, buffer);
	
	/*hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","WindowsApp",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, // x 
		CW_USEDEFAULT, // y 
		640, // width
		480, // height 
		NULL,NULL,hInstance,NULL);*/

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	hDC = GetDC(hwnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    
    time2 = std::chrono::system_clock::now();
	time1 = time2;
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count();
    
    //inits
    //initEmu("D:/Chip-8 Emulator Folder/CHIP8Emulator/Rom/PONG.ch8");
    glInit();

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while (1) {
		while(PeekMessage(&msg, hwnd, 0, 0, PM_NOREMOVE)) {
		    if(GetMessage(&msg, hwnd, 0, 0)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
		    } else {
				goto quit;
		    }
		}
		display();
    }

quit:

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hwnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hwnd);
    if (hPalette)
	DeleteObject(hPalette);

    return 0;
}
