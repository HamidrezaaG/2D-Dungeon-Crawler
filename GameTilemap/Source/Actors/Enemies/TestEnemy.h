#pragma once
#include "Enemy.h"

class TestEnemy: public Enemy
{
public:
	TestEnemy(Vector2 position, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Player* _pplayer, fw::GameCore* _pgame);
	~TestEnemy();

private:

};
