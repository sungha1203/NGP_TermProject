#include "stdafx.h"
#include "Player.h"


Player::Player() : GameObject()
{
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 0.0f;
	move_pos = { -12.0f,0.2f,-9.5f }; //플레이어 초기위치

	m_textureNum = 10;
	ReadObj("Player1.obj", m_vertex, m_normal, m_vt);
	scale[0] = 0.007;
	scale[1] = 0.007;
	scale[2] = 0.007;
	SetVBO();
}

Player::~Player()
{
}
