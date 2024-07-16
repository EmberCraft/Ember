#include "Emberpch.h"
#include "Ember/Scene/Scene.h"
#include "Ember/Scene/Components.h"
#include "Ember/Scene/Entity.h"

#include "Ember/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Ember
{

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>();
		auto& tag = entity.GetComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<CameraComponent, TransformComponent>();
			for(auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if(camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}

		}

		if(mainCamera)
		{
			// Render 2D

			Renderer2D::BeginScene(mainCamera->GetProjectionMatrix(), *cameraTransform);
			
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for(auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();

		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize camera's with non-FixedAspectRatio
		auto view = m_Registry.view<CameraComponent>();
		for(auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if(!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

}