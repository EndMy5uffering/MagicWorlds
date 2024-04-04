#pragma once
#include "Renderer.h"
#include "Camera.h"
#include <string>

class Scene
{
private:

	std::string m_name;

	virtual void Load() {};
	virtual void Unload() {};

public:
	Scene(const std::string name) : m_name{name} {}

    ~Scene() { Uninitialize(); }

	virtual void Render(Renderer &renderer) = 0;
	virtual void Update(GLFWwindow &hwin, float dt) = 0;

	void Initialize() 
	{
		std::cout << "Loading scene: [ " << m_name << " ]\n";
		Load();
		std::cout << "Scene loaded\n";
	}

	void Uninitialize() 
	{
		std::cout << "Unloading scene: [ " << m_name << " ]\n";
		Unload();
		std::cout << "Scene unloaded\n";
	}
};


