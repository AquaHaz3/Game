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

	Direction direction = Direction::RIGHT; // �����������

private:

	int walk_tick = 0; // ����� ����� (��� �������� ������)
	char x_offset = 0;
	char y_offset = 0;
	SpriteRef texture = 0; // �������� ������

	SpriteRef inventoryCell = 0; 
	SpriteRef inventoryHand = 0;
	std::vector<Item> inventory;
	int invFreeIndex = 0; // ������ ��������� ������

	void drawUI(); // ������ ��������� (�����, XP)
	void drawInventory();
	bool putToInventory(uint8_t id);

	void checkForAttack(); // ��������� ����� ������� (�������� �� ������� ���� � �.�.)

	int xp = 0;
	int remindAboutXp = 0; // ����������� � xp;
	char bow_progress = 0; // ����������� ����
	char sword_progress = 0;

	int inv_x = 0; // 'x' ������� ��������� �� ������
	//int xpbar_x; // 'x' ������� 'xp' ���� �� ������

	Weapon weapon; // ���� ������� ������ � ���� (��� ���-��)
	int weaponID = 0;

	bool isFrozen;
};

typedef std::shared_ptr<Player> PlayerRef;

