#pragma once

class VertexBuffer
{
	unsigned int m_RendererID; // every buffer/object gets a unique ID
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

