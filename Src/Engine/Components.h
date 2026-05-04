#pragma once
#include "GameMath.h"
#include <string>



struct TransformComponent
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 scale;
};

struct MaterialComponent
{
    DirectX::XMFLOAT3 color = { 0.8f, 0.8f, 0.8f };
    int32_t textureId = 0;
};

enum class ShapeType
{
	Circle,
    Null,
    Count
};

enum class PhysicsBodyType
{
    Static,
    Dynamic,
    Kinematic
};


struct MeshComponent
{
    ShapeType shapeType;
};


struct RigidbodyComponent
{
    PhysicsBodyType type = PhysicsBodyType::Static;

    DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
};