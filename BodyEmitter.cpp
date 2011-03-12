
#include "BodyEmitter.h"
#include "ObjectMotionState.h"

#define BOUNDARY_X 50.0
#define BOUNDARY_Y 44.0
#define BOUNDARY_Z -100.0

#define EMIT_STEP 1.0

const float NORMAL_SPEED = 5.0;
const float BOOST_SPEED = 65.0;

using namespace std;

BodyEmitter::BodyEmitter(btDiscreteDynamicsWorld *world) {
	this->world = world;
	accum = 0.0;
	boostMode = false;

	collisionShapes[MARS] = new btSphereShape(1);
	collisionShapes[ASTEROID] = new btSphereShape(1);
	collisionShapes[EROS] = new btSphereShape(1);
	
	btScalar mass = 4.0;
	btVector3 inertia(0,0,0);
	for(int i = 0; i < NUM_BODY_TYPES; i++){
		btCollisionShape* shape = collisionShapes[i];
		shape->calculateLocalInertia(mass, inertia);
	}

// These models were good... but a little too big?
//	collisionShapes[GOLEVKA] = new btSphereShape(1);
//	collisionShapes[JUNO] = new btSphereShape(1);
}

BodyEmitter::~BodyEmitter() {	
	for(int i = 0; i < NUM_BODY_TYPES; i++){
		delete collisionShapes[i];
	}
}

void BodyEmitter::boostSpeed(){
	list<Body*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++){
		Body* b = *it;
		btVector3 vel = b->getLinearVelocity();
		vel.setZ(BOOST_SPEED);
		b->setLinearVelocity(vel);
	}
}

void BodyEmitter::resetSpeed(){
	list<Body*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++){
		Body* b = *it;
		btVector3 vel = b->getLinearVelocity();
		vel.setZ(NORMAL_SPEED);
		b->setLinearVelocity(vel);
	}
}

void BodyEmitter::setBoostMode(bool boost){
	boostMode = boost;
}


float RandomFloat(float min, float max){
	float t = (float)rand() / RAND_MAX;
	return min + t * (max - min);
}

void BodyEmitter::emitBodies(float tstep) {
	accum += tstep;
	
	list<Body*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++){
		Body* b = (*it);
		btTransform trans;
		b->getMotionState()->getWorldTransform(trans);
		
		//printf("z: %f\n", trans.getOrigin().getZ());
	}
	//printf("--\n");
	
	if (accum > EMIT_STEP) {
		accum = 0.0;

		printf("%d\n", (int)bodies.size());
		
		btVector3 pos(
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_X - BOUNDARY_X,
				(float)rand() / RAND_MAX * 2.0 * BOUNDARY_Y - BOUNDARY_Y,
				-250);
		btDefaultMotionState *motionState =
			new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
		
		//ObjectMotionState *motionState =
		//	new ObjectMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
		
		int type = rand() % NUM_BODY_TYPES;
		
		btScalar mass = 4.0;
		
		btRigidBody::btRigidBodyConstructionInfo
			constructionInfo(mass, motionState, collisionShapes[type]);
		
		Body* newBody = new Body(&models[type], constructionInfo);
//		newBody->setLinearVelocity(btVector3(RandomFloat(-4,4),RandomFloat(-4,4),RandomFloat(-10,10)));
		
		float speed = boostMode ? BOOST_SPEED : NORMAL_SPEED;
		newBody->setLinearVelocity(btVector3(RandomFloat(-4,4),RandomFloat(-4,4),speed));
		world->addRigidBody(newBody);
		bodies.push_back(newBody);
	}
}

void BodyEmitter::drawBodies(RenderPass pass) {
	list<Body*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++)
		(*it)->draw(pass);
}

void BodyEmitter::loadModels() {
	models[MARS].loadFromFile("models/mars", "mars.3ds", importers[MARS]);
	models[ASTEROID].loadFromFile("models/aster", "asteroid.3ds", importers[ASTEROID]);
	models[EROS].loadFromFile("models/eros", "eros.3ds", importers[EROS]);
//	models[GOLEVKA].loadFromFile("models/golevka", "golevka.3ds", importers[GOLEVKA]);
//	models[JUNO].loadFromFile("models/juno", "juno.3ds", importers[JUNO]);
}
