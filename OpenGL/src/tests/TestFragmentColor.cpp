#include "TestFragmentColor.h"

#include "Renderer.h"


#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"


namespace test
{
	TestFragmentColor::TestFragmentColor()
		: m_Color{ 0.2f, 0.3f, 0.8f, 1.0f },
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0)
	{
		glm::vec3 translationA(200, 200, 0);

		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,   
			 50.0f,  50.f,  1.0f, 1.0f,   
			-50.0f,  50.0f, 0.0f, 1.0f    
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0 
		};

		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();

		m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VBO, layout);
		m_IBO = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/BasicVertex.shader", "res/shaders/FragmentColor.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.5f, 1.0f);
	}

	TestFragmentColor::~TestFragmentColor()
	{
	}

	void TestFragmentColor::OnUpdate(float deltaTime)
	{
	}

	void TestFragmentColor::OnRender()
	{
		GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
		glm::mat4 mvp = m_Proj * m_View * model;

		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		
	}

	void TestFragmentColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Fragment Color", m_Color);
		ImGui::SliderFloat3("Translation", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
