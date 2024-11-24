#include "stdafx.h"
#include "Player.h"


Player::Player(int id) : GameObject()
{
	m_Id = -1;
	move_pos = { -12.0f,0.2f,-9.5f }; 
	m_box.Center = XMFLOAT3(-12.0f, 0.2f, -9.5f);
	m_box.Extents = XMFLOAT3(0.05f, 0.05f, 0.05f);;
	if (id == 1) {
		ReadObj("Player1.obj", m_vertex, m_normal, m_vt);
		m_textureNum = 10;
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 0.0f;
		scale[0] = 0.007;
		scale[1] = 0.007;
		scale[2] = 0.007;
	}
	if (id == 2) {
		ReadObj("Player2.obj", m_vertex, m_normal, m_vt);
		m_textureNum = 11;
		color.r = 1.0f;
		color.g = 0.5f;
		color.b = 0.3f;
		scale[0] = 0.003;
		scale[1] = 0.003;
		scale[2] = 0.003;
	}
	SetVBO();
}

Player::~Player()
{

}

void Player::SetId(int id)
{
	m_Id = id;
}

int Player::GetId()
{
	return m_Id;
}