#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OPENGL ERROR] (" << error << ") " << function << " : " << file << " : " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind(); // shaders/programs. Tells the GPU what to do. Nothing to do with lighting
    va.Bind(); // Array that contains all the actual data that we do stuff with. Vertex buffer and index buffer
    ib.Bind(); // contains indices
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
