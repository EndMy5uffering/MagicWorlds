#include "UILib.h"

Transform& UILib::GUIElement::GetTransform()
{
	return this->m_transform;
}

void UILib::GUIElement::Add(const GUIElement& elem)
{
	this->m_children.push_back(elem);
}

