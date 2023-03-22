#pragma once

class Arena;
class Player;

class SimpleEnemy : public Enemy
{

public:
        SimpleEnemy(Player* _pplayer, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Arena* _pArena, fw::GameCore* _pgame);
        ~SimpleEnemy();

    virtual void Update(float deltaTime) override;
    
protected:

};