#include "movingTarget.h"
#include <Windows.h>
#include "utilitymacro.h"
#include <time.h>
#include <stdlib.h>

MovingTarget::MovingTarget(int maxWidth, int maxHeight, float detectRadius, float maxSpeed)
{
	setMaxWidth(maxWidth);
	setMaxHeight(maxHeight);
	setDetectRadius(detectRadius);
	setMaxSpeed(maxSpeed);
	setVelocity(Vec2<float>(0, 0));
	totalSec = 0;
}

MovingTarget::~MovingTarget(void) { }

void MovingTarget::setMaxWidth(int maxWidth) { this->maxWidth = maxWidth; }
void MovingTarget::setMaxHeight(int maxHeight) { this->maxHeight = maxHeight; }
void MovingTarget::setDetectRadius(float detectRadius) { this->detectRadius = detectRadius; }

void MovingTarget::draw(void *a_hdc) {
	Entity::draw(a_hdc);
	HDC hdc = (HDC)a_hdc;
	HGDIOBJ oldPen = SelectObject(hdc, GetStockObject(DC_PEN));
	COLORREF oldDCPenColor = SetDCPenColor(hdc, DARKEN(color) );

	SetDCPenColor(hdc, oldDCPenColor);
	SelectObject(hdc, oldPen);
}


void MovingTarget::wander(MOB *badGuy, float seconds) {

	totalSec += seconds;
	
	if(totalSec >= 0.01) {
	
		Vec2<float> *p = getPosition();

		Vec2<float> badGuyPos = *badGuy->getPosition();
		Vec2<float> myPos = *p;

		Vec2<float> badGuyVelocity = badGuy->getVelocity();
		Vec2<float> myVelocity = getVelocity();

		float distance = Vec2<float>(badGuyPos - myPos).magnitude();
		Vec2<float> dv = badGuyVelocity - myVelocity;

		const float maxAccel = 25, accel = maxAccel * totalSec;

		int dir = rand() % 4;
		bool left = rand() % 2 != 0;
		bool right = rand() % 2 != 0;
		bool up = rand() % 2 != 0;
		bool down = rand() % 2 != 0;

		if(distance <= detectRadius ) {
			setMaxSpeed(40);
			
			float theta = 90;
			float cs = (float)cos( 2 * PI / 180.0 * theta );
			float sn = (float)sin( 2 * PI / 180.0 * theta );
			float px = badGuyPos.x * cs - badGuyPos.y * sn;
			float py = badGuyPos.x * sn + badGuyPos.y * cs;
			addVelocity(Vec2<float>(p->x + px, p->y + py));


		} else {
			setMaxSpeed(25);
		}

		float R =  area.radius;

		if(p->x - R * 2 <= 0) {
			right = true;
			left = false;

			MovingTarget wall(maxWidth, maxHeight);
			wall.setPosition(Vec2<float>(0, p->y));			
			Vec2<float> r = MovingTarget::getReflect(this, &wall);

			setVelocity(Vec2<float>());
			setVelocity(r);
		}
		else if(p->x + R * 2 >= maxWidth) {
			left = true;
			right = false;

			MovingTarget wall(maxWidth, maxHeight);
			wall.setPosition(Vec2<float>((float)maxWidth, p->y));
			Vec2<float> r = MovingTarget::getReflect(this, &wall);

			setVelocity(Vec2<float>());
			setVelocity(r);
		}
		// Y axis boundary colision detection
		if(p->y - R * 2 <= 0)  {
			down = true;
			up = false;

			MovingTarget wall(maxWidth, maxHeight);
			wall.setPosition(Vec2<float>(p->x, 0));
			Vec2<float> r = MovingTarget::getReflect(this, &wall);

			setVelocity(Vec2<float>());
			setVelocity(r);
		}
		else if (p->y + R * 2 >= maxHeight ) {
			up = true;
			down = false;
			
			MovingTarget wall(maxWidth, maxHeight);
			wall.setPosition(Vec2<float>(p->x, (float)maxHeight));			
			Vec2<float> r = MovingTarget::getReflect(this, &wall);

			setVelocity(Vec2<float>());
			setVelocity(r);
		}



		if(dir >= 0 && dir <= 3) {
			Vec2<float> direction;
			if(left) { direction += Vec2<float>(-maxAccel,0); }
			if(up)   { direction += Vec2<float>(0,-maxAccel); }
			if(right){ direction += Vec2<float>(maxAccel,0);  }
			if(down) { direction += Vec2<float>(0,maxAccel);  }
			direction.limitMagnitude(accel);
			addVelocity(direction);
		} 
		
		update(totalSec);
		totalSec = 0;
	}

}

