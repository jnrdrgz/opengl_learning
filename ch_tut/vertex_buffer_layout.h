#pragma once

#include <vector>
#include <GL/glew.h>
#include "renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}

		ASSERT(false);
		return 0;
	}
};

struct VertexBufferLayout {
	VertexBufferLayout() : m_stride{0} {}

	template<typename T>
	void Push(unsigned int count)
	{

	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int  count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int  count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& getElements() const { return m_elements; }
	inline unsigned int getStride() const { return m_stride; }

	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;

};