#pragma once
#include "mob.h"

class MovingTarget :
	public MOB
{
	// Box bounds
	int maxWidth, maxHeight;
	// Detect enemy radius
	float detectRadius;
	// Sec elapsed after update
	float totalSec;
public:
	MovingTarget(int maxWidth, int maxHeight, float detectRadius = 100, float maxSpeed = 20);
	void setMaxWidth(int maxWidth);
	void setMaxHeight(int maxHeight);
	void setDetectRadius(float detectRadius);
	
	void draw(void *hdc);
	~MovingTarget(void);
	void wander(MOB *badGuy, float seconds);
	static Vec2<float> getReflect(MOB *e1, MOB *e2)
	{
		Vec2<float> *p1 = e1->getPosition();
		Vec2<float> *p2 = e2->getPosition();

		float theta = 90;
		float cs = (float)cos( 2 * PI / 180.0 * theta );
		float sn = (float)sin( 2 * PI / 180.0 * theta );
		float px = p2->x * cs - p2->y * sn;
		float py = p2->x * sn + p2->y * cs;

		return Vec2<float>(p1->x + px, p1->y + py);
	}
};

	