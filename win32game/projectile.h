#pragma once
#include "gameobject.h"
#include "circle.h"
#include "entity.h"

class Projectile :
	public Entity
{
protected:
	Vec2<float> velocity; // how much to move per second
	float maxSpeed;   // what to limit speed to

	Entity *owner;
public:
	Projectile();
	Projectile(Vec2<float> center, float radius, int color);
	Projectile(const Projectile &that);
	~Projectile(void);

	Vec2<float> getVelocity(){ return velocity; }
	void setVelocity(Vec2<float> v)
	{
		velocity = v;
		velocity.limitMagnitude(maxSpeed);
	}
	void addVelocity(Vec2<float> v)
	{
		velocity += v;
		velocity.limitMagnitude(maxSpeed);
	}
	void update(float secondsPassed)
	{
		this->area.center += (velocity * secondsPassed);
	}

	void draw(void* hdc);
	void setMaxSpeed(float maxSpeed){ this->maxSpeed = maxSpeed; }
	float getMaxSpeed(){ return maxSpeed; }

	float getR() const { return area.radius; };


	void init(Vec2<float> position, float radius, int color);

	Entity *getOwner()  { return owner; }
	void setOwner(Entity * owner) { this->owner = owner; }
	
};

