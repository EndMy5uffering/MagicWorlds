#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/matrix_operation.hpp"
#include <vector>
#include <array>
#include <queue>
#include <stack>
#include "Renderer.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"
#include "InputSystem.h"

namespace UILib
{

	class UIManager;

	struct UIVertex
	{
		glm::vec4 pos;
		glm::vec4 color;
		glm::vec2 uvpos;
	};
	
	class GUIElement 
	{
	private:
		Transform m_transform;
		Transform m_transform_cash;
		GUIElement* m_parent = nullptr;
		std::vector<GUIElement> m_children;
		glm::vec4 m_color;
		glm::vec4 m_uv_range = {0, 0, 1, 1};

		bool m_is_visiblie = true;

		bool m_mouse_enter = false;

	public:
		Transform& GetTransform();

		void Add(const GUIElement& elem);

		void Update(UIManager& uiManager) {} // Empty element is not supposed to do anything
		//bool Render(const UIManager& uiManager, Quad& out); //Should be implemented by all subclasses

		bool IsVisible() { return m_is_visiblie; }
		bool MouseEnter() { return m_mouse_enter; }

		void SetColor(const glm::vec4& col) { m_color = col; }
		glm::vec4 GetColor() { return m_color; }

		void SetUVRange(const glm::vec4& uv) { m_uv_range = uv; }
		glm::vec4 GetUVRange() { return m_uv_range; }

		std::vector<GUIElement>& GetChildren() { return m_children; }
		size_t GetChildCount() { return m_children.size(); }

		bool HasParent() { return m_parent != nullptr; }
		void SetParent(GUIElement* parent) { m_parent = parent; }
		GUIElement* GetParent() { return m_parent; }

	};

	class UIManager
	{
	private:
		Shader& m_uishader;
		Renderer& m_renderer;
		Camera& m_camera;
		InputSystem& m_inputSystem;

		std::vector<GUIElement> m_elements;
		Mesh<UIVertex> m_ui_mesh;

		bool m_ui_mesh_updated = false;
		bool m_ui_element_added = false;
		bool m_packed = false;


		void MakeQuadVerts(std::vector<UIVertex>& vbuff, GUIElement* el, glm::mat4 parent) 
		{
			const glm::mat4 elTransform = parent * el->GetTransform().GetMatrix();
			
			glm::vec4 pos0 = elTransform * glm::vec4{ 0.5, 0.5, 0, 1 };
			glm::vec4 pos1 = elTransform * glm::vec4{ 0.5, -0.5, 0, 1 };
			glm::vec4 pos2 = elTransform * glm::vec4{ -0.5, -0.5, 0, 1 };
			glm::vec4 pos3 = elTransform * glm::vec4{-0.5, 0.5, 0, 1};

			glm::vec4 uvRange = el->GetUVRange();

			vbuff.push_back({ pos0, el->GetColor(), {uvRange.x, uvRange.y} });
			vbuff.push_back({ pos1, el->GetColor(), {uvRange.x, uvRange.w} });
			vbuff.push_back({ pos2, el->GetColor(), {uvRange.z, uvRange.w} });
			vbuff.push_back({ pos3, el->GetColor(), {uvRange.z, uvRange.y} });
		}

		void MakeIdxList(std::vector<unsigned int> &idx, size_t& off) 
		{
			idx.push_back(0 + off);
			idx.push_back(1 + off);
			idx.push_back(2 + off);
			idx.push_back(0 + off);
			idx.push_back(2 + off);
			idx.push_back(3 + off);
			off += 4;
		}

		void Pack_helper(GUIElement& root, glm::mat4& parent, size_t& idxOffset, std::vector<UIVertex>& verts, std::vector<unsigned int>& idx_list)
		{
			if (!root.IsVisible()) return;

			glm::mat4 transform = root.GetTransform().GetMatrix();
			MakeQuadVerts(verts, &root, parent * transform);
			MakeIdxList(idx_list, idxOffset);

			std::vector<GUIElement>& children = root.GetChildren();
			for (GUIElement& child: children) Pack_helper(child, transform, idxOffset, verts, idx_list);
			
		}

		void Pack()
		{
			VertexBufferLayout vbl;
			vbl.Push<glm::vec4>();
			vbl.Push<glm::vec4>();
			vbl.Push<glm::vec2>();

			std::vector<UIVertex> verts;
			std::vector<unsigned int> idx_list;

			size_t idxOffset = 0;

			glm::mat4 idn = glm::mat4{ 1 };
			for (GUIElement& el : m_elements) Pack_helper(el, idn, idxOffset, verts, idx_list);

			m_ui_mesh.SetIndexBuffer(idx_list);
			m_ui_mesh.SetVertexBufferContent(verts);

			m_ui_mesh.PackDynamic();

			m_ui_mesh_updated = false;
			m_ui_element_added = false;
			m_packed = true;
		}

		void Repack()
		{

		}

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
			m_ui_element_added = true;
		}

		void RemoveElement() 
		{
		
		}

		void RenderUIElements()
		{


			m_uishader.Bind();

			m_uishader.SetUniformMat4f("u_proj", m_camera.GetProjMatrix());
			m_uishader.SetUniformMat4f("u_view", m_camera.GetViewMatrix());


		}

	};

};

