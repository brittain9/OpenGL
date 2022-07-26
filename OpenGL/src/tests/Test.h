#pragma once

#include <functional>
#include <iostream>
#include <vector>

static int width{ 800 };
static int height{ 600 };

namespace test
{
	class Test
	{
	public:
		Test(){}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu :public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template <typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T(); })); // lamda
		}
	private:
		// Test menu can write to Test* and change current test
		Test*& m_currentTest;
		// Vector of pairs with string and function returning a test* so we can construct and destruct as needed
		std::vector < std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}

