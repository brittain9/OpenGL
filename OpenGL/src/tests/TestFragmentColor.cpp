#include "TestFragmentColor.h"

#include "Renderer.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"


namespace test
{
	TestFragmentColor::TestFragmentColor()
		: m_Color{ 0.2f, 0.3f, 0.8f, 1.0f },
		m_Proj(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0)
	{
		glm::vec3 translationA(200, 200, 0);

		int rectSize = 5;

		float positions[] = {
			// positions, x , y , z	= 0.0f														
			 static_cast<float>(-width / rectSize),static_cast<float>(-height / rectSize), 0.0f,
			 static_cast<float>(width / rectSize), static_cast<float>(-height / rectSize), 0.0f,
			 static_cast<float>(width / rectSize), static_cast<float>(height / rectSize),  0.0f,
			 static_cast<float>(-width / rectSize),static_cast<float>(height / rectSize),  0.0f,
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0 
		};

		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();

		m_VBO = std::make_unique<VertexBuffer>(positions, sizeof(positions));

		VertexBufferLayout layout;
		layout.Push<float>(3);

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
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glDisable(GL_DEPTH_TEST);


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
		ImGui::SliderFloat("Translation X", &m_TranslationA.x, 0.0f, static_cast<float>(width));
		ImGui::SliderFloat("Translation Y", &m_TranslationA.y, 0.0f, static_cast<float>(height));

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
