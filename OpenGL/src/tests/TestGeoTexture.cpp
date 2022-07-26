#include "TestGeoTexture.h"

#include <GLFW/glfw3.h>


#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test
{
	TestGeometryTexture2D::TestGeometryTexture2D()
		: m_Proj(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f)),
		m_View(glm::mat4(1.0f)),
		m_TranslationA(400, 300, 0)

	{
		glm::vec3 translationA(400, 300, 0);

		int rectSize = 3;

		float positions[] = {
			// positions																		// Colors			// tex coords
		static_cast<float>(-width / rectSize),static_cast<float>(-height / rectSize),0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		static_cast<float>(width / rectSize), static_cast<float>(-height / rectSize), 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		static_cast<float>(width / rectSize), static_cast<float>(height / rectSize), 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		static_cast<float>(-width / rectSize), static_cast<float>(height / rectSize), 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2, // triangle 1
			2, 3, 0  // triangle 2
		};

		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();

		m_VBO = std::make_unique<VertexBuffer>(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VBO, layout);
		m_IBO = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/VertexTexture.shader", "res/shaders/FragmentTexture.shader");
		m_Shader->Bind();
		m_Texture = std::make_unique<Texture>("res/textures/goat.png");
		m_Shader->SetUniform1i("u_Texture", 0); // 0 matches slot 0

	}

	TestGeometryTexture2D::~TestGeometryTexture2D()
	{
	}

	void TestGeometryTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestGeometryTexture2D::OnRender()
	{
		GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glDisable(GL_DEPTH_TEST);

		Renderer renderer;

		m_Texture->Bind();

		glm::mat4 model = glm::mat4(1.0f); // identity matrix, does nothing
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		m_View = glm::translate(glm::mat4(1.0f), m_TranslationA);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("model", model);
		m_Shader->SetUniformMat4f("view", m_View);
		m_Shader->SetUniformMat4f("projection", m_Proj);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
	}

	void TestGeometryTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat2("Translation", &m_TranslationA.x, 0.0f, static_cast<float>(width));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
