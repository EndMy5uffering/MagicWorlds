#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <array>
#include <vector>
#include <unordered_map>
#include <iostream>


// Perhaps to be able to set callbacks for individual keys in the future
//typedef void (*InputSystemKeyCallback)();

class InputSystem
{
private:

	static std::unordered_map<GLFWwindow*, InputSystem*> m_win_inputs;
	
	static const unsigned int m_num_keys = 600;
	std::array<bool, m_num_keys> m_keys{};
	std::array<bool, m_num_keys> m_keys_last{};

	// For key callbacks in the future
	//std::array<std::vector<InputSystemKeyCallback>, m_num_keys> m_key_callbacks{};

	static const unsigned int m_num_mouse_buttons = 10;
	std::array<bool, m_num_mouse_buttons> m_mouse_buttons{};
	std::array<bool, m_num_mouse_buttons> m_mouse_buttons_last{};

	double m_mouse_x = 0;
	double m_mouse_y = 0;
	double m_scroll = 0;

	GLFWwindow *m_window;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		InputSystem* is = InputSystem::m_win_inputs[window];

		if (is == nullptr) return;

		is->UpdateKeyInputs(key, scancode, action, mods);
	}

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		InputSystem* is = InputSystem::m_win_inputs[window];

		if (is == nullptr) return;

		is->m_scroll = yoffset;
	}

	static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
	{
		InputSystem* is = InputSystem::m_win_inputs[window];

		if (is == nullptr) return;

		is->m_mouse_x = xpos;
		is->m_mouse_y = ypos;
	}

public:

	InputSystem(GLFWwindow* window);
	~InputSystem();

	/**
	* Called by the engine to update the state of the input system.
	*/
	void Update();

	/**
	* Returns true if a key is pressed.
	* @param key_code The numerical identifyer for a key.
	*/
	bool IsKey(int key_code);

	/**
	* Returns true if a key was pressed an then released.
	* Only returns true once in the same update the key was released, otherwise false.
	* @param key_code The numerical identifyer for a key.
	*/
	bool IsKeyUp(int key_code);

	/**
	* Returns true if a key was pressed.
	* Only returns true once in the same update the key was pressed, otherwise false.
	* @param key_code The numerical identifyer for a key.
	*/
	bool IsKeyDown(int key_code);

	/**
	* Returns true if a mouse button was pressed.
	* @param button_code The numerical identifyer for a button.
	*/
	bool IsButton(int button_code);

	/**
	* Returns true if a mouse button was released.
	* Only returns true once in the same update the button was released, otherwise false.
	* @param button_code The numerical identifyer for a button.
	*/
	bool IsButtonUp(int button_code);

	/**
	* Returns true if a mouse button was pressed.
	* Only returns true once in the same update the button was pressed, otherwise false.
	* @param button_code The numerical identifyer for a button.
	*/
	bool IsButtonDown(int button_code);

	/**
	*
	*/
	int GetMouseX();

	/**
	*
	*/
	int GetMouseY();

	/**
	*
	*/
	glm::vec2 GetMousePosition();

	/**
	*
	*/
	double GetScroll();

	/**
	*
	*/
	void UpdateKeyInputs(int key, int scancode, int action, int mods);

};