#pragma once
#include"stdafx.h"
#include"Object.h"

class Player : public GameObject
{
	int m_Id;
public:
	Player(int id);
	~Player();

	void SetId(int id);

	int GetId();
};