#pragma once

namespace fw {

    class Event;
    class EventManager;
    class FWCore;
    class Camera;
    class ShaderProgram;

    class GameCore
    {
    public:
        GameCore(FWCore* pFramework);
        virtual ~GameCore();

        virtual void Update(float deltaTime) = 0;
        virtual void OnEvent(Event* pEvent) = 0;
        virtual void Draw() = 0;

        FWCore* GetFramework();
        EventManager* GetEventManager() { 
            int bp = 1;
            return m_pEventManager; 
        
        }

        virtual Camera* GetActiveCamera();
        virtual ShaderProgram* GetDebugShader() = 0;

    protected:
        FWCore* m_pFramework = nullptr;
        EventManager* m_pEventManager = nullptr;
        
        Camera* m_pActiveCamera;
    };

} // namespace fw
