
#ifndef BODY_H
#define BODY_H

#include "Framework.h"
#include "Model.h"
#include "Framebuffer.h"

#include <list>

#include <btBulletDynamicsCommon.h>


enum BodyType {
	MARS,
	ASTEROID,
	EROS,
	GOLEVKA,
	JUNO,
	GATE,
	SPACEBAT,
	NUM_BODY_TYPES
};

class Body: public btRigidBody {
	public:
		Body(Model *model, btRigidBody::btRigidBodyConstructionInfo &btInfo, BodyType ty);
		~Body();

		virtual void draw(RenderPass pass);
		BodyType getType();
		void printType();

	protected:
		Model *model;
		BodyType type;
};

#endif
