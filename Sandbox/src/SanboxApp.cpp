#include "Emberpch.h"
#include "Ember.h"

class ExampleLayer : public Ember::Layer
{
	public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		EMBER_INFO("ExampleLayer::Update");
	}

	void OnEvent(Ember::Event& event) override
	{
		EMBER_TRACE("{0}", event);
	}
};

class Sandbox : public Ember::Application
{
	public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Ember::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Ember::Application* Ember::CreateApplication()
{
	return new Sandbox();
}