#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <iostream>
#include "Content.h"
#include "VectorDrawer.h"
#include "Steerable3DController.h"
#include "SteeringControler.h"
#include "Params.h"
#include "Box.h"
#include "ButterflyEffect.h"
#include "AnimateController.h"

namespace BGE
{
	class Animate :
		public Game
	{
	public:
		//standard methods
		Animate(void);
		~Animate(void);
		bool Initialise();
		void Update();
		void Cleanup();

		//call the animal and the butterfly
		shared_ptr<AnimateController> animal;
		shared_ptr<ButterflyEffect> butterfly;

		float elapsed;
		float timeChange;
		int count;
	};
}