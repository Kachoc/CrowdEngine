#include "Debug.h"

#include "Window.h"

namespace Crowd
{

#if defined(_WIN32)	// _WIN32


//
//	BaseWindow
//
bool BaseWindow::TreatMessages()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_QUIT)
		{
			return false;
		}
	}
	return true;
}

HWND BaseWindow::GetWindow() const
{
	return m_window;
}

String BaseWindow::GetCaption() const
{
	return m_caption;
}


//
//	Window
//
bool Window::m_classRegistered(false);

Window::Window() : m_hInstance(NULL) {}

Window::Window(LPCTSTR name, HINSTANCE hInstance) : m_hInstance(NULL)
{
	Create(name, hInstance);
}

Window::~Window()
{
	if(m_window) SendMessage(m_window, WM_CLOSE, 0, 0);
}

void Window::SetCaption(const String &caption)
{
	SendMessage(m_window, WM_SETTEXT, 0, (LPARAM) caption.asChar());
	m_caption = caption;
}

void Window::GetClientRectangle(RECT &rectangle)
{
	if(m_window) {
		GetClientRect(m_window, &rectangle);
	}
}

bool Window::TreatMessages()
{
	MSG msg;
	msg.message = WM_NULL;

	while(PeekMessage(&msg, m_window, 0, 0, PM_REMOVE) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_DESTROY) return false;
	}

	return true;
}

void Window::Create(LPCTSTR name, HINSTANCE hInstance)
{
	try
	{
		RegisterClass(hInstance);
	}
	catch(Exception &e)
	{
		e.addFront("Impossible de créer une fenêtre:\n\n");
		throw e;
	}

	m_window = CreateWindow(wndClassBaseName, name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	m_caption = name;
	m_hInstance = hInstance;

	if(!m_window) throw InitFailure(GetLastErrorString(), "Création d'une fenêtre de type Window");

	ShowWindow(m_window, SW_MAXIMIZE);
	UpdateWindow(m_window);
}

void Window::Destroy()
{
	if(m_window)
	{
		DestroyWindow(m_window);
		m_window = NULL;
	}
	m_hInstance = NULL;
}

void Window::RegisterClass(HINSTANCE hInstance)
{
	if(m_classRegistered) return;

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = Proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_GRAYTEXT + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = wndClassBaseName;
	wc.hIconSm = NULL;

	if(!RegisterClassEx(&wc)) throw InitFailure(GetLastErrorString(), "Registration de la classe de fenêtre pour la classe Window");

	m_classRegistered = true;
}

LRESULT WINAPI Window::Proc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
		return 0;

		case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		default:
		return DefWindowProc(wnd, msg, wParam, lParam);
	}
}

HINSTANCE Window::GetInstance() const
{
	return m_hInstance;
}

HMODULE GetCurrentModule()
{
#if _MSC_VER < 1300 // earlier than .NET compiler (VC 6.0)
     
	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	VirtualQuery( &dummy, &mbi, sizeof(mbi) );
     
	return reinterpret_cast<HMODULE>(mbi.AllocationBase);
     
#else // VC 7.0
    // from ATL 7.0 sources
     
    return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
}

#else	// _WIN32

#endif	// différentes plates-formes

} // namespace Crowd