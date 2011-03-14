
#include "ParticleEngine.h"

ParticleEngine::ParticleEngine(int width){
	this->width = width;
	sf::Image* fiya = new sf::Image();
	fiya->Bind();
	fiya->LoadFromFile("models/FireRed.png");
	particleImages.push_back(fiya);
	
	sf::Image* electric = new sf::Image();
	electric->Bind();
	electric->LoadFromFile("models/SparkBlue.png");
	particleImages.push_back(electric);
	
	Shader* fireShader = new Shader("shaders/fire");
	if (!fireShader->loaded()) {
		std::cerr << "Shader failed to load" << std::endl;
		std::cerr << fireShader->errors() << std::endl;
		exit(-1);
	}
	particleShaders.push_back(fireShader);
}

//void ParticleEngine::addEmitter(Ship* ship, int particleType){
void ParticleEngine::addEmitter(btVector3* pos, int particleType){
	ParticleEmitter newEmitter(pos, particleShaders[0], particleImages[particleType], this->width);
	emitters.push_back(newEmitter);
}

void ParticleEngine::updateEmitters(double tstep, bool fast){
	//if(counter%2)return;
	for(unsigned i = 0; i < emitters.size(); i++){
		emitters[i].updateEmitter(tstep, fast);
	}
}

void ParticleEngine::renderEmitters(bool fast){
	
	glUseProgram(particleShaders[0]->programID());
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_POINT_SPRITE);
	
//	for(unsigned i = 0; i < emitters.size(); i++){
//		emitters[i].renderParticles();
//	}
	
	emitters[0].renderParticles();
	if(fast) emitters[1].renderParticles();
	
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}