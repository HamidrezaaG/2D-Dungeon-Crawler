#include "FrameworkPCH.h"

#include "EventManager.h"
#include "Event.h"
#include "Core/GameCore.h"

namespace fw {

    EventManager::EventManager()
    {

    }

    EventManager::~EventManager()
    {
        while (m_EventQueue.empty() == false)
        {
            delete m_EventQueue.front();
            m_EventQueue.pop();
        }
    }

    void EventManager::AddEvent(Event* pEvent, float dispatchDelay)
    {
        pEvent->SetDelay(dispatchDelay);
        m_EventQueue.push(pEvent);
    }

    void EventManager::AddEvent(Event* pEvent)
    {
        pEvent->SetDelay(0.0f);
        m_EventQueue.push(pEvent);
    }

    void EventManager::DispatchAllEvents(float deltaTime, GameCore* pGameCore)
    {

        std::queue<Event*> m_NotSentEventQueue;

        while (m_EventQueue.empty() == false)
        {
            // Remove it from the queue.
            Event* pEvent = m_EventQueue.front();
            m_EventQueue.pop();

            pEvent->m_Delay -= deltaTime;
            if (pEvent->m_Delay <= 0)
            {
                // Send it to the game.
                pGameCore->OnEvent(pEvent);

                // Delete the event.
                delete pEvent;
            }
            else
            {
                // Temporarily put the event into another queue.
                m_NotSentEventQueue.push(pEvent);
            }
        }
        while (m_NotSentEventQueue.empty() == false)
        {
            m_EventQueue.push(m_NotSentEventQueue.front());
            m_NotSentEventQueue.pop();
        }
    }

} // namespace fw