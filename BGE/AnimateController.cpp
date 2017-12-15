#include "AnimateController.h"
#include "Utils.h"
#include "Content.h"
#include "VectorDrawer.h"

using namespace BGE;

//reference
//controller class - used as per all other controller classes in the BGE but implemented differently
AnimateController::AnimateController(float radius, float width, float height, float length)
{
	//normal variables set
	elapsed = 10000.0f;
	animatePosition = glm::vec3(0, 10, 0);
	rigidBodyStrength = 2000.0f;
	//passed to the animate.cpp file for any change
	CreateAnimate(radius, width, height, length);
	//rigid body fictions set
	body->rigidBody->setFriction(25);
	tail->rigidBody->setFriction(25);
	head->rigidBody->setFriction(25);
}

AnimateController::~AnimateController()
{

}

void AnimateController::Update()
{
	//reference
	//torque concept taken from the sceneGraphGame file but utilised in a different way
	torque = glm::cross(body->transform->right*(rigidBodyStrength * 10), body->transform->look*10.0f);

	GameComponent::Update();
}

// reference - taken from the game component file
void AnimateController::Cleanup()
{
	GameComponent::Cleanup();
}

//create legs
// reference
// this tutorial kind of helped http://www.alsprogrammingresource.com/common_ode_joints.html
// also used void SceneGraphGame::CreateScene() to help understand the implementation

//right side is created first
shared_ptr<PhysicsController> AnimateController::CreateLeg(bool right_side)
{
	glm::vec3 position = animatePosition;
	//set leg attributes
	//legs are made of two boxes each
	float inner_leg_length = 4;
	float outer_leg_length = 6;
	float blockWidth = .5;
	float blockDepth = 1;

	// box create taken form the physics library as per the assignment example
	if (right_side) {
		//leg hinges
		position = glm::vec3(0, 10, 0);
		//CreateBox(blockWidth, blockHeight, blockDepth, glm::vec3(x, y, z), glm::quat());
		//inner leg
		shared_ptr<PhysicsController> leg_hinge1 = Game::Instance()->physicsFactory->CreateBox(inner_leg_length, blockWidth, blockDepth, position, glm::quat());
		leg_hinge1->rigidBody->setFriction(btScalar(0.0f));
		//outer leg
		shared_ptr<PhysicsController> leg_hinge2 = Game::Instance()->physicsFactory->CreateBox(outer_leg_length, blockWidth, blockDepth, position, glm::quat());
		leg_hinge2->rigidBody->setFriction(btScalar(0.0f));

		/* ASSOCIATIONS */

		// constraints of hinge joint
		btTransform ht1, ht2;
		ht1.setIdentity();
		ht2.setIdentity();
		ht1.setOrigin(btVector3(2.5, 0, 0));
		ht2.setRotation(GLToBtQuat(glm::angleAxis(90.0f, glm::vec3(0, 0, 1))));
		ht2.setOrigin(btVector3(-2.5, 0, 0));
		//create hinge1
		btFixedConstraint* hinge2;
		hinge2 = new btFixedConstraint(*leg_hinge1->rigidBody, *leg_hinge2->rigidBody, ht1, ht2);
		Game::Instance()->dynamicsWorld->addConstraint(hinge2);
		return leg_hinge1;
	}
	//right leg
	else {
		position = glm::vec3(-5, 10, 0);
		shared_ptr<PhysicsController> leg_hinge1 = Game::Instance()->physicsFactory->CreateBox(inner_leg_length, blockWidth, blockDepth, position, glm::quat());
		shared_ptr<PhysicsController> leg_hinge2 = Game::Instance()->physicsFactory->CreateBox(outer_leg_length, blockWidth, blockDepth, position, glm::quat());
		leg_hinge2->rigidBody->setFriction(btScalar(0.0f));

		btFixedConstraint* hinge1_hinge2;
		//adding variables to constraint
		btTransform ht1, ht2;
		//attach hinge1 to hinge2
		ht1.setIdentity();
		ht2.setIdentity();
		ht1.setOrigin(btVector3(-2.5, 0, 0));
		//set rotation constraint
		//reference
		// concept taken from sceneGraphGame file where fized joint is implemented
		ht2.setRotation(GLToBtQuat(glm::angleAxis(-90.0f, glm::vec3(0, 0, 1))));
		ht2.setOrigin(btVector3(2.5, 0, 0));
		hinge1_hinge2 = new btFixedConstraint(*leg_hinge1->rigidBody, *leg_hinge2->rigidBody, ht1, ht2);
		//reference
		//based on the following : dynamicsWorld->addConstraint(fixed);
		//created a new joint constraint called hinge1_hinge2
		Game::Instance()->dynamicsWorld->addConstraint(hinge1_hinge2);
		return leg_hinge1;
		return leg_hinge2;
	}
}

