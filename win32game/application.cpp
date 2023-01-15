#include "application.h"
#include <stdio.h> // printf (debug messages, thanks createWin32Console()!)
#include <Windows.h>

#define GOAL_RADIUS 10

void Application::init(void* hwnd)
{
	printf("init\n");
	if(state == Application::STATE_INIT)
	{
		this->hwnd = hwnd;
		// setup the game entities
		RECT r;
		GetClientRect((HWND)hwnd, &r);
		height = r.bottom-r.top;
		width = r.right-r.left;
		player.init(Vec2<float>(width/2.0f, height/2.0f), 20, RGB(32,255,32));
		player.setMaxSpeed(100);


		const int goalsNumber = 10;
		for (int i = 0; i < goalsNumber; ++i) {
			MovingTarget goal(width, height, 100, 20);
			Vec2<float> pos = Vec2<float>::random
			(
				Vec2<float>((float)GOAL_RADIUS, (float)GOAL_RADIUS),
				Vec2<float>((float)(width), (float)(height))
			);
			goal.init(pos, (float)GOAL_RADIUS, RGB(255, 192, 32));
			goals.push_back(goal);
		}

		Vec2<float> pos = Vec2<float>::random
		(
			Vec2<float>(15, 15),
			Vec2<float>((float)(width), (float)(height))
		);
		hunter.init(pos, 15, RGB(255, 32, 32));
		hunter.setVelocity(Vec2<float>());

		mouseClickEntity.init(Vec2<float>(0,0), 2, RGB(255,255,255));
		outputMessage= "Use the Arrow keys to move, and collect Goal Objects! \n Press [SPACE] for attack \n F1 to enable/disable Player AI";

		// finished initialization!
		state = Application::STATE_RUNNING;
	}
}

void Application::release()
{
	printf("release\n");
	if(state != STATE_RELEASED)
	{
		state = STATE_RELEASED;
	}
}

void Application::resize(int a_width, int a_height)
{
	width = a_width;
	height = a_height;
}

void Application::draw(void* a_hdc)
{
	HDC hdc = (HDC)a_hdc;

	// dark background
	RECT r;
	GetClientRect((HWND)hwnd, &r);
	FillRect(hdc, &r, HBRUSH(GetStockObject(DKGRAY_BRUSH)));

	// draw the game
	for (size_t i = 0; i < goals.size(); ++i)
		goals[i].draw(hdc);

	for (size_t i = 0; i < projectiles.size(); ++i)
		projectiles[i].draw(hdc);

	hunter.draw(hdc);
	player.draw(hdc);
	mouseClickEntity.draw(hdc);

	// draw the in-game message
	SetBkMode(hdc,TRANSPARENT);
	SetTextColor(hdc, player.getColor());
	DrawText(hdc, outputMessage, -1, &r, DT_CENTER );
}

void applyMove(MOB * e, float seconds, bool left, bool up, bool right, bool down)
{
	const float maxAccel = 100, accel = maxAccel * seconds;
	if(left || up || right || down) {
		Vec2<float> direction;
		if(left) { direction += Vec2<float>(-maxAccel,0); }
		if(up)   { direction += Vec2<float>(0,-maxAccel); }
		if(right){ direction += Vec2<float>(maxAccel,0);  }
		if(down) { direction += Vec2<float>(0,maxAccel);  }
		direction.limitMagnitude(accel);
		e->addVelocity(direction);
	} else {
		e->applyStoppingForce(accel);
	}
}

void Application::setUserInput(int input)
{
	switch(input){
	case VK_SPACE:
	{
		Projectile proj = player.attack();
		projectiles.push_back(proj);
	}
	break;
	case VK_F1:
	{
		AiPlayer = !AiPlayer;
		if (AiPlayer) {
			printf("Ai Player Enabled");
			applyMove(&player, 2, 0, 0, 0, 0);
		}
		else {
			printf("Ai Player Enabled");
		}

		break;
	}
	case VK_ESCAPE:
		state = Application::STATE_QUIT;
		DestroyWindow((HWND)hwnd);
		return;
	}
}

void Application::setMouseState(int x, int y, int mouseButtonState)
{
	mouseLoc.init(x, y);
	mouseButtonsPressed = mouseButtonState;
}

void Application::setMousePress(int x, int y, int mouseButtonState, int msg)
{
	if (state == Application::STATE_RELEASED) return;
	mouseButtonsPressed = mouseButtonState;
	
	if(isLeftMousePressed()) {
		mouseClickEntity.setPosition(Vec2<float>((float)x, (float)y));
		// Goal position from mouse coords
		Vec2<float> goalPosition((float)x, (float)y);
		// Create new goal
		MovingTarget goal(width, height);
		// Check if new goal is not collide with goals on game board
		for (size_t i = 0; i < goals.size(); ++i)
			if (goal.collidesWith(&goals[i])) return;
		// New objet has no colision with other goals so we can append to the goals list
		// Init goal position and add to vector of goals
		goal.init(goalPosition, GOAL_RADIUS, RGB(255, 192, 32));
		goals.push_back(goal);
	}
}

bool Application::isLeftMousePressed()
{
	return (mouseButtonsPressed & MK_LBUTTON) != 0;
}



