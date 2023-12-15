#pragma once

#include "Entity.h"
#include <memory>
#include <vector>
#include "Item.h"

class Player : public Entity
{
	
public:

	Player();

	Player(int x, int y);

	virtual void Draw() override;
	virtual void Update(__int64 tick) override;
	virtual void OnEvent(Event* event) override;

	Direction direction = Direction::RIGHT; // Направление

private:

	int walk_tick = 0; // номер кадра (для анимации ходьбы)
	char x_offset = 0;
	char y_offset = 0;
	SpriteRef texture = 0; // текстура игрока

	SpriteRef inventoryCell = 0; 
	SpriteRef inventoryHand = 0;
	std::vector<Item> inventory;
	int invFreeIndex = 0; // индекс свободной ячейки

	void drawUI(); // Рисует интерфейс (Жизни, XP)
	void drawInventory();
	bool putToInventory(uint8_t id);

	void checkForAttack(); // Обработка атаки игроком (Проверки на нажатие мыши и т.д.)

	int xp = 0;
	int remindAboutXp = 0; // Напоминание о xp;
	char bow_progress = 0; // натянутость лука
	char sword_progress = 0;

	int inv_x = 0; // 'x' позиция инвентаря на экране
	//int xpbar_x; // 'x' позиция 'xp' бара на экране

	Weapon weapon; // Наше текущее оружие в руке (его хар-ки)
	int weaponID = 0;

	bool isFrozen;
};

typedef std::shared_ptr<Player> PlayerRef;