//creates the bubblebody and tail, attaches the legs to the body 
//sclae is used as in the physics factory
shared_ptr<PhysicsController> AnimateController::CreateBubbles(vector<shared_ptr<PhysicsController>> legs, glm::vec3 scale)
{
	// normal variables
	glm::vec3 position = animatePosition;
	glm::vec3 offset;

	//variables to define the catepillars body - based on the work completed in the sceneGraphGame file
	//scale is used as in the physics facrory
	float radius = scale.x;
	float diameter = radius * 2;
	float width = scale.x;
	float height = scale.y;
	float length = scale.z;

	//set contraint for hinge joint to body - milter joint - 45 degrees
	float hingeConstraint = glm::half_pi<float>() / 2;
	//reference
	// concept of using offsets taken from the sceneGraphGame file, all implementations are my own
	// create an offset position 
	offset = glm::vec3(0, 0, length+(radius));

	//head  - start of movement - bubble 1
	head = Game::Instance()->physicsFactory->CreateSphere(radius*1.05, position + offset, glm::quat());
	//creating a fixed joint
	glm::quat q = glm::angleAxis(30.0f, glm::vec3(0, 1, 0));
	// bubble 2
	shared_ptr<PhysicsController> bubblebody = Game::Instance()->physicsFactory->CreateSphere(radius*0.9, position, q);
	// bubble 3
	shared_ptr<PhysicsController> bubblebody3 = Game::Instance()->physicsFactory->CreateSphere(radius*0.9, position + offset, glm::quat());
	// bubble 4
	shared_ptr<PhysicsController> bubblebody4 = Game::Instance()->physicsFactory->CreateSphere(radius*0.8, position + offset, glm::quat());
	//end of movement - bubble 5
	tail = Game::Instance()->physicsFactory->CreateSphere(radius*0.8, position + offset, glm::quat());

	//connnect head to tail
	btFixedConstraint* head_tail;
	btTransform t1, t2;
	t1.setIdentity();
	t2.setIdentity();
	t1.setOrigin(btVector3(0, 0, length + diameter));
	t2.setOrigin(btVector3(0, 0, 0));
	head_tail = new btFixedConstraint(*head->rigidBody, *tail->rigidBody, t1, t2);
	Game::Instance()->dynamicsWorld->addConstraint(head_tail);

	//connnect bubblebody2 to head - fixed joint
	btTransform t3,t4;
	t3.setIdentity();
	t4.setIdentity();
	t3.setOrigin(btVector3(0, 0, length));
	t4.setRotation(GLToBtQuat(glm::angleAxis(-30.0f, glm::vec3(0, 0, 0)))); 
	t4.setOrigin(btVector3(0, 0, 0));
	btFixedConstraint * fixed = new btFixedConstraint(*bubblebody->rigidBody, *tail->rigidBody, t3, t4);
	Game::Instance()->dynamicsWorld->addConstraint(fixed);

	//connnect bubbybod3y to tail
	btFixedConstraint* universal;
	btTransform t5, t6;
	t5.setIdentity();
	t6.setIdentity();
	t5.setOrigin(btVector3(0, 0, length-diameter));
	t6.setRotation(GLToBtQuat(glm::angleAxis(180.0f, glm::vec3(0, 0, 0))));
	t6.setOrigin(btVector3(0, 0, 0));
	universal = new btFixedConstraint(*bubblebody3->rigidBody, *tail->rigidBody, t5, t6);
	Game::Instance()->dynamicsWorld->addConstraint(universal);

	//connnect bubbybody4 to tail
	btFixedConstraint* bubblebody4_tail;
	btTransform t7, t8;
	t7.setIdentity();
	t8.setIdentity();
	t7.setOrigin(btVector3(0, 0, length/3));
	t8.setOrigin(btVector3(0, 0, 0));
	bubblebody4_tail = new btFixedConstraint(*bubblebody4->rigidBody, *tail->rigidBody, t7, t8);
	Game::Instance()->dynamicsWorld->addConstraint(bubblebody4_tail);

	//create the legs and connections to the second bubble sphere
	for (int x = 0; x < 6; x++)
	{
		if (x<3) {
			// left
			btVector3 body_spacing = btVector3(-width - (radius/4), 0, (length/2) - ((length/4)*x));
			btVector3 leg_spacing = btVector3(0, 0, 0);
			btHingeConstraint* rightHinge;
			rightHinge = new btHingeConstraint(*bubblebody->rigidBody, *legs.at(x)->rigidBody, body_spacing, leg_spacing, btVector3(0, 1, 0), btVector3(-1, 1, 0), true);
			rightHinge->setLimit(-hingeConstraint, hingeConstraint);
			joints.push_back(rightHinge);
			Game::Instance()->dynamicsWorld->addConstraint(rightHinge);
		}
		else {
			// right
			btVector3 body_spacing = btVector3(width + (radius / 4), 1, (length / 2) - ((length / 4)*(x - 3)));
			btVector3 leg_spacing = btVector3(0, 0, 0);
			btHingeConstraint* leftHinge;
			leftHinge = new btHingeConstraint(*bubblebody->rigidBody, *legs.at(x)->rigidBody, body_spacing, leg_spacing, btVector3(0, 1, 0), btVector3(1, 1, 0), true);
			leftHinge->setLimit(-hingeConstraint, hingeConstraint);
			joints.push_back(leftHinge);
			Game::Instance()->dynamicsWorld->addConstraint(leftHinge);
		}
	}
	return bubblebody;
}

