
#include "Gate.h"
#include "Framework.h"
#include "Model.h"
#include <iostream>
#include <cmath>

using namespace sf;


sf::Image* Gate::changed;

Gate::Gate(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty, ParticleEngine* pEngine)

: Body(model, btInfo, ty){
	particleEngine = pEngine;
	completed = false;
	radius = 5.0;
}

void Gate::loadChangeImage(){
	changed = new Image();
	changed->LoadFromFile("models/green.jpg");
	printf("Loaded gate image\n");
}

float Gate::getRadius(){
	return radius;
}

void Gate::setRadius(float r){
	radius = r;
}


Gate::~Gate() {}

void Gate::setCompleted(){
	completed = true;
	
	btTransform gateTransform;
	this->getMotionState()->getWorldTransform(gateTransform);
	btVector3 gatePos = gateTransform.getOrigin();
	
	this->setAngularVelocity(btVector3(1,1,10));
	if(rand()%2)this->setAngularVelocity(btVector3(1,-1,-10));
	//gatePos.m_floats[2] -= 40;
	particleEngine->addEmitter(&gatePos, EXPLOSION, false, true, 2);

}

void Gate::draw(RenderPass pass){
	btTransform gateTransform;
	this->getMotionState()->getWorldTransform(gateTransform);
	
	if(completed){
		gateTransform*=btTransform(btQuaternion(0,0,0,1),btVector3(0.1,0.1,0.1));
	}

	model->setTransformation(gateTransform);

	sf::Image* saved = NULL;
	if(completed){
		saved = model->getDiffuseImage();
		model->setDiffuseImage(changed);
	}
	
	model->render(pass);
	
	if(completed){
		model->setDiffuseImage(saved);
	}
}
