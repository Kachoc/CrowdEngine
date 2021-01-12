#ifndef CE_WINDOW_H_INCLUDED
#define CE_WINDOW_H_INCLUDED

#if defined(_WIN32)	// _WIN32
#include <Windows.h>
#else // _WIN32
#error Plate-forme non supportée
#endif // différentes plates-formes

#include "Export.h"
#include "CEString.h"


namespace Crowd
{

enum
{
WND_CREATE_OVERLAPPED =		0x1,
WND_CREATE_FRAME =			0x2,
WND_CREATE_POPUP =			0x4,
WND_SIZE_DEFAULT =			0x10,
WND_SHOW_MINIMIZED =		0x100,
WND_SHOW_NORMAL =			0x200,
WND_SHOW_MAXIMIZED =		0x400,

WND_DEFAULT_OVERLAPPED =	WND_CREATE_OVERLAPPED | WND_SIZE_DEFAULT | WND_SHOW_MAXIMIZED,
};

class BaseWindow;


#if defined(_WIN32)	// _WIN32

typedef LRESULT (WINAPI *WndProc)(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

class BaseWindow
{
// classe à dériver pour faire les fenêtres de l'application, compatibles avec le Renderer
	protected:
	BaseWindow() : m_window(NULL), m_caption("") {}
	virtual ~BaseWindow() {}

	static CRWD_EXPORT bool TreatMessages();


	public:
	virtual CRWD_EXPORT HWND GetWindow() const;
	virtual CRWD_EXPORT String GetCaption() const;


	protected:
	friend class Renderer;

	HWND		m_window;
	String		m_caption;
};

const TCHAR wndClassBaseName[] = TEXT("WindowFromBaseWindowClassName");
class Window : public BaseWindow
{
	public:
	CRWD_EXPORT Window();
	CRWD_EXPORT Window(LPCTSTR name, HINSTANCE hInstance);
	virtual CRWD_EXPORT ~Window();

	virtual CRWD_EXPORT void SetCaption(const String &caption);
	virtual CRWD_EXPORT void GetClientRectangle(RECT &rectangle);

	virtual CRWD_EXPORT bool TreatMessages();	// retourne true juqu'à ce que la fenêtre soit détruite
	virtual CRWD_EXPORT void Destroy();

	static CRWD_EXPORT LRESULT WINAPI Proc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual CRWD_EXPORT HINSTANCE GetInstance() const;

	protected:
	void RegisterClass(HINSTANCE hInstance);
	virtual void Create(LPCTSTR name, HINSTANCE hInstance);

	protected:
	static bool m_classRegistered;

	HINSTANCE m_hInstance;
};


//
//	Pour récupérer le HMODULE dans une DLL.
//
#if _MSC_VER >= 1300 // for VC 7.0
// from ATL 7.0 sources
#ifndef _delayimp_h
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif
#endif
     
HMODULE GetCurrentModule();


#else	// _WIN32



#endif	// différentes plates-formes

} // namespace Crowd

#endif // CE_WINDOW_H_INCLUDED