shared_ptr<PhysicsController> AnimateController::CreateAnimate(float _radius, float _width, float _height, float _length)
{
	// shared variables from AnimateController::CreateBubbles
	float radius = _radius;
	float width = _width;
	float height = _height;
	float length = _length;
	// as defined in vec3
	glm::vec3 scale = glm::vec3(width, height, length);

	// create the three legs
	for (int x = 0; x < 3; x++)
	{
		legs.push_back(CreateLeg(false));
	}
	for (int x = 0; x < 3; x++)
	{
		legs.push_back(CreateLeg(true));
	}
	body = CreateBubbles(legs, scale);

	return body;
}


/* MOVING CONTROLS */
//foward
void AnimateController::forward()
{
	head->rigidBody->setFriction(0);
	body->rigidBody->setFriction(0);
	head->rigidBody->applyCentralForce(GLToBtVector(head->transform->look * (rigidBodyStrength*10)));
}
//back
void AnimateController::backwards()
{
	tail->rigidBody->setFriction(0);
	head->rigidBody->setFriction(0);
	tail->rigidBody->applyCentralForce(GLToBtVector(tail->transform->look * (rigidBodyStrength*-10)));
}
//left 
void AnimateController::left(glm::vec3 torque)
{
	body->rigidBody->setFriction(0);
	tail->rigidBody->setFriction(0);
	body->rigidBody->applyTorque(GLToBtVector(torque));
}

//right
void AnimateController::right(glm::vec3 torque)
{
	body->rigidBody->setFriction(0);
	tail->rigidBody->setFriction(0);
	body->rigidBody->applyTorque(GLToBtVector(torque));
}

//fps controller exploration
void AnimateController::attack()
{
	tail->rigidBody->setFriction(0);
	body->rigidBody->setFriction(0);
	tail->rigidBody->applyCentralForce(GLToBtVector(body->transform->up * (rigidBodyStrength*5)));
}