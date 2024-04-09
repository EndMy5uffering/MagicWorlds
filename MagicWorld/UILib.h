#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/matrix_operation.hpp"
#include <vector>
#include "Renderer.h"
#include "Transform.h"

namespace UILib
{



	class GUIElement 
	{
	private:
		Transform m_transform;
		std::vector<GUIElement> m_children;

		bool m_is_visiblie = true;

		bool m_mouse_enter = false;

	public:
		Transform& GetTransform();

		void AddElement(const GUIElement& elem);
	};

	class GUIWindow : GUIElement
	{
	private:
		std::string m_window_name = "";


	public:


	};

};

