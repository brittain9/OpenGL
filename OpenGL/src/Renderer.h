#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
	#define GLCALL(x) GLClearError();\
			x;\
			ASSERT(GLLogCall(#x, __FILE__, __LINE__)) // # turns c into string
#else
	#define GLCALL(x) x
#endif // DEBUG


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const; // make const because no modifications
};