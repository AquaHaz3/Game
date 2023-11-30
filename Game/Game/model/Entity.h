#pragma once

#include "../core/Box2D.h"

class Entity : public Box2D
{

public:

	Entity();

	virtual void Draw() override;
	virtual void Update() override;
	virtual void OnEvent(Event* event) override;

private:

	int health;

};

