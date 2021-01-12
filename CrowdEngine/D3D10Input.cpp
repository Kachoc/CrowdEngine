#include "D3D10Input.h"

namespace Crowd
{

CInputManager::CInputManager()
{
	ZeroMemory(m_keyboardState, sizeof(m_keyboardState));
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));
}

CInputManager::~CInputManager()
{
	m_keyboardDevice->Unacquire();
	m_mouseDevice->Unacquire();
}

Status CInputManager::Initialize(Window &window)
{
	HRESULT hr = DirectInput8Create(GetCurrentModule(), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*) m_inputDevice.GetPtrP(), NULL);
	if(FAILED(hr)) {
		return Status(String("D3D10::CInputManager::Initialize : ") + GetD3DXErrorString(hr));
	}

	hr = m_inputDevice->CreateDevice(GUID_SysKeyboard, m_keyboardDevice.GetPtrP(), NULL);
	if(FAILED(hr)) { return Status(String("D3D10::CInputManager::Initialize : ") + GetD3DXErrorString(hr)); }
	hr = m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr)) { return Status(String("D3D10::CInputManager::Initialize : ") + GetD3DXErrorString(hr)); }
	hr = m_keyboardDevice->SetCooperativeLevel(window.GetWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hr)) { return Status(String("D3D10::CInputManager::Initialize : ") + GetD3DXErrorString(hr)); }
	
	hr = m_keyboardDevice->Acquire();
	
	hr = m_inputDevice->CreateDevice(GUID_SysMouse, m_mouseDevice.GetPtrP(), NULL);
	if(FAILED(hr)) { return Status(String("D3D10::CInputManager::Initialize : ") + GetD3DXErrorString(hr)); }
	hr = m_mouseDevice->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr)) { return Status(String("D3D10::CInputManager::Initialize : ") + GetD3DXErrorString(hr)); }
	hr = m_mouseDevice->SetCooperativeLevel(window.GetWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hr)) { return Status(String("D3D10::CInputManager::Initialize : ") + GetD3DXErrorString(hr)); }

	hr = m_mouseDevice->Acquire();

	return Status::Success;
}

void CInputManager::Update()
{
	if(m_keyboardDevice) 
	{
		HRESULT hr = m_keyboardDevice->GetDeviceState(sizeof(m_keyboardState), (void**) &m_keyboardState);
		if(FAILED(hr))
		{
			ZeroMemory(m_keyboardState, sizeof(m_keyboardState));
			m_keyboardDevice->Acquire();
		}
	}
	else
	{
		ZeroMemory(m_keyboardState, sizeof(m_keyboardState));
	}

	if(m_mouseDevice)
	{
		DIMOUSESTATE2 mouseState;
		HRESULT hr = m_mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
		if(FAILED(hr))
		{
			ZeroMemory(&m_mouseState, sizeof(m_mouseState));
			m_mouseDevice->Acquire();
		}
		else
		{
			m_mouseState.transX = mouseState.lX;
			m_mouseState.transY = mouseState.lY;
			m_mouseState.transZ = mouseState.lZ;

			m_mouseState.buttons[0] = mouseState.rgbButtons[0];
			m_mouseState.buttons[1] = mouseState.rgbButtons[1];
			m_mouseState.buttons[2] = mouseState.rgbButtons[2];
		}
	}
	else
	{
		ZeroMemory(&m_mouseState, sizeof(m_mouseState));
	}
}

} // namespace Crowd