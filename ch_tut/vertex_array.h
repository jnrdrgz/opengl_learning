#pragma once

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

struct VertexArray {
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;


	unsigned int m_RendererId;
};