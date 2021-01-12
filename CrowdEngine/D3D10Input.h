#ifndef D3D10_INPUT_H_INCLUDED
#define D3D10_INPUT_H_INCLUDED

#include "Input.h"

namespace Crowd
{

class CInputManager : public Crowd::InputManager
{
	friend class InputManager;

	protected:
	CInputManager();
	virtual ~CInputManager();

	public:
	virtual Status Initialize(Window &window);

	virtual void Update();

	protected:
	DirectInputPtr			m_inputDevice;
	MouseDevicePtr			m_mouseDevice;
	KeyboardDevicePtr		m_keyboardDevice;
};

} // namespace Crowd


#endif // D3D10_INPUT_H_INCLUDED