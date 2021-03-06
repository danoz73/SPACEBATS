#ifndef BODY_EMITTER_H
#define BODY_EMITTER_H

#include "Framework.h"
#include "ParticleEngine.h"
#include "Body.h"
#include "Model.h"
#include <list>
#include "btBulletDynamicsCommon.h"
#include "Level.h"

class BodyEmitter {
	public:
		BodyEmitter(btDiscreteDynamicsWorld *world);
		~BodyEmitter();

		void loadModels();


		void emitBodies(float tstep, ParticleEngine* pEngine, Level* level);
		void drawBodies(RenderPass pass);
	
		void emit(BodyType type, ParticleEngine* pEngine);
		void clear();
	
		void boostSpeed();
		void resetSpeed();
		void setBoostMode(bool boost);	
	
		void setEmitSpeed(float speed);

	private:
		float emitSpeed;
	
		struct ContactCallback : public btCollisionWorld::ContactResultCallback {
			BodyEmitter *bodyEmitter;

			ContactCallback(BodyEmitter *bodyEmitter);
			btScalar addSingleResult(btManifoldPoint &cp,
					const btCollisionObject *colObj0, int partId0, int index0,
					const btCollisionObject *colObj1, int partId1, int index1);
		};
		friend struct ContactCallback;
		ContactCallback *contactCallback;

		btVector3 getAngularVelocityForType(BodyType type);
		btVector3 getPositionForType(BodyType type);
		btVector3 getLinearVelocityForType(BodyType type);
		btScalar getMassForType(BodyType type);
		void setSpeed(float speed);
	
	
		btDiscreteDynamicsWorld *world;
		std::list<Body*> bodies;

		float accum;

		Model models[NUM_BODY_TYPES];
		Assimp::Importer importers[NUM_BODY_TYPES];

		btCollisionObject *wall;
		btCollisionShape *wallShape;
	
		bool boostMode;
};

#endif
