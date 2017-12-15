#include "Animate.h"
#include "Utils.h"
#include "Content.h"
#include "VectorDrawer.h"

using namespace BGE;

Animate::Animate(void)
{

}

Animate::~Animate(void)
{

}

bool Animate::Initialise()
{
	// noraml calls - gravity is set to pull down animate if it jumps
	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();
	setGravity(glm::vec3(0, -20, 0));

	// animate is called, and butterflies fly on start  up
	// radius, width, height, length
	animal = make_shared<AnimateController>(3, 3, 3, 16);
	Attach(make_shared<ButterflyEffect>(true));
	return Game::Initialise();
}

void Animate::Update()
{
	//use physics torque twist
	glm::vec3 torque = animal->torque;

	/*MOVING*/
	//foward
	if (keyState[SDL_SCANCODE_UP])
	{
		animal->forward();
	}
	//backwards
	if (keyState[SDL_SCANCODE_DOWN])
	{
		animal->backwards();
	}
	// left
	else if (keyState[SDL_SCANCODE_LEFT])
	{
		animal->left(torque);
	}
	// right
	else if (keyState[SDL_SCANCODE_RIGHT])
	{
		animal->right(-torque);
	}
	else if (keyState[SDL_SCANCODE_TAB])
	{
		animal->attack();
	}
	// friction increased to stop animal from moving too much without controls
	else  {
		animal->body->rigidBody->setFriction(50);
		animal->tail->rigidBody->setFriction(50);
		animal->head->rigidBody->setFriction(50);
	}

	animal->Update();
	Game::Update();
}

void Animate::Cleanup()
{
	Game::Cleanup();
}