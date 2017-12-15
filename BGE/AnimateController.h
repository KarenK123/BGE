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

namespace BGE
{
	class AnimateController :
		public GameComponent
	{
	private:
		//for creating the spider
		shared_ptr<PhysicsController> CreateLeg(bool leftLeg);
		shared_ptr<PhysicsController> CreateBubbles(vector<shared_ptr<PhysicsController>> legs, glm::vec3 scale);

		//for setting a animatePosition location
		glm::vec3 animatePosition;
	public:
		//standard methods
		AnimateController(float radius, float length, float width, float height);
		AnimateController(glm::vec3 position);
		AnimateController();
		~AnimateController();

		void Update();
		void Cleanup();

		//custom methods
		void forward();
		void backwards();
		void left(glm::vec3 torque);
		void right(glm::vec3 torque);
		void attack();

		shared_ptr<PhysicsController> CreateAnimate(float radius, float width, float height, float length);

		//custom variables
		vector<shared_ptr<PhysicsController>> legs;
		vector<btHingeConstraint*> joints;
		shared_ptr<PhysicsController> body;
		shared_ptr<PhysicsController> tail;
		shared_ptr<PhysicsController> head;

		shared_ptr<PhysicsController> animal;

		//for leg timing and alternations
		bool alternate;
		float elapsed;
		float timeToMove;

		//for movement of legs
		int velocity;
		int strength;
		//for movement of rigidbody
		float rigidBodyStrength;

		//for rigidbody turning
		glm::vec3 torque;
		glm::vec3 hardTorque;
	};
}