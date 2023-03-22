#pragma once

class Game;

class Arena : public fw::GameObject
{

public:
    Arena::Arena(Vector2 position, fw::Mesh* _pmesh, fw::ShaderProgram* _pshader, fw::GameCore* _pgame);
    ~Arena();

    void Update(float deltaTime) override;

    void SetRadius(float _radius);
    float GetRadius();

    void SetCalculatedVelocity(Vector2 _v);
    Vector2 GetCalculatedVelocity();

    virtual fw::k_ObjectType GetType() override;

protected:
    float m_Radius;
    Vector2 m_CalculatedVelocity;
};