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

	Direction direction; // �����������

private:

	int walk_tick; // ����� ����� (��� �������� ������)
	char x_offset;
	char y_offset;
	Sprite texture; // �������� ������

	Sprite inventoryCell; 
	Sprite inventoryHand;
	std::vector<Item> inventory;
	int invFreeIndex; // ������ ��������� ������

	void drawUI(); // ������ ��������� (�����, XP)
	void drawInventory();
	bool putToInventory(uint8_t id);

	void checkForAttack(); // ��������� ����� ������� (�������� �� ������� ���� � �.�.)

	int xp;
	int remindAboutXp; // ����������� � xp;
	char bow_progress; // ����������� ����
	char sword_progress;

	int inv_x; // 'x' ������� ��������� �� ������
	//int xpbar_x; // 'x' ������� 'xp' ���� �� ������

	Weapon weapon; // ���� ������� ������ � ���� (��� ���-��)
	int weaponID;
};

typedef std::shared_ptr<Player> PlayerRef;

