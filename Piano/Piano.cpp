#include "stdafx.h"
#include "Piano.h"
#include "MainWindow.h"
#include "Controls.h"
#pragma warning(push)
#pragma warning(disable:4711)
#	include "PianoKeyboard\Keyboard.h"
#pragma warning(pop)
#include "PianoSound\Sound_Facade.h"

#include <vld.h>

using namespace std;

vector<vector<set<int16_t>>> Piano::notes = vector<vector<set<int16_t>>>();
vector<vector<pair<unsigned, unsigned>>> Piano::milliSeconds
	= vector<vector<pair<unsigned, unsigned>>>();

shared_ptr<Keyboard> Piano::keyboard = make_shared<Keyboard>();
shared_ptr<Sound_Facade> Piano::sound = nullptr;

vector<size_t> Piano::indexes = vector<size_t>();
vector<size_t> Piano::tracks = vector<size_t>();
shared_ptr<size_t> Piano::leftTrack = nullptr;
shared_ptr<size_t> Piano::rightTrack = nullptr;

vector<vector<vector<string>>> Piano::fingersLeft = vector<vector<vector<string>>>();
vector<vector<vector<string>>> Piano::fingersRight = vector<vector<vector<string>>>();


ATOM Piano::MyRegisterClass(const HINSTANCE hInstance)
{
	WNDCLASSEXW wcex{ sizeof wcex };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWindow::WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LARGE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MENU);
	wcex.lpszClassName = szWindowClass_;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	return RegisterClassEx(&wcex);
}

inline BOOL Piano::InitInstance(const HINSTANCE hInstance, const int nCmdShow)
{
	MainWindow::hWndMain = CreateWindowEx(WS_EX_ACCEPTFILES, szWindowClass_, TEXT("Piano Fingers"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		HWND_DESKTOP, nullptr, hInstance, nullptr);
		if (!MainWindow::hWndMain) return FALSE;

		ShowWindow(MainWindow::hWndMain, SW_SHOWMAXIMIZED);
#ifdef _DEBUG
		UNREFERENCED_PARAMETER(nCmdShow);
#else
		ShowWindow(MainWindow::hWndMain, nCmdShow);
#endif
		UpdateWindow(MainWindow::hWndMain);

		return TRUE;
}

int Piano::Main(const HINSTANCE hInstance, const int nCmdShow)
{
	Piano::MyRegisterClass(hInstance);
	if (!Piano::InitInstance(hInstance, nCmdShow)) return FALSE;

    const auto hAccelTable(LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MENU)));
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(MainWindow::hWndMain, hAccelTable, &msg) &&
			!IsDialogMessage(Controls::hDlgControls, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return static_cast<int>(msg.wParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
	return Piano::Main(hInstance, nCmdShow);
}