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
            body.linearDamping = 0.0f;
            body.linearForce = { 10.0f, 0.0f, 0.0f };
            body.restitution = 0.75f;
            body.friction = 0.5f;

            MeshComponent mesh{};
            mesh.shapeType = ShapeType::Circle;

            MaterialComponent material{};
            material.color = { 0.8f, 0.2f, 0.1f };

            CircleColliderComponent collider{};
            collider.radius = 1.0f;


            registry.emplace<TransformComponent>(entity, transform);
            registry.emplace<RigidbodyComponent>(entity, body);
            registry.emplace<MeshComponent>(entity, mesh);
            registry.emplace<MaterialComponent>(entity, material);
            registry.emplace<CircleColliderComponent>(entity, collider);
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
            body.linearDamping = 0.5f;
            body.linearForce = { 20.0f, 0.0f, 0.0f };
            body.restitution = 0.35f;
            body.friction = 2.5f;

            MeshComponent mesh{};
            mesh.shapeType = ShapeType::Circle;

            MaterialComponent material{};
            material.color = { 0.8f, 0.8f, 0.1f };
            CircleColliderComponent collider{};
            collider.radius = 1.0f;


            registry.emplace<TransformComponent>(entity, transform);
            registry.emplace<RigidbodyComponent>(entity, body);
            registry.emplace<MeshComponent>(entity, mesh);
            registry.emplace<MaterialComponent>(entity, material);
            registry.emplace<CircleColliderComponent>(entity, collider);
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

            CircleColliderComponent collider{};
            collider.radius = 1.0f;


            registry.emplace<TransformComponent>(entity, transform);
            registry.emplace<RigidbodyComponent>(entity, body);
            registry.emplace<MeshComponent>(entity, mesh);
            registry.emplace<MaterialComponent>(entity, material);
            registry.emplace<CircleColliderComponent>(entity, collider);
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

            CircleColliderComponent collider{};
            collider.radius = 1.0f;

            registry.emplace<TransformComponent>(entity, transform);
            registry.emplace<RigidbodyComponent>(entity, body);
            registry.emplace<MeshComponent>(entity, mesh);
            registry.emplace<MaterialComponent>(entity, material);
            registry.emplace<CircleColliderComponent>(entity, collider);
        }

    }

    void OnUpdate(entt::registry& registry, GameTime time)
    {
        const float dt = static_cast<float>(time.FixedDeltaTime());

        auto view = registry.view<TransformComponent, RigidbodyComponent, CircleColliderComponent>();

        for (auto [entity, transform, body, collider] : view.each())
        {
            if (body.type == PhysicsBodyType::Static)
                continue;

            if (body.type == PhysicsBodyType::Dynamic)
            {
                if (body.invMass <= 0.0f)
                    continue;

                UpdateDynamicBody(body, dt);
            }

            if (body.type == PhysicsBodyType::Kinematic)
            {
				UpdateKinematicBody(body, dt);
            }

			IntegratePosition(body, dt);
			SolveCircleFloorCollision(body, collider, dt);
			SyncTransform(transform, body);
        }
    }


    void UpdateDynamicBody(RigidbodyComponent& body, float dt)
    {
        body.velocity.x += body.linearImpulse.x * body.invMass;
        body.velocity.y += body.linearImpulse.y * body.invMass;

        body.linearImpulse = { 0.0f, 0.0f, 0.0f };

        body.linearForce.x += gravity.x * body.mass;
        body.linearForce.y += gravity.y * body.mass;

        body.velocity.x += body.linearForce.x * body.invMass * dt;
        body.velocity.y += body.linearForce.y * body.invMass * dt;

        body.linearForce = { 0.0f, 0.0f, 0.0f };

        body.velocity.x += body.acceleration.x * dt;
        body.velocity.y += body.acceleration.y * dt;

        const float dampingFactor = 1.0f / (1.0f + body.linearDamping * dt);

        body.velocity.x *= dampingFactor;
        body.velocity.y *= dampingFactor;
    }

    void UpdateKinematicBody(RigidbodyComponent& body, float dt)
    {
        body.velocity.x += body.acceleration.x * dt;
        body.velocity.y += body.acceleration.y * dt;
    }

    void IntegratePosition(RigidbodyComponent& body, float dt)
    {
        body.position.x += body.velocity.x * dt;
        body.position.y += body.velocity.y * dt;
    }

    void SolveCircleFloorCollision(RigidbodyComponent& body, const CircleColliderComponent& collider, float dt)
    {
        const float bottom = body.position.y - collider.radius;

        if (bottom < floorY)
        {
            body.position.y = floorY + collider.radius;

            if (body.velocity.y < 0.0f)
            {
                const float incomingVelocity = -body.velocity.y;

                if (incomingVelocity < minBounceVelocity)
                {
                    body.velocity.y = 0.0f;
                }
                else
                {
                    body.velocity.y = incomingVelocity * body.restitution;
                }

                if(body.type == PhysicsBodyType::Dynamic)
                {
					const float frictionFactor = 1.0f / (1.0f + body.friction * dt);
					body.velocity.x *= frictionFactor;
				}
            }
        }
    }

    void SyncTransform(TransformComponent& transform, const RigidbodyComponent& body)
    {
        transform.position.x = body.position.x;
        transform.position.y = body.position.y;
    }




private:
    DirectX::XMFLOAT3 gravity = { 0.0f, -9.8f, 0.0f };
    float floorY = -5.0f;
    float minBounceVelocity = 0.35f;
};


class MyGame
{
public:



    void Run()
    {
        // Initialize games
        gameTime = {};

        GameTime::Config config{};
        config.fixedDeltaTime = 1.0 / 60.0;
        config.maxDeltaTime = 0.25;
        config.maxPhysicsStepsPerFrame = 8;
        config.timeScale = 1.0;
        config.clearPhysicsAccumulatorOnReset = true;

        gameTime.SetConfig(config);
        gameTime.Reset();

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

            while (gameTime.UpdatePhysics())
            {
                physicsSystem.OnUpdate(registry, gameTime);
            }


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