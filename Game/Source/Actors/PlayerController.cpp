#include "GamePCH.h"

#include "PlayerController.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::StartFrame()
{
    m_OldFlags = m_Flags;
}

void PlayerController::OnEvent(fw::Event* pEvent)
{
    if( pEvent->GetType() == fw::InputEvent::GetStaticEventType() )
    {
        fw::InputEvent* pInputEvent = static_cast<fw::InputEvent*>( pEvent );
        
        if( pInputEvent->GetDeviceType() == fw::InputEvent::DeviceType::Keyboard )
        {
                unsigned int keyCode = pInputEvent->GetKeyCode();

            if( pInputEvent->GetDeviceState() == fw::InputEvent::DeviceState::Pressed )
            {
                if (keyCode == k_Controls_UP   || keyCode == VK_UP   ) { m_Flags |= Mask::Up;   }
                if (keyCode == k_Controls_LEFT || keyCode == VK_LEFT ) { m_Flags |= Mask::Left; }
                if (keyCode == k_Controls_DOWN || keyCode == VK_DOWN ) { m_Flags |= Mask::Down; }
                if (keyCode == k_Controls_RIGHT|| keyCode == VK_RIGHT) { m_Flags |= Mask::Right;}
                
                if (keyCode == k_Controls_RESTART) { m_Flags |= Mask::Restart; }

            }

            if( pInputEvent->GetDeviceState() == fw::InputEvent::DeviceState::Released )
            {
                if (keyCode == k_Controls_UP   || keyCode == VK_UP) { m_Flags &= ~Mask::Up; }
                if (keyCode == k_Controls_LEFT || keyCode == VK_LEFT) { m_Flags &= ~Mask::Left; }
                if (keyCode == k_Controls_DOWN || keyCode == VK_DOWN) { m_Flags &= ~Mask::Down; }
                if (keyCode == k_Controls_RIGHT|| keyCode == VK_RIGHT) { m_Flags &= ~Mask::Right;}

                if (keyCode == k_Controls_RESTART) { m_Flags &= ~Mask::Restart; }

            }
        }
    }
}

bool PlayerController::IsHeld(Mask mask)
{
    return (m_Flags & mask) != 0;
}

bool PlayerController::WasNewlyPressed(Mask mask)
{
    return ((m_Flags & mask) != 0) && ((m_OldFlags & mask) == 0);
}

bool PlayerController::WasNewlyReleased(Mask mask)
{
    return ((m_Flags & mask) == 0) && ((m_OldFlags & mask) != 0);
}