#pragma once
#include "mob.h"
class Hunter :
	public MOB
{
	float totalSec;
	bool active;
public:
	Hunter(float maxSpeed = 25);
	~Hunter(void);
	void draw(void* hdc);
	void follow(MOB *target, float seconds);
	void enable();
	void disable();
	bool isActive();
};

