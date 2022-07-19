#include <GL/glew.h>
#include <GLFW/glfw3.h>



#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



constexpr int width{ 1280 };
constexpr int height{ 720 };

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // From learnopengl Hello Window. this with glfwSetFrame will auto resize the viewport on window resize
    glViewport(0, 0, width, height);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // Specify version 3.3 for tutorial and compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Alex's Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLenum err = glewInit(); // Have to use glewInit() after openGL context created.
    if (GLEW_OK != err)
        fprintf(stderr, "Error: %s", glewGetErrorString(err));
    fprintf(stdout, "Status: Using GLEW %s", glewGetString(GLEW_VERSION));
    fprintf(stdout, " | Status: Using openGL %s\n", glGetString(GL_VERSION));

    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,   // 0 bottom left
             0.5f, -0.5f, 1.0f, 0.0f,   // 1 right side of texture
             0.5f,  0.5f, 1.0f, 1.0f,   // 2 top right
            -0.5f,  0.5f, 0.0f, 1.0f    // 3 top left
        };

        // index buffer create two triangles and share vertices instead of being redundant.
        unsigned int indices[] = {
            0, 1, 2, // triangle 1
            2, 3, 0  // triangle 2
        };

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        glViewport(0, 0, width, height);

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.125f, 1.125f, -1.0f, 1.0f); // 16:9 aspect ratio

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.5f, 1.0f);
        shader.SetUniformMat4f("u_MVP", proj);

        Texture texture("res/textures/explosion.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0); // 0 matches slot 0

        va.Unbind(); 
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float c = 0.0f;
        float inc = 0.01f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            // Render loop
            renderer.Clear();
            GLCALL(glClearColor(c, 0.0f, 0.0f, 1.0f));

            shader.Bind();
            //shader.SetUniform4f("u_Color", c, 0.3f, 0.8f ,1.0f);

            renderer.Draw(va, ib, shader);

            if (c > 1.0f) {
                inc = -0.01f;
            }
            else if (c < 0.0f) {
                inc = 0.01f;
            }

            c += inc;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            glfwPollEvents(); // check if input events triggered
        }

    }

    glfwTerminate();
    return 0;
}