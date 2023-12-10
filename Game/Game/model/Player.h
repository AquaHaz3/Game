#pragma once

#include "Entity.h"
#include <memory>
#include <vector>
#include "Item.h"

class Player : public Entity
{
	
public:

	Player() : Entity(0, 0, 32, 32, EntityID::Player) {}

	Player(int x, int y);

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

	Direction direction; // Направление

private:

	int walk_tick; // номер кадра (для анимации ходьбы)
	char x_offset;
	char y_offset;
	Sprite texture; // текстура игрока

	Sprite inventoryCell; 
	Sprite inventoryHand;
	std::vector<Item> inventory;
	int invFreeIndex; // индекс свободной ячейки

	void drawUI(); // Рисует интерфейс (Жизни, XP)
	void drawInventory();
	bool putToInventory(uint8_t id);

	void checkForAttack(); // Обработка атаки игроком (Проверки на нажатие мыши и т.д.)

	int xp;
	int remindAboutXp; // Напоминание о xp;
	char bow_progress; // натянутость лука
	char sword_progress;

	int inv_x; // 'x' позиция инвентаря на экране
	//int xpbar_x; // 'x' позиция 'xp' бара на экране

	Weapon weapon; // Наше текущее оружие в руке (его хар-ки)
	int weaponID;
};

typedef std::shared_ptr<Player> PlayerRef;

