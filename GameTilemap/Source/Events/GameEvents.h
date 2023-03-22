#pragma once

class RemoveFromGameEvent : public fw::Event
{
public:
    RemoveFromGameEvent(fw::GameObject* pObject)
    {
        m_pObject = pObject;
    }
    virtual ~RemoveFromGameEvent() {}

    static const char* GetStaticEventType() { return "RemoveFromGameEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    fw::GameObject* GetGameObject() { return m_pObject; }

protected:
    fw::GameObject* m_pObject;
};
class ResetGameEvent : public fw::Event
{
public:
    ResetGameEvent() {};
    virtual ~ResetGameEvent() {}

    static const char* GetStaticEventType() { return "ResetGameEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }
};
class DynamicCollisionEvent : public fw::Event
{
public:
    DynamicCollisionEvent(fw::GameObject* _objA, fw::GameObject* _objB) : m_ObjectA(_objA), m_ObjectB(_objB) {};
    virtual ~DynamicCollisionEvent() {}

    static const char* GetStaticEventType() { return "DynamicCollisionEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    fw::GameObject* GetObjectA(){ return m_ObjectA; }
    fw::GameObject* GetObjectB(){ return m_ObjectB; }

protected:
    fw::GameObject* m_ObjectA;
    fw::GameObject* m_ObjectB;
};
class TileCollisionEvent : public fw::Event
{
public:
    TileCollisionEvent(fw::GameObject* _obj, fw::RectangleCollider* _tilecollider) : m_Object(_obj), m_TileCollider(_tilecollider) {};
    virtual ~TileCollisionEvent() {}

    static const char* GetStaticEventType() { return "TileCollisionEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    fw::GameObject* GetObject() { return m_Object; }
    fw::RectangleCollider* GetTileCollider() { return m_TileCollider; }

protected:
    fw::GameObject* m_Object;
    fw::RectangleCollider* m_TileCollider;
}; 
class PlayerDamageEvent : public fw::Event
{
public:
    PlayerDamageEvent(fw::GameObject* _instigator, int _damage) : m_Object(_instigator), m_Damage(_damage) {};
    virtual ~PlayerDamageEvent() {}

    static const char* GetStaticEventType() { return "PlayerDamageEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    fw::GameObject* GetInstigator() { return m_Object; }
    int GetDamage() { return m_Damage; }

protected:
    fw::GameObject* m_Object;
    int m_Damage;
};

class EnemyDamageEvent : public fw::Event
{
public:
    EnemyDamageEvent(fw::GameObject* _instigator, Vector2 _location, int _damage) : m_Object(_instigator), m_location(_location), m_Damage(_damage) {};
    virtual ~EnemyDamageEvent() {}

    static const char* GetStaticEventType() { return "EnemyDamageEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    fw::GameObject* GetInstigator() { return m_Object; }
    int GetDamage() { return m_Damage; }
    Vector2 GetLocation() { return m_location; }

protected:
    fw::GameObject* m_Object;
    Vector2 m_location;
    int m_Damage;
};