void Application::update(float secondsPassed)
{
	if(state == Application::STATE_RELEASED) return;
	// another way to test for key presses
	unsigned short PRESSED = (1 << 15);
	if( (GetAsyncKeyState( VK_ESCAPE ) & PRESSED) != 0){
		state = Application::STATE_QUIT;
		DestroyWindow((HWND)hwnd);
	}
	if (AiPlayer == false) {
		// move direction of player according to key presses
		applyMove(&player, secondsPassed,
			(GetAsyncKeyState(VK_LEFT) & PRESSED) != 0,
			(GetAsyncKeyState(VK_UP) & PRESSED) != 0,
			(GetAsyncKeyState(VK_RIGHT) & PRESSED) != 0,
			(GetAsyncKeyState(VK_DOWN) & PRESSED) != 0);
	}
	else { // Player entity controls by Ai
		int index = 0;
		MovingTarget goal = goals[index];
		Vec2<float> *player_pos = player.getPosition();
		float initial_distance = Vec2<float>(*player.getPosition() - *goal.getPosition()).magnitude();
		// Move to the closest goal
		for (size_t i = 0; i < goals.size(); ++i) {
			float distance = Vec2<float>(*player_pos - *goals[i].getPosition()).magnitude();
			if (distance < initial_distance) {
				index = i;
				initial_distance = distance;
			}
		}
		// Target closest goal
		goal = goals[index];
		// Recalculate player velocity depends on closest goal position
		Vec2<float> *goal_pos = goal.getPosition();
		Vec2<float> velocity = *goal_pos - *player_pos;
		// Set new velocity
		player.setVelocity(velocity);
		player.addVelocity(goal.getVelocity());

		if (hunter.isActive()) {
			Vec2<float> *hpos = hunter.getPosition();
			Vec2<float> hvel = hunter.getVelocity();

			Vec2<float> *ppos = player.getPosition();
			Vec2<float> pvel = player.getVelocity();

			float hmag = hvel.magnitude();
			float pmag = pvel.magnitude();

			float dist = (*hpos - *ppos).magnitude();
			float dr = 2 * player.getR() + 2 * hunter.getR();
			// DANGER player colide with hunter - kill hunter
			if (dist < dr) {
				player.setVelocity(hunter.getVelocity() * -1);
				Projectile proj = player.attack();
				projectiles.push_back(proj);
			}
		}
	}
	// update the game
	// Random wander
	for (size_t i = 0; i < goals.size(); ++i) goals[i].wander(&player, secondsPassed);
	// Hunter follows player
	if(hunter.isActive()) hunter.follow(&player, secondsPassed);
	// Projectiles fly
	for (size_t i = 0; i < projectiles.size(); ++i) projectiles[i].update(secondsPassed);

	/////////////// COLISIONS ////////////////
	// Goals colision detect
	for (size_t i = 0; i < goals.size(); ++i) {
		MovingTarget *e1 = &goals[i];
		for (size_t j = i + 1; j < goals.size(); ++j) {
			MovingTarget *e2 = &goals[j];
			if (e1->collidesWith(e2)) {

				Vec2<float> r1 = MovingTarget::getReflect(e1, e2);
				Vec2<float> r2 = MovingTarget::getReflect(e2, e1);

				e1->setVelocity(Vec2<float>());
				e2->setVelocity(Vec2<float>());

				e1->addVelocity(r1);
				e2->addVelocity(r2);
			}
		}


			if (e1->collidesWith(&hunter)) {
				Vec2<float> r1 = MovingTarget::getReflect(e1, &hunter);
				Vec2<float> r2 = MovingTarget::getReflect(&hunter, e1);

				e1->setVelocity(Vec2<float>());
				e1->addVelocity(r1);
			}


	}

	// Player box colision detect
	Vec2<float> *pos = player.getPosition();
	if (pos->x + player.getR() > width)
		player.setPosition(Vec2<float>(width - player.getR(), pos->y));
	else if (pos->x - player.getR() <= 0)
		player.setPosition(Vec2<float>(player.getR(), pos->y));

	if (pos->y + player.getR() > height)
		player.setPosition(Vec2<float>(pos->x, height - player.getR()));
	else if (pos->y - player.getR() <= 0)
		player.setPosition(Vec2<float>(pos->x, player.getR()));


	// Projectives
	for (size_t i = 0; i < projectiles.size(); ++i) {
		Projectile *proj = &projectiles[i];
		Vec2<float> *pos = proj->getPosition();
		// Projectile out of the box
		if (pos->x + proj->getR() > width || pos->x - proj->getR() <= 0
			|| pos->y + proj->getR() > height || pos->y - proj->getR() <= 0) {
			projectiles.removeAt(i);
			i = -1;

		}
		else { // If not out of the box -> check if we hit someone on the field
			bool resetIndex = false;
			bool removeProjectile = false;
			for (size_t j = 0; j < goals.size() && !resetIndex; ++j) {
				MovingTarget *goal = &goals[j];
				if (goal->collidesWith(proj)) {
					goals.removeAt(j);
					removeProjectile = true;
					resetIndex = true;
				}
			}

			// Check if we hit hunter
			if (hunter.collidesWith(proj)) {
				hunter.disable();

				projectiles.removeAt(i);
				resetIndex = true;
			}

			if(removeProjectile) projectiles.removeAt(i);

			if (resetIndex) i = -1;

		}

	}









	player.update(secondsPassed);


	if (hunter.isActive() && hunter.collidesWith(&player)) {
		goals.clear();
		outputMessage = "Game over. You LOSE!";
		state = Application::STATE_RELEASED;
		return;
	}


	for (size_t i = 0; i < goals.size(); ++i) {
		if (goals[i].collidesWith(&player)) {
			goals[i].setColor(RGB(64, 64, 64));
			goals.removeAt( i);
			break;
		}
	}
	if (goals.size() == 0) {
		outputMessage = "You Win!";
		state = Application::STATE_RELEASED;
	}
}
