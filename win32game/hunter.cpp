#include "Hunter.h"
#include <Windows.h>
#include "utilitymacro.h"
#include <time.h>
#include <stdlib.h>

Hunter::Hunter(float maxSpeed)
{
	setMaxSpeed(maxSpeed);
	active = true;
	area.radius = 5;
	totalSec = 0;	
}

Hunter::~Hunter(void) { }

void Hunter::draw(void* hdc) {
	if (active == false) return;
	MOB::draw(hdc);
}

void Hunter::enable() { 
	active = true; 
	setColor(RGB(255, 32, 32));
}

void Hunter::disable() { 
	active = false; 
	setColor(RGB(64, 64, 64));
}
bool Hunter::isActive() { return active; }

void Hunter::follow(MOB *target, float seconds) {
	if (active == false) return;
	totalSec += seconds;
	
	if(totalSec >= 0.04) {
	
		Vec2<float> targetPos = *target->getPosition();
		Vec2<float> myPos = *getPosition();
		Vec2<float> velocity = targetPos - myPos;
		// Set new velocity
		setVelocity(velocity);

		update(totalSec);
		totalSec = 0;
	}
}
