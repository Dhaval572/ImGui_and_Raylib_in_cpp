#pragma once
#include <raylib.h>
#include <raymath.h>

class Particle
{
public:
    Vector2 position;
    Vector2 initPos;
    Vector2 velocity;
    Vector2 acceleration;
    float lifeTime;
    float angle;
    float age;
    float size;
    Color color;
    float speed;

    Particle(Vector2 pos, Vector2 vel, Vector2 acc, float speed, float angle, float size, float lifeTime, Color color);

    bool IsAlive() const;
    void Update(float dt);
    void Draw() const;
    void Reset();
};
