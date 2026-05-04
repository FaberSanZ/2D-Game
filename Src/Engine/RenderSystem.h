#include <windows.h>
#include <fstream>
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "GameWindows.h"
#include "GameTime.h"
#include "entt.hpp"
#include "Components.h"
#include "RenderingDevice.h"


class RenderSystem
{
private:

public:
    RenderSystem()
    {
    }

    uint32_t m_Width{ }; // Width of the render target
    uint32_t m_Height{ }; // Height of the render target
    Render render{};
	Mesh circle = {};

    void OnInitialize(entt::registry& registry, HWND hwnd,uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;

        render.Initialize(hwnd, m_Width, m_Height);


		circle = GenerateCirclenMesh(1.0f, 26);
    }

    Mesh GenerateCirclenMesh(float radius, uint32_t segmentCount)
    {
        std::vector<Vertex> vertices;
        vertices.reserve(segmentCount + 2);

        vertices.push_back({ 0.0f, 0.0f, 0.0f, 1.0f });

        for (uint32_t i = 0; i <= segmentCount; ++i)
        {
            float angle = (2.0f * 3.14159265f * i) / segmentCount;
            float x = radius * cos(angle);
            float y = radius * sin(angle);

            vertices.push_back({ x, y, 0.0f, 1.0f });
        }

        std::vector<uint32_t> indices;
        indices.reserve(segmentCount * 3);

        for (uint32_t i = 1; i <= segmentCount; ++i)
        {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i);
        }

        return render.CreateMesh(vertices.data(), (uint32_t)vertices.size(), indices.data(), (uint32_t)indices.size());
    }



	float speed = 1.0f;

    void OnUpdate(entt::registry& registry, GameTime time)
    {
		Update(registry, time);
        Loop(registry);
    }

    void Loop(entt::registry& registry)
    {
		render.Reset(); // Reset the command allocator and command list for the current frame
		render.Clear(); // Clear the render target and depth/stencil buffer, and set them for rendering
		render.BeginFrame(); // Set the viewport, scissor rect, and pipeline state for the current frame


		circle.Draw(render.commandList); // Draw the mesh using the command list

        render.Loop();
    }


    void Update(entt::registry& registry, GameTime time)
    {
        std::vector<InstanceData> circleInstancing;

        DirectX::XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f };

        auto view_mesh = registry.view<MeshComponent>();

        for (auto [entity, mesh] : view_mesh.each())
        {

            if(registry.all_of<MaterialComponent>(entity))
            {
                auto& mateial = registry.get<MaterialComponent>(entity);
                color = { mateial.color.x, mateial.color.y, mateial.color.z, 1.0f };
			}


            if (registry.all_of<TransformComponent>(entity))
            {
                auto& trasform = registry.get<TransformComponent>(entity);

                if (mesh.shapeType == ShapeType::Circle)
                {
                    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(trasform.position.x, trasform.position.y, 0.0f);
                    DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
                    DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(trasform.scale.x, trasform.scale.y, 1.0f);
                    circleInstancing.push_back({ DirectX::XMMatrixTranspose(scale * rotation * translation), color });
                }
            }

        }


        if (circleInstancing.size() > 0)
            render.UpdateInstanceBuffer(circle, circleInstancing.data(), circleInstancing.size());

    }

    void OnShutdown()
    {
        render.Cleanup();
    }

};


