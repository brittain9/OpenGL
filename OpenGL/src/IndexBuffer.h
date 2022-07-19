#pragma once

class IndexBuffer
{
	unsigned int m_RendererID; // every buffer/object gets a unique ID
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count); // size in bytes, count = element count
	~IndexBuffer();

	void Bind() const; // make const because they dont modify anything
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
 
