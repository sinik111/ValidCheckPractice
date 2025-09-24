#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <queue>
#include <cassert>

#include "Object.h"

class Enemy :
	public Object
{
private:
	int m_hp = 10;

public:
	void TakeDamage(int damage)
	{
		m_hp -= damage;

		m_hp = std::max(m_hp, 0);

		std::cout << "Enemy TakeDamage Current Hp :" << m_hp << std::endl;
	}

	int GetHp() const
	{
		return m_hp;
	}
};

class Player :
	public Object
{
private:
	Enemy* m_target = nullptr;

public:
	void SetTarget(Enemy* target)
	{
		SafeAssign(m_target, target);
	}

	void Attack()
	{
		if (m_target != nullptr)
		{
			m_target->TakeDamage(2);
		}
		else
		{
			std::cout << "Target is nullptr" << std::endl;
		}
	}
};



int main()
{
	Player* player = new Player();
	Enemy* enemy = new Enemy();

	player->SetTarget(enemy);

	while (true)
	{
		int command = 0;

		std::cin >> command;

		if (command == 1)
		{
			player->Attack();
		}
		else if (command == 2)
		{
			break;
		}

		if (enemy != nullptr && enemy->GetHp() == 0)
		{
			delete enemy;
			enemy = nullptr;
		}
	}

	delete enemy;
	delete player;
}