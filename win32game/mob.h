#pragma once

#include "entity.h"
#include "Projectile.h"
#include <Windows.h>
#include "utilitymacro.h"

/** [Mob]ile Entity */
class MOB : public Entity
{
	Vec2<float> velocity; // how much to move per second
	float maxSpeed;   // what to limit speed to
public:
	MOB() {}
	virtual Vec2<float> getVelocity() { return velocity; }
	virtual void setVelocity(Vec2<float> v)
	{
		velocity = v;
		velocity.limitMagnitude(maxSpeed);
	}
	virtual void addVelocity(Vec2<float> v)
	{
		velocity += v;
		velocity.limitMagnitude(maxSpeed);
	}
	void update(float secondsPassed)
	{
		this->area.center += (velocity * secondsPassed);
	}
	void draw(void* hdc);
	void applyStoppingForce(float force);
	void setMaxSpeed(float maxSpeed) { this->maxSpeed = maxSpeed; }
	float getMaxSpeed() { return maxSpeed; }

	float getR() const { return area.radius; }

	Projectile attack() {
		Projectile proj;
		proj.init(*getPosition(), 7, RGB(32, 32, 255));
		Vec2<float> v = velocity / velocity.magnitude();
		if (velocity.magnitude() == 0)
			proj.setVelocity(Vec2<float>(0, -170));
		else
			proj.setVelocity(v * 170);
		proj.setOwner(this);

		return proj;
	}
};
