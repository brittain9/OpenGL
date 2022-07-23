#pragma once
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "glm/vec3.hpp"
#include <memory>

namespace test
{
	class TestFragmentColor : public Test
	{
	public:
		TestFragmentColor();
		~TestFragmentColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_Color[4];

		std::unique_ptr <VertexArray> m_VAO;
		std::unique_ptr <VertexBuffer> m_VBO;
		std::unique_ptr <IndexBuffer> m_IBO;
		std::unique_ptr <Shader> m_Shader;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA;
	};
}
