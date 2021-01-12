//
//	Input.h
//
// Se charge de la gestion des entrées (souris, clavier).
//
// La dll, chargée généralement avec une classe dérivée de Library (EngineLoader), doit appeler Global::Initialize(engineLoader.GetGlobalInformation())
// pour synchroniser l'application et la dll.

#ifndef CRWD_INPUT_H_INCLUDED
#define CRWD_INPUT_H_INCLUDED

#include "D3DEntry.h"
#include "Resources.h"
#include "Status.h"
#include "CRWDMath.h"
#include "Window.h"

namespace Crowd
{

struct MouseState
{
	int						transX;
	int						transY;
	int						transZ;
	unsigned char			buttons[3];
};

enum CRWD_KBD {
	CRWD_KBD_A = 16,
	CRWD_KBD_Z = 17,
	CRWD_KBD_E = 18,
	CRWD_KBD_R = 19,
	CRWD_KBD_T = 20,
	CRWD_KBD_Y = 21,
	CRWD_KBD_U = 22,
	CRWD_KBD_I = 23,
	CRWD_KBD_O = 24,
	CRWD_KBD_P = 25,
	CRWD_KBD_Q = 30,
	CRWD_KBD_S = 31,
	CRWD_KBD_D = 32,
	CRWD_KBD_F = 33,
	CRWD_KBD_G = 34,
	CRWD_KBD_H = 35,
	CRWD_KBD_J = 36,
	CRWD_KBD_K = 37,
	CRWD_KBD_L = 38,
	CRWD_KBD_M = 39,
	CRWD_KBD_UP = 200,
	CRWD_KBD_DOWN = 208,
	CRWD_KBD_LEFT = 203,
	CRWD_KBD_RIGHT = 205
};


//
//	Gère les entrées clavier et souris.
//	A initialiser avant utilisation, puis utiliser la fonction Update() à chaque frame pour mettre à jour les entrées.
//
class InputManager
{
	public:
	friend class RefCnt<InputManager>;
	typedef SmartPtr<InputManager, RefCnt> ptr;

	friend class EngineLoader;
	friend class Global;

	CRWD_EXPORT static InputManager &GetInstance();

	virtual Status Initialize(Window &window) = 0;

	virtual void Update() = 0;
	CRWD_EXPORT virtual const char	*GetKeyboard() const;
	CRWD_EXPORT virtual MouseState		GetMouse() const;

	protected:
	static InputManager::ptr			m_instance;

	char								m_keyboardState[256];
	MouseState							m_mouseState;
};

} // namespace Crowd

#endif // CRWD_INPUT_H_INCLUDED