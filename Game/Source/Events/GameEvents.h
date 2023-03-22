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


class SpawnEnemyEvent : public fw::Event
{
public:
    SpawnEnemyEvent() {};
    virtual ~SpawnEnemyEvent() {}

    static const char* GetStaticEventType() { return "SpawnEnemyEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }
};

class CollisionEvent : public fw::Event
{
public:
    CollisionEvent(fw::GameObject* _objA, fw::GameObject* _objB) : m_ObjectA(_objA), m_ObjectB(_objB) {};
    virtual ~CollisionEvent() {}

    static const char* GetStaticEventType() { return "CollisionEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    fw::GameObject* GetObjectA(){ return m_ObjectA; }
    fw::GameObject* GetObjectB(){ return m_ObjectB; }

protected:
    fw::GameObject* m_ObjectA;
    fw::GameObject* m_ObjectB;
};

class UpdatePlayerHealthEvent : public fw::Event
{
public:
    UpdatePlayerHealthEvent() {};
    virtual ~UpdatePlayerHealthEvent() {}

    static const char* GetStaticEventType() { return "UpdatePlayerHealthEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }
};


