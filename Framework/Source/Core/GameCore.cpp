#include "FrameworkPCH.h"

#include "GameCore.h"

namespace fw {

    GameCore::GameCore(FWCore* pFramework)
    {
        m_pFramework = pFramework;
    }

    fw::FWCore* GameCore::GetFramework()
    {
        return m_pFramework;
    }

    Camera* GameCore::GetActiveCamera()
    {
        return m_pActiveCamera;
    }

    GameCore::~GameCore()
    {
    }

} // namespace fw
