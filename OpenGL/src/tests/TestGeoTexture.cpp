#include "TestGeoTexture.h"




#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test
{
	TestGeometryTexture2D::TestGeometryTexture2D()
		: m_Proj(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)

	{
		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

		int rectSize = 3;

		float positions[] = {
			// positions																	// Colors					// tex coords
		static_cast<float>(-width / rectSize),static_cast<float>(-height / rectSize),0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		static_cast<float>(width / rectSize), static_cast<float>(-height / rectSize), 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		static_cast<float>(width / rectSize), static_cast<float>(height / rectSize), 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		static_cast<float>(-width / rectSize), static_cast<float>(height / rectSize), 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f
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
		m_Texture = std::make_unique<Texture>("res/textures/container.jpg");
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
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

	}

	void TestGeometryTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat2("Translation", &m_TranslationA.x, 0.0f, static_cast<float>(width));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}