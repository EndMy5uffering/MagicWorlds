#pragma once
#include "Scene.h"

class DefaultScene : Scene
{

private:

	Camera cam;

public:

	DefaultScene() : Scene{ "DefaultScene" }, cam{ 500, 500, {0,0,0} }
	{
	}

	void Render(Renderer& renderer) override 
	{
	
	}

	void Update(GLFWwindow& hwin, float dt) override
	{
	
	}

};

