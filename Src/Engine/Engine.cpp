// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GameWindows.h"
#include "GameTime.h"
#include "RenderSystem.h"
#include "entt.hpp"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class PhysicsSystem
{
public:

    void OnInitialize(entt::registry& registry)
    {
        {
            auto entity = registry.create();

            TransformComponent transform{};
            transform.position = { -0.8f, 0.6f, 0.0f };
            transform.scale = { 1.0f, 1.0f };

            RigidbodyComponent body{};
            body.type = PhysicsBodyType::Dynamic;
            body.position = transform.position;
            body.acceleration = { 0.0f, 0.0f, 0.0f };
            body.mass = 1.0f;
            body.invMass = 1.0f / body.mass;
            body.velocity = { 0.0f, 0.0f, 0.0f };
            body.linearImpulse = { 0.35f, 0.0f, 0.0f };

            MeshComponent mesh{};
            mesh.shapeType = ShapeType::Circle;

            MaterialComponent material{};
            material.color = { 0.8f, 0.2f, 0.1f };

            registry.emplace<TransformComponent>(entity, transform);
            registry.emplace<RigidbodyComponent>(entity, body);
            registry.emplace<MeshComponent>(entity, mesh);
            registry.emplace<MaterialComponent>(entity, material);
        }

        {
            auto entity = registry.create();

            TransformComponent transform{};
            transform.position = { -0.8f, 0.2f, 0.0f };
            transform.scale = { 1.0f, 1.0f };

            RigidbodyComponent body{};
            body.type = PhysicsBodyType::Dynamic;
            body.position = transform.position;
            body.acceleration = { 0.0f, 0.0f, 0.0f };
            body.mass = 4.0f;
            body.invMass = 1.0f / body.mass;
            body.velocity = { 0.0f, 0.0f, 0.0f };
            body.linearImpulse = { 0.35f, 0.0f, 0.0f };

            MeshComponent mesh{};
            mesh.shapeType = ShapeType::Circle;

            MaterialComponent material{};
            material.color = { 0.8f, 0.8f, 0.1f };

            registry.emplace<TransformComponent>(entity, transform);
            registry.emplace<RigidbodyComponent>(entity, body);
            registry.emplace<MeshComponent>(entity, mesh);
            registry.emplace<MaterialComponent>(entity, material);
        }


        {
            auto entity = registry.create();

            TransformComponent transform{};
            transform.position = { 0.0f, -0.2f, 0.0f };
            transform.scale = { 1.0f, 1.0f };

            RigidbodyComponent body{};
            body.type = PhysicsBodyType::Static;
            body.position = transform.position;
            //body.velocity = { 0.0f, 0.0f, 0.0f };
            //body.acceleration = { 0.0f, 0.0f, 0.0f };
            body.mass = 0.0f;
            body.invMass = 0.0f;

            MeshComponent mesh{};
            mesh.shapeType = ShapeType::Circle;

            MaterialComponent material{};
            material.color = { 0.3f, 0.8f, 0.3f };

            registry.emplace<TransformComponent>(entity, transform);
            registry.emplace<RigidbodyComponent>(entity, body);
            registry.emplace<MeshComponent>(entity, mesh);
            registry.emplace<MaterialComponent>(entity, material);
        }


        {
            auto entity = registry.create();

            TransformComponent transform{};
            transform.position = { -0.8f, -0.6f, 0.0f };
            transform.scale = { 1.0f, 1.0f };

            RigidbodyComponent body{};
            body.type = PhysicsBodyType::Kinematic;
            body.position = transform.position;
            body.velocity = { 0.25f, 0.0f, 0.0f };
            body.acceleration = { 0.0f, 0.0f, 0.0f };
            body.mass = 0.0f;
            body.invMass = 0.0f;

            MeshComponent mesh{};
            mesh.shapeType = ShapeType::Circle;

            MaterialComponent material{};
            material.color = { 0.3f, 0.2f, 0.8f };

            registry.emplace<TransformComponent>(entity, transform);
            registry.emplace<RigidbodyComponent>(entity, body);
            registry.emplace<MeshComponent>(entity, mesh);
            registry.emplace<MaterialComponent>(entity, material);
        }

    }

    void OnUpdate(entt::registry& registry, GameTime time)
    {
        const float dt = static_cast<float>(time.GetDeltaTime());

        auto view = registry.view<TransformComponent, RigidbodyComponent>();

        for (auto [entity, transform, body] : view.each())
        {
            if (body.type == PhysicsBodyType::Static)
                continue;

            if (body.type == PhysicsBodyType::Dynamic)
            {
                if (body.invMass <= 0.0f)
                    continue;

                body.velocity.x += body.linearImpulse.x * body.invMass;
                body.velocity.y += body.linearImpulse.y * body.invMass;

                body.linearImpulse = { 0.0f, 0.0f, 0.0f };

                body.velocity.x += body.acceleration.x * dt;
                body.velocity.y += body.acceleration.y * dt;

                body.velocity.x += gravity.x * dt;
                body.velocity.y += gravity.y * dt;
            }

            if (body.type == PhysicsBodyType::Kinematic)
            {
                body.velocity.x += body.acceleration.x * dt;
                body.velocity.y += body.acceleration.y * dt;
            }

            body.position.x += body.velocity.x * dt;
            body.position.y += body.velocity.y * dt;

            transform.position.x = body.position.x;
            transform.position.y = body.position.y;
        }
    }

    void ApplyLinearImpulse(RigidbodyComponent& body, const DirectX::XMFLOAT3& impulse)
    {
        if (body.type != PhysicsBodyType::Dynamic)
            return;

        if (body.invMass <= 0.0f)
            return;

        body.velocity.x += impulse.x * body.invMass;
        body.velocity.y += impulse.y * body.invMass;
    }


private:
    DirectX::XMFLOAT3 gravity = { 0.0f, -0.5f, 0.0f };

};


class MyGame
{
public:



    void Run()
    {
        // Initialize games
        gameTime = {};
        gameTime.OnInitialize();

        gameWindow = {};
        gameWindow.OnInitialize();



        Scene1();

		physicsSystem = {};
		physicsSystem.OnInitialize(registry);

        renderSystem = {};
        renderSystem.OnInitialize(registry, gameWindow.GetHandle(), gameWindow.GetClientWidth(), gameWindow.GetClientHeight());


        // Main loop
        Update();
    }



    void Scene1()
    {

    }

    void Update()
    {
        while (gameWindow.IsRunning())
        {
            gameWindow.PumpMessages();
            gameTime.OnUpdate();

			physicsSystem.OnUpdate(registry, gameTime);
            renderSystem.OnUpdate(registry, gameTime);
        }
    }

    GameWindows gameWindow;
    GameTime gameTime;

    RenderSystem renderSystem;
	PhysicsSystem physicsSystem;

    entt::registry registry;
};



int main()
{
    MyGame myGame;
    myGame.Run();
    return 0;
}