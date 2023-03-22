#pragma once

class PlayerTrail : public fw::GameObject
{

public:
    PlayerTrail(GameObject* _targetObj, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame);
    
    void init(float _lerpIntensity, float _radius);
    
    ~PlayerTrail();

    void Update(float deltaTime) override;

    void SetCircle(float _radius, int _vertCount, bool _filled);

    void SetVertCount(int _vertCount);

    virtual fw::k_ObjectType GetType() override;

protected:

    GameObject* m_TargetObject;

    bool m_IsFilled = true;
    float m_Radius;
    float m_alpha;
    int m_VertCount;

    float m_LerpIntensity = 1.0f;
};