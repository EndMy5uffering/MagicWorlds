#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/matrix_operation.hpp"
#include <vector>
#include <array>
#include <queue>
#include "Renderer.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"
#include "InputSystem.h"

namespace UILib
{

	struct UIVertex
	{
		glm::vec4 pos;
		glm::vec4 color;
		glm::vec2 uvpos;
	};
	
	struct Quad 
	{
		Texture& tex;
		std::array<UIVertex, 4> verts{
			UIVertex{ {1.0, 1.0, 0.0, 1.0}, { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0 } },
			UIVertex{ {1.0, -1.0, 0.0, 1.0}, { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0 } },
			UIVertex{ {-1.0, -1.0, 0.0, 1.0}, { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0 } },
			UIVertex{ {-1.0, 1.0, 0.0, 1.0}, { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0 } }
		};
		std::array<unsigned int, 6> idx_list{0, 1, 2, 2, 3, 0};

		void AddOffset(unsigned int off) { for (int i = 0; i < 6; ++i) idx_list[i] += off; }

		void AddTransform(const Transform& transform) 
		{
			const glm::mat4 t = transform.GetMatrix();
			verts[0].pos = t * verts[0].pos;
			verts[1].pos = t * verts[1].pos;
			verts[2].pos = t * verts[2].pos;
			verts[3].pos = t * verts[3].pos;
		}

		void SetUV(int idx, const glm::vec2& uv) 
		{
			if (idx < 0 || idx > 3) return;
			verts[idx].uvpos = uv;
		}
	};

	class GUIElement 
	{
	private:
		Transform m_transform;
		Transform m_transform_cash;
		std::vector<GUIElement> m_children;
		glm::vec4 m_color;
		glm::vec4 m_uv_range = {0, 0, 1, 1};

		bool m_is_visiblie = true;

		bool m_mouse_enter = false;

	public:
		Transform& GetTransform();

		void Add(const GUIElement& elem);

		void Update() {} // Empty element is not supposed to do anything
		//bool Render(const UIManager& uiManager, Quad& out); //Should be implemented by all subclasses

		bool IsVisible() { return m_is_visiblie; }
		bool MouseEnter() { return m_mouse_enter; }

		void SetColor(const glm::vec4& col) { m_color = col; }
		glm::vec4 GetColor() { return m_color; }

		void SetUVRange(const glm::vec4& uv) { m_uv_range = uv; }
		glm::vec4 GetUVRange() { return m_uv_range; }

		std::vector<GUIElement>& GetChildren() { return m_children; }
		size_t GetChildCount() { return m_children.size(); }
	};

	class UIManager
	{
	private:
		Shader& m_uishader;
		Renderer& m_renderer;
		Camera& m_camera;
		InputSystem& m_inputSystem;

		std::vector<GUIElement> m_elements;

	public:

		UIManager(Shader& shader, Renderer& renderer, Camera& cam, InputSystem& is) 
			:
			m_uishader{ shader }, 
			m_renderer{ renderer },
			m_camera{ cam },
			m_inputSystem{ is }
		{}

		~UIManager() {}

		void AddElement(GUIElement& element) 
		{
			m_elements.push_back(element);
		}

		void RemoveElement() 
		{
		
		}

		void RenderUIElements()
		{
			VertexBufferLayout vbl;
			vbl.Push<glm::vec4>();
			vbl.Push<glm::vec4>();
			vbl.Push<glm::vec2>();

			std::queue<GUIElement> processQueue;

			std::vector<UIVertex> verts;
			std::vector<unsigned int> idx_list;

			

			Mesh<UIVertex> mesh{
				verts, idx_list, vbl
			};

			m_uishader.Bind();

			m_uishader.SetUniformMat4f("u_proj", m_camera.GetProjMatrix());
			m_uishader.SetUniformMat4f("u_view", m_camera.GetViewMatrix());

			mesh.Draw(m_renderer);
			mesh.Delete();

		}

	};

};

