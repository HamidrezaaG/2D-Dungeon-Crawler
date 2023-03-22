#pragma once

namespace fw {

    class Event;
    class GameCore;

    class EventManager
    {
    public:
        EventManager();
        ~EventManager();

        void AddEvent(Event* pEvent, float dispatchDelay);
        void AddEvent(Event* pEvent);

        void DispatchAllEvents(float deltaTime, GameCore* pGameCore);

    protected:
        std::queue<Event*> m_EventQueue;
    };

} // namespace fw
