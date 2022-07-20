#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"

// Projection is the math behind renderering a 3d world to a 2d monitor. 3d points to 2d.
// Orthographic: usually 2d projection
// Perspective : usually 3d projection

// View and model matrix. All of these get multiplied by vertex to put position on screen
// MVP: model view projection matrix. transformation pipeline of vertex
// View matrix: camera position and orientation
// model: translation, rotation, and scale

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


int width{ 960 };
int height{ 540  };

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

    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    {
        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f,   // 0 bottom left
        	200.0f, 100.0f, 1.0f, 0.0f,   // 1 right side of texture
        	200.0f, 200.0f, 1.0f, 1.0f,   // 2 top right
            100.0f, 200.0f, 0.0f, 1.0f    // 3 top left
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

        glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.5f, 1.0f);

        Texture texture("res/textures/foam.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0); // 0 matches slot 0


        va.Unbind(); 
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        glm::vec3 translation{ 200, 200, 0 };

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            // Render loop
            renderer.Clear();
            GLCALL(glClearColor(0.9f, 0.9f, 0.9f, 1.0f));

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            glm::mat4 model = glm::translate(glm::mat4(1.0f),translation);
            glm::mat4 mvp = proj * view * model; // reverse order multiplication


            shader.Bind();
            shader.SetUniform4f("u_Color", 0.3f, 0.3f, 0.8f ,1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);

            {
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            glfwPollEvents(); // check if input events triggered
        }

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}