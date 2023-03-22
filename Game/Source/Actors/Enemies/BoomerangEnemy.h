#pragma once

class Arena;
class Player;

class BoomerangEnemy : public Enemy
{

public:
        BoomerangEnemy(Player* _pplayer, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Arena* _pArena, fw::GameCore* _pgame);
        ~BoomerangEnemy();

    virtual void Update(float deltaTime) override;
    
protected:
};