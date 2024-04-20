#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/matrix_operation.hpp"
#include <vector>
#include <queue>
#include "Renderer.h"
#include "Transform.h"
#include "Mesh.h"

namespace UILib
{




	class GUIElement 
	{
	private:
		Transform m_transform;
		std::vector<GUIElement> m_children;
		glm::vec4 m_color;
		glm::vec4 m_uv_range = {0, 0, 1, 1};

		bool m_is_visiblie = true;

		bool m_mouse_enter = false;

	public:
		Transform& GetTransform();

		void Add(const GUIElement& elem);

		bool IsVisible() { return m_is_visiblie; }
		bool MouseEnter() { return m_mouse_enter; }

		void SetColor(const glm::vec4& col) { m_color = col; }
		glm::vec4 GetColor() { return m_color; }

		void SetUVRange(const glm::vec4& uv) { m_uv_range = uv; }
		glm::vec4 GetUVRange() { return m_uv_range; }

		std::vector<GUIElement>& GetChildren() { return m_children; }
		size_t GetChildCount() { return m_children.size(); }
	};

	struct UIVertex 
	{
		glm::vec4 pos;
		glm::vec4 uvpos;
		glm::vec4 color;
	};

	

	class UIManager
	{
	private:
		Shader& m_uishader;
		Renderer& m_renderer;
		Camera& m_camera;

		std::vector<GUIElement> m_elements;

	public:

		UIManager(Shader& shader, Renderer& renderer, Camera& cam) 
			:
			m_uishader{ shader }, 
			m_renderer{ renderer },
			m_camera{ cam }
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
			vbl.Push<glm::vec4>();

			std::queue<GUIElement> processQueue;

			std::vector<UIVertex> verts;
			std::vector<unsigned int> idx_list;

			int itemOffset = 0;
			for (GUIElement& el : m_elements) 
			{
				//process root element
				//check if element has a size
				glm::vec3 scale = el.GetTransform().GetScale();
				if (el.IsVisible() && scale.x >= 0 && scale.y >= 0) 
				{
					//add all children to queue
					for (GUIElement& c : el.GetChildren()) processQueue.push(c);
					
					//Getting all important data to render the element
					glm::vec4 col = el.GetColor();
					glm::vec4 uv_range = el.GetUVRange();
					glm::mat4 trans = el.GetTransform().GetMatrix();


					//Adding verts to vert buffer
					verts.push_back(UIVertex{ trans * glm::vec4{1, 1, -1, 1}, uv_range, col });
					verts.push_back(UIVertex{ trans * glm::vec4{1, -1, -1, 1}, uv_range, col });
					verts.push_back(UIVertex{ trans * glm::vec4{-1, -1, -1, 1}, uv_range, col });
					verts.push_back(UIVertex{ trans * glm::vec4{-1, 1, -1, 1}, uv_range, col });

					//adding 
					idx_list.push_back(0 + itemOffset);
					idx_list.push_back(1 + itemOffset);
					idx_list.push_back(2 + itemOffset);
					idx_list.push_back(2 + itemOffset);
					idx_list.push_back(3 + itemOffset);
					idx_list.push_back(0 + itemOffset);

				}
				itemOffset += 6;
				
				//Process children
				for ( ; !processQueue.empty(); processQueue.pop()) 
				{
				
					GUIElement& child = processQueue.front();

					//check if element has a size
					glm::vec3 scale = child.GetTransform().GetScale();
					if (child.IsVisible() && scale.x >= 0 && scale.y >= 0 && child.GetChildCount() > 0)
					{
						//add all children to queue
						for (GUIElement& c : child.GetChildren()) processQueue.push(c);

						//Getting all important data to render the element
						glm::vec4 col = child.GetColor();
						glm::vec4 uv_range = child.GetUVRange();
						glm::mat4 trans = child.GetTransform().GetMatrix();

						//Adding verts to vert buffer
						verts.push_back(UIVertex{ trans * glm::vec4{1, 1, 0, 1}, uv_range, col });
						verts.push_back(UIVertex{ trans * glm::vec4{1, -1, 0, 1}, uv_range, col });
						verts.push_back(UIVertex{ trans * glm::vec4{-1, -1, 0, 1}, uv_range, col });
						verts.push_back(UIVertex{ trans * glm::vec4{-1, 1, 0, 1}, uv_range, col });

						//adding 
						idx_list.push_back(0 + itemOffset);
						idx_list.push_back(1 + itemOffset);
						idx_list.push_back(2 + itemOffset);
						idx_list.push_back(2 + itemOffset);
						idx_list.push_back(3 + itemOffset);
						idx_list.push_back(0 + itemOffset);

					}
					itemOffset += 6;

				}

			}

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

