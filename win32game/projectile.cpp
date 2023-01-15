#include "Projectile.h"
#include <Windows.h>
#include "utilitymacro.h"
#include "entity.h"


Projectile::Projectile() {
	owner = 0;
	maxSpeed = 150;
}
Projectile::Projectile(const Projectile &that) {
	this->area.center = that.area.center;
	this->area.radius = that.area.radius;
	this->color = that.color;
	this->owner = that.owner;
	this->maxSpeed = that.maxSpeed;
	this->velocity = that.velocity;
}

Projectile::Projectile(Vec2<float> center, float radius, int color) {
	area.center = center;
	area.radius = radius;
	this->color = color;
	owner = 0;
	maxSpeed = 150;
}
Projectile::~Projectile(void) {}

void Projectile::init(Vec2<float> center, float radius, int color) {
	area.center = center;
	area.radius = radius;
	this->color = color;
	maxSpeed = 150;
}

void Projectile::draw(void* a_hdc) {
	HDC hdc = (HDC)a_hdc;
	// remember what old draw settings were
	HGDIOBJ oldPen = SelectObject(hdc, GetStockObject(DC_PEN));
	COLORREF oldDCPenColor = SetDCPenColor(hdc, DARKEN(color) );
	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
	COLORREF oldDCBrushColor = SetDCBrushColor(hdc, color);
	// draw with new settings
	area.draw(a_hdc);
	// revert to old settings
	SetDCBrushColor(hdc, oldDCBrushColor);
	SelectObject(hdc, oldBrush);
	SetDCPenColor(hdc, oldDCPenColor);
	SelectObject(hdc, oldPen);
}
