#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/matrix_operation.hpp"
#include <vector>

namespace UILib
{

	class GUIManager 
	{
	

		
	};

	class GUIElement 
	{
	private:
		glm::mat4 m_model;
		std::vector<GUIElement&> m_children;

		bool m_is_showing = true;



	public:
		GUIElement(const glm::vec2 pos, const glm::vec2 scale, const float rot_rad = 0) 
			:
			m_model{ glm::diagonal4x4(glm::vec4{1,1,1,1}) }
		{
			
		}

		virtual void Draw() = 0;

	};

	class GUIWindow : GUIElement
	{
	
	public:
		GUIWindow(const glm::vec2 pos, glm::vec2 scale) : GUIElement(pos, scale) 
		{
			
		}

		void Draw() override 
		{
				
		}

	};


};

