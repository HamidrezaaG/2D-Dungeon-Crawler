#include "GamePCH.h"
#include "Player.h"
#include "Arena.h"
#include "PlayerController.h"
#include "../Events/GameEvents.h"

Player::Player(Vector2 position, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, PlayerController* _pcontroller, Arena* _pArena, fw::GameCore* _pgame) : GameObject(position, _pmesh, _name, _pshader, _pgame)
{
    m_pFramework = GameObject::m_pGameCore->GetFramework();
    m_pController = _pcontroller;

    m_pCollider = new fw::CircleCollider(this, m_Radius); 
    m_pCollider->Enable();

    m_pFlashTimer = new fw::Timer(k_Player_HurtTimer, false);
    m_pArena = _pArena;
}

Player::~Player()
{
    delete m_pCollider;
    delete m_pFlashTimer;
}

void Player::Update(float deltaTime)
{
    if (m_pFlashTimer->IsDone())
    {
        m_pFlashTimer->Reset();
    }

    if (m_pFlashTimer->IsRunning() && IsAlive())
    {
        m_pFlashTimer->Update(deltaTime);
        m_Color = k_Color_PlayerHurt + (k_Color_Player - k_Color_PlayerHurt) * m_pFlashTimer->GetPercentageElapsed();
    }


    m_Acceleration.Set(0, 0);

    if(m_pController->IsHeld(PlayerController::Mask::Up))
        m_Acceleration.y += k_Player_Acceleration;
    if(m_pController->IsHeld(PlayerController::Mask::Down))
        m_Acceleration.y -= k_Player_Acceleration;
    if(m_pController->IsHeld(PlayerController::Mask::Left))
        m_Acceleration.x -= k_Player_Acceleration;
    if(m_pController->IsHeld(PlayerController::Mask::Right))
        m_Acceleration.x += k_Player_Acceleration;

    if (!IsAlive())
    {
        m_Acceleration += Vector2::Down() * k_World_Gravity;
    }

    m_Velocity += m_Acceleration * deltaTime;
    //if(m_Acceleration.IsZero())
    m_Velocity *= k_Player_FrictionCoefficient;
    m_Velocity.ClampLength(k_Player_MaxSpeed);

    // Bounce off walls
    if((m_Position.x < 0 && m_Velocity.x < 0) || (m_Position.x > fw::k_World_Scale) && m_Velocity.x > 0)
        m_Velocity.x *= -1;
    if ((m_Position.y < 0 && m_Velocity.y < 0) || (m_Position.y > fw::k_World_Scale) && m_Velocity.y > 0)
        m_Velocity.y *= -1;


    //ImGui::Text("%f", m_Velocity.Magnitude());
    //m_Velocity += Vector2::Down() * 10 * deltaTime;

    if (m_ArenaCollision)
    {
        m_Velocity.RemoveDirection(m_wallDir);
        m_Velocity += m_pArena->GetCalculatedVelocity().IsolatedDirection(m_wallDir);
        //Vector2 v = m_pArena->GetCalculatedVelocity();
        //v.IsolateDirection(m_wallDir);
        
        m_ArenaCollision = false;
    }

    m_Position += m_Velocity * deltaTime;

}

void Player::SetCircle(float _radius, int _vertCount, bool _filled)
{
    m_Radius = _radius;
    m_VertCount = _vertCount;
    m_IsFilled = _filled;

    m_pMesh->MakeCircle(m_Radius, m_VertCount, m_IsFilled);
}

fw::k_ObjectType Player::GetType()
{
    return fw::k_ObjectType::Player;
}

void Player::RemoveAccelerationInDirection(Vector2 _direction)
{
    m_ArenaCollision = true; 
    m_wallDir = _direction;
}
float Player::GetRadius()
{
    return m_Radius;
}

fw::Collider* Player::GetCollider()
{
    return m_pCollider;
}

void Player::Reset() {
    m_pFlashTimer->Reset();
    m_Health = k_Player_Health;
    m_pCollider->Enable();
    m_Color = k_Color_Player;
}

void Player::Reset(float percent) {

    m_pFlashTimer->Reset();

    if (m_Health != k_Player_Health)
        m_ResetColorStartPoint = m_Color;
    else {
        m_ResetColorStartPoint = m_Color;
    }
    m_Health = k_Player_Health;
    m_pCollider->Enable();
    m_Color = k_Color_Player - (k_Color_Player - m_ResetColorStartPoint) * (1.0f-percent);
}


void Player::OnCollisionHit(GameObject* _pOtherObject)
{
    if (_pOtherObject->GetCollider()->GetType() == fw::k_ColliderType::Circle)
    {
        m_Color = k_Color_PlayerHurt;

        fw::CircleCollider* otherCollider = static_cast<fw::CircleCollider*>(_pOtherObject->GetCollider());
        Vector2 dir = (m_Position - _pOtherObject->GetPosition()).Normalized();

        m_Position = _pOtherObject->GetPosition() + dir * (m_pCollider->GetRadius() + otherCollider->GetRadius()) ;

        m_Velocity.RemoveDirection(-dir);
        m_Velocity += dir * k_Player_HurtBumpPower;

        _pOtherObject->GetCollider()->Disable();

        m_Health--;
        m_pGameCore->GetEventManager()->AddEvent(new UpdatePlayerHealthEvent());

        if (m_Health == 0)
        {
            
            m_Color = k_Color_PlayerHurt;
            m_Color.SetAlpha(k_Color_Player_DeadAlpha);
            m_pCollider->Disable();
        }
        else {
            m_pFlashTimer->Restart();
        }
    }
}

int Player::GetHealth()
{
    return m_Health;
}

void Player::SetHealth(int h)
{
    m_Health = h;
}

bool Player::IsAlive()
{
    return m_Health > 0;
}
