#include "InputSystem.h"

std::unordered_map<GLFWwindow*, InputSystem*> InputSystem::m_win_inputs;

InputSystem::InputSystem(GLFWwindow* window) : m_window{ window }
{
	
	m_win_inputs[window] = this;
	glfwSetKeyCallback(m_window, InputSystem::KeyCallback);
	glfwSetScrollCallback(m_window, InputSystem::ScrollCallback);
	glfwSetCursorPosCallback(m_window, InputSystem::MouseMoveCallback);
	glfwSetMouseButtonCallback(m_window, InputSystem::MouseButtonCallback);

}

InputSystem::~InputSystem()
{
	InputSystem::m_win_inputs.erase(m_window);
	glfwSetKeyCallback(m_window, nullptr);
	glfwSetScrollCallback(m_window, nullptr);
	glfwSetCursorPosCallback(m_window, nullptr);
}

/*void InputSystem::Update()
{
	m_scroll = 0.0;

	for (unsigned long i = 0; i < m_num_mouse_buttons; i++) {
		m_mouse_buttons_last[i] = m_mouse_buttons[i];
	}


}*/

bool InputSystem::IsKey(int key_code)
{
	if (key_code > m_num_keys || key_code < 0) return false;
	return this->m_keys[key_code];
}

bool InputSystem::IsKeyUp(int key_code)
{
	if (key_code > m_num_keys || key_code < 0) return false;
	return !this->m_keys[key_code] && m_keys_last[key_code];
}

bool InputSystem::IsKeyDown(int key_code)
{
	if (key_code > m_num_keys || key_code < 0) return false;
	return this->m_keys[key_code] && !m_keys_last[key_code];
}

bool InputSystem::IsButton(int button_code)
{
	if (button_code > m_num_mouse_buttons || button_code < 0) return false;
	return m_mouse_buttons[button_code];
}

bool InputSystem::IsButtonUp(int button_code)
{
	if (button_code > m_num_mouse_buttons || button_code < 0) return false;
	return !m_mouse_buttons[button_code] && m_mouse_buttons_last[button_code];
}

bool InputSystem::IsButtonDown(int button_code)
{
	if (button_code > m_num_mouse_buttons || button_code < 0) return false;
	return m_mouse_buttons[button_code] && !m_mouse_buttons_last[button_code];
}

int InputSystem::GetMouseX()
{
	return m_mouse_x;
}

int InputSystem::GetMouseY()
{
	return m_mouse_y;
}

glm::vec2 InputSystem::GetMousePosition()
{
	return glm::vec2{float(m_mouse_x), float(m_mouse_y)};
}

double InputSystem::GetScroll()
{
	return m_scroll;
}

void InputSystem::UpdateKeyInputs(int key, int scancode, int action, int mods)
{
	for (unsigned long i = 0; i < m_num_keys; i++) {
		m_keys_last[i] = m_keys[i];
	}

	switch (action)
	{
	case GLFW_PRESS: 
	{
		if (key > m_num_keys || key < 0) break;
		m_keys[key] = true;
	} break;
	case GLFW_REPEAT: 
	{
		if (key > m_num_keys || key < 0) break;
		m_keys[key] = true;
	} break;
	case GLFW_RELEASE: 
	{
		if (key > m_num_keys || key < 0) break;
		m_keys[key] = false;
	} break;

	default:
		break;
	}


}

void InputSystem::UpdateButtonInputs(int button, int action, int mods)
{
	for (unsigned long i = 0; i < m_num_mouse_buttons; i++) {
		m_mouse_buttons_last[i] = m_mouse_buttons[i];
	}

	switch (action)
	{
	case GLFW_PRESS:
	{
		if (button > m_num_mouse_buttons || button < 0) break;
		m_mouse_buttons[button] = true;
	} break;
	case GLFW_RELEASE:
	{
		if (button > m_num_mouse_buttons || button < 0) break;
		m_mouse_buttons[button] = false;
	} break;
	default:
		break;
	}
}