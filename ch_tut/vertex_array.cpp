#include "vertex_array.h"

#include "renderer.h"
#include <iostream>

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererId));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, VertexBufferLayout& layout) {
	
	Bind();
	vb.Bind();
	
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));

		std::cout << "-" << i << "-" << element.count << "-" << element.type << "-" << 
			element.normalized << "-" << layout.getStride() << "-" << (const void*)offset << "\n";


		GLCall(glVertexAttribPointer(
			i, element.count, element.type, 
			element.normalized, layout.getStride(),(const void*) offset
		));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

	}

}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}