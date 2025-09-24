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
	Enemy* enemy1 = new Enemy();
	Enemy* enemy2 = new Enemy();

	player->SetTarget(enemy1);

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
		else if (command == 3)
		{
			std::cout << "Player destroyed" << std::endl;
			delete player;
			player = nullptr;
		}
		else if (command == 4)
		{
			enemy1->TakeDamage(2);
		}
		else if (command == 5)
		{
			player->SetTarget(enemy2);
		}

		if (enemy1 != nullptr && enemy1->GetHp() == 0)
		{
			std::cout << "Enemy1 destroyed" << std::endl;
			delete enemy1;
			enemy1 = nullptr;
		}
		if (enemy2 != nullptr && enemy2->GetHp() == 0)
		{
			std::cout << "Enemy2 destroyed" << std::endl;
			delete enemy2;
			enemy2 = nullptr;
		}
	}

	delete enemy2;
	delete enemy1;
	delete player;
}