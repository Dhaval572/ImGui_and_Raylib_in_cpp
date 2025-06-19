#include "Particle.h"

Particle::Particle(Vector2 pos, Vector2 vel, Vector2 acc, float speed, float angle, float size, float lifeTime, Color color)
	: position(pos),
	  initPos(pos),
	  velocity(vel),
	  acceleration(acc),
	  speed(speed),
	  angle(angle),
	  size(size),
	  color(color),
	  lifeTime(lifeTime),
	  age(0.0f)
{
}

bool Particle::IsAlive() const
{
	return age < lifeTime;
}

void Particle::Update(float dt)
{
	age += dt;
	velocity.x += acceleration.x * dt;
	velocity.y += acceleration.y * dt;

	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
}

void Particle::Draw() const
{
	float alpha = 1.0f - (age / lifeTime);
	Color fadedColor = color;
	fadedColor.a = static_cast<unsigned char>(alpha * 255.0f);

	DrawCircleV(position, size, fadedColor);
}

void Particle::Reset()
{
	age = 0.0f;
	position = initPos; 
	velocity.x = speed * cos(angle);
	velocity.y = speed * sin(angle);
}
