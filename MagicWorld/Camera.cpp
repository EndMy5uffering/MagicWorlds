#include "Camera.h"

#include <iostream>

Camera::Camera(int width, int height) :
	mWidth{ width },
	mHeight{ height },
	forward(glm::vec3(0.0f, 0.0f, -1.0f)), 
	position(glm::vec3(0.0f, 0.0f, 0.0f)), 
	up(glm::vec3(0.0f, 1.0f, 0.0f))
{
}

Camera::Camera(int width, int height, glm::vec3 startPos) :
	mWidth{ width },
	mHeight{ height },
	forward(glm::vec3(0.0f, 0.0f, -1.0f)),
	position(startPos),
	up(glm::vec3(0.0f, 1.0f, 0.0f))
{
}

Camera::Camera(int width, int height, glm::vec3 startPos, glm::vec3 forward) :
	mWidth{ width },
	mHeight{ height },
	forward(forward),
	position(startPos),
	up(glm::vec3(0.0f, 1.0f, 0.0f))
{
}

Camera::Camera(int width, int height, glm::vec3 startPos, glm::vec3 forward, glm::vec3 up) :
	mWidth{ width },
	mHeight{ height },
	forward(forward),
	position(startPos), up(up)
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewProjMatrix()
{
	return glm::perspective(glm::radians(mFOV), ((float)mWidth / (float)mHeight), mNear, mFar) * 
		glm::lookAt(this->position, this->position + this->forward, this->up);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->position, this->position + this->forward, this->up);
}

glm::mat4 Camera::GetProjMatrix()
{
	return glm::perspective(glm::radians(mFOV), ((float)mWidth / (float)mHeight), mNear, mFar);
}

glm::vec3 Camera::GetPosition()
{
	return position;
}

float Camera::GetFar()
{
	return this->mFar;
}

float Camera::GetNear()
{
	return this->mNear;
}

float Camera::GetFOV()
{
	return this->mFOV;
}

float Camera::GetScreenWidth()
{
	return this->mWidth;
}

float Camera::GetScreenHeight()
{
	return this->mHeight;
}

glm::vec2 Camera::GetViewport()
{
	return glm::normalize(glm::vec2(mWidth, mHeight));
}

glm::vec2 Camera::GetFocal()
{
	float d = (2.0f * tanf(mFOV * 0.5f));
	return glm::vec2{ mWidth / d, mHeight / d };
}

float Camera::GetSpeed()
{
	return mSpeed;
}

bool Camera::IsViewFixed()
{
	return this->mFixViewPoint;
}

bool Camera::IsPositionFixed()
{
	return mFixPostion;
}

void Camera::HandleInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += forward * mSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += forward * -mSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += -mSpeed * glm::normalize(glm::cross(forward, up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += mSpeed * glm::normalize(glm::cross(forward, up));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		up = glm::rotate(up, glm::radians(1.0f), forward);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		up = glm::rotate(up, glm::radians(-1.0f), forward);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		mSpeed = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		mSpeed = 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		position += mSpeed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		position += -mSpeed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !mCaptureMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPos(window, (double)mWidth / 2.0, (double)mHeight / 2.0);
		mFirstCapture = false;
		mCaptureMouse = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		mCaptureMouse = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (mCaptureMouse) 
	{
		HandleCamRotation(window);
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS))
	{
		std::cout << "POS: { 'x': " << position.x << ", 'y': " << position.y << ", 'z': " << position.z << "}\n";
		std::cout << "FOR: { 'x': " << forward.x << ", 'y': " << forward.y << ", 'z': " << forward.z << "}\n";
		std::cout << "UP : { 'x': " << up.x << ", 'y': " << up.y << ", 'z': " << up.z << "}\n";
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		std::cout << "{ " << position.x << ", " << position.y << ", " << position.z << "},\n";
		std::cout << "{ " << forward.x << ", " << forward.y << ", " << forward.z << "},\n";
		std::cout << "{ " << up.x << ", " << up.y << ", " << up.z << "}\n";
	}
}

void Camera::Resize(int width, int height)
{
	this->mWidth = width;
	this->mHeight = height;
}

void Camera::HandleCamRotation(GLFWwindow* window)
{
	if (mFixViewPoint) return;
	double mouseX, mouseY;
	
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glfwSetCursorPos(window, (double)mWidth / 2.0, (double)mHeight / 2.0);

	double rotX = -mSensitivity * (mouseY - int((double)mHeight / 2.0)) / (double)(mHeight);
	double rotY = -mSensitivity * (mouseX - int((double)mWidth / 2.0)) / (double)(mWidth);

	if(!mLockX) forward = glm::rotate(forward, (float)glm::radians(rotX), glm::normalize(glm::cross(forward, up)));

	glm::vec3 side = glm::normalize(glm::cross(up, forward));
	up = glm::normalize(glm::cross(forward, side));

	if(!mLockY) forward = glm::rotate(forward, (float)glm::radians(rotY), up);
}

void Camera::SetIsViewFixedOnPoint(bool fixed, glm::vec4 point)
{
	SetIsViewFixedOnPoint(fixed);

	if (fixed) 
	{
		forward = glm::normalize(glm::vec3(point) - position);

		glm::vec3 side = glm::normalize(glm::cross(up, forward));
		up = glm::normalize(glm::cross(forward, side));
	}
}

void Camera::SetIsViewFixedOnPoint(bool fixed)
{
	this->mFixViewPoint = fixed;
	if (fixed) 
	{
		mCaptureMouse = false;
	}
}

void Camera::SetIsPositionFixed(bool fixed)
{
	this->mFixPostion = fixed;
}

void Camera::SetSpeed(float speed)
{
	this->mSpeed = speed;
}
