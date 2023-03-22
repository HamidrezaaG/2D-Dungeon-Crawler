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
                if (keyCode == k_Controls_ATTACK)  { m_Flags |= Mask::Attack; }

            }

            if( pInputEvent->GetDeviceState() == fw::InputEvent::DeviceState::Released )
            {
                if (keyCode == k_Controls_UP   || keyCode == VK_UP) { m_Flags &= ~Mask::Up; }
                if (keyCode == k_Controls_LEFT || keyCode == VK_LEFT) { m_Flags &= ~Mask::Left; }
                if (keyCode == k_Controls_DOWN || keyCode == VK_DOWN) { m_Flags &= ~Mask::Down; }
                if (keyCode == k_Controls_RIGHT|| keyCode == VK_RIGHT) { m_Flags &= ~Mask::Right;}

                if (keyCode == k_Controls_RESTART) { m_Flags &= ~Mask::Restart; }
                if (keyCode == k_Controls_ATTACK)  { m_Flags &= ~Mask::Attack; }

            }
        }
            else if (pInputEvent->GetDeviceType() == fw::InputEvent::DeviceType::Mouse)
        {
            if (pInputEvent->GetDeviceState() == fw::InputEvent::DeviceState::ScrollDown)
            {
                m_Flags |= Mask::ZoomOut;
            }
            if (pInputEvent->GetDeviceState() == fw::InputEvent::DeviceState::ScrollUp)
            {
                m_Flags |= Mask::ZoomIn;
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
    if(mask != Mask::ZoomIn && mask != Mask::ZoomOut) // Operate normally if requested mask isn't scroll data
    {
        return ((m_Flags & mask) != 0) && ((m_OldFlags & mask) == 0);
    } 
    else // If requested mask IS scroll data, turn that bit off after reading it.
    {
        bool to_return = ((m_Flags & mask) != 0) && ((m_OldFlags & mask) == 0);
        m_Flags &= ~(unsigned int)mask;
        return to_return;
    }
}

bool PlayerController::WasNewlyReleased(Mask mask)
{
    return ((m_Flags & mask) == 0) && ((m_OldFlags & mask) != 0);
}