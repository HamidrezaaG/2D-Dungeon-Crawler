#pragma once


namespace fw {

    class Collider;
    class Texture;
    enum class k_ObjectType { Unknown, Player, Enemy, Dynamic, Pickup, Door };

class GameObject
{

public:

    GameObject(float _x, float _y, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame);
    GameObject(Vector2 _pos, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame);
    GameObject(fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame);

    void Init(Vector2 _pos, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame);

    virtual ~GameObject();

    virtual void Update(float deltaTime);

    virtual fw::Collider* GetCollider();
    virtual void OnCollisionHit(GameObject* _pOtherObject);

    virtual void Draw();

    void SetColorAlpha(float alpha);
    float GetColorAlpha();

    fw::Mesh* GetMesh();
    void SetMesh(fw::Mesh* _mesh);

    Vector2 GetPosition();
    Vector2 GetVelocity();
    Vector2 GetScale();
    float GetAngle();

    void SetPosition(float _x, float _y);
    void SetVelocity(Vector2 _v);
    void SetPosition(Vector2 _v);
    void SetAngle(float r);
    void SetScale(float s);
    void SetScale(Vector2 s);
    
    void SetTexture(fw::Texture* _texture);
    fw::Texture* GetTexture();
    
    void SetHidden(bool _hidden);

    Color GetColor();
    void SetColor(Color color);


    std::string GetName();

    virtual k_ObjectType GetType();

protected:
  
    fw::GameCore* m_pGameCore = nullptr;


    Vector2 m_Position = Vector2::Zero();
    Vector2 m_Velocity = Vector2::Zero();
    Vector2 m_Scale = Vector2::One();

    float m_Angle = 0;
    bool m_Hidden = false;

    fw::Mesh* m_pMesh = nullptr;
    fw::ShaderProgram* m_pShader = nullptr;
    
    fw::Texture* m_pTexture = nullptr;
    Vector2 m_UVScale = Vector2::One();
    Vector2 m_UVOffset = Vector2::Zero();

    Color m_Color = Color::White();

    fw::Camera* m_pActiveCamera;

    std::string m_Name;

};



}