#pragma once

#include "GameObject.h"

class Particle : public GameObject
{
public:

	Particle() { 
		life_tick = 0; 
		isAlive = true;
		flags |= PARTICLE_OBJECT;
	};

	int life_tick;
	bool isAlive;

};

