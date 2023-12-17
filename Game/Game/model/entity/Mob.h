#pragma once

#include "../Entity.h"

class Mob : public Entity
{

public:

	Mob(int x, int y, EntityID id);

	virtual void Draw();
	void inline updateDirection(float& angle);
	virtual void Update(__int64 tick);

	virtual void OnEvent(Event* event) override;

private:

	bool isHaveTarget; 

	void chooseTarget(__int64 tick); // Выбор цели
	void toogleState(); // 

	Entity* target; // Цель
	bool isIdle; // Статус (true - ожидание, false - движение)

	int idleTick; // Тик простоя
	char direction; // направление

	float detectRadius; // Радиус обнаружения
	float idleRadius; // Радиус доступного движения во время простоя
	bool isDistanceBattle; // Атакаует с дистанции, если false - ближний бой
	bool isSolid; // Через стены не проникает

	Vector2 idlePos; // Позиция простоя

    int _idle_MoveState; // 
    float _idle_dx; //
    float _idle_dy; //

	int max_hp; // Здоровье 

};

