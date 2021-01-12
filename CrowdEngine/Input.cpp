#include "Input.h"
#include "D3D10Input.h"

namespace Crowd
{

InputManager::ptr InputManager::m_instance(new CInputManager);

InputManager &InputManager::GetInstance()
{
	return *m_instance;
}

const char *InputManager::GetKeyboard() const
{
	return m_keyboardState;
}

MouseState InputManager::GetMouse() const
{
	return m_mouseState;
}

} // namespace Crowd