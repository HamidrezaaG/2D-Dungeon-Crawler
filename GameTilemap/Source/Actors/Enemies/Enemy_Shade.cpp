#include "GamePCH.h"
#include "Enemy_Shade.h"
#include "TileMaps/Pathfinder.h"
#include "TileMaps/TileMap.h"
#include <time.h>
#include "Actors/Player.h"
#include "Events/GameEvents.h"

Enemy_Shade::Enemy_Shade(IVector2 _gridposition, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Pathfinder* _pPathfinder, Player* _pplayer, fw::GameCore* _pgame) : Enemy(_pmesh, _name, _pshader, _pplayer, _pgame)
{
    m_pPathfinder = _pPathfinder;
    m_pTileMap = m_pPathfinder->GetTilemap();

    m_Position = m_pTileMap->ToWorldSpace(_gridposition);
    m_ThisStep = _gridposition;
    m_NextStep = _gridposition;
    m_FinalStep = _gridposition;
    time_t t;
    srand((unsigned)time(&t));


}

void Enemy_Shade::Die()
{
    m_IsDead = true;
    m_EState = k_State::Dying;
}

void Enemy_Shade::TakeDamage(fw::GameObject* instigator, int damage)
{
        m_Velocity += (GetPosition() - instigator->GetCollider()->GetRelativePosition()).Normalized() * k_Shade_Kickback_Intensity;
    
    if (m_EState != k_State::Hurting && m_EState != k_State::Dying)
    {
        //SetColor(GetColor() + Color(1.0f, 0, 0, 0));
        m_EState = k_State::Hurting;
        Enemy::TakeDamage(instigator, damage);
        m_CanMove = true;
    }
}

fw::Collider* Enemy_Shade::GetCollider()
{
    return m_pCollider;
}

Enemy_Shade::~Enemy_Shade()
{
    delete m_pCollider;
}

void Enemy_Shade::SetBehavior(k_Behavior b)
{
    if (m_ECurrentBehavior == b)
        return;
    m_PathList.clear();
    m_ECurrentBehavior = b;
}

void Enemy_Shade::Update(float deltaTime)
{
    UpdateAnimation(deltaTime);
    m_DistanceToPlayer = GetPosition().SqrDistanceFrom(m_pPlayer->GetPosition());

    ImGui::Begin("Monsters");
    std::string status = "Health: %d | Currently " ;
    if (m_EState == k_State::Dying)
    {
        m_Health = 0;
        m_ECurrentAnimation = k_Animation::Idle;
        status += "Dying";

        m_DeathTimer += deltaTime;
        m_Color = Color::Red();

        SetScale(1 - (m_DeathTimer / (k_Shade_DeathRemovalTime)));
        if (m_DeathTimer >= k_Shade_DeathRemovalTime)
        {
            m_pGameCore->GetEventManager()->AddEvent(new RemoveFromGameEvent(this));
        }

    }
    else if (m_EState == k_State::Hurting)
    {
        m_HurtTimer += deltaTime;
        status += "Hurting";


        if (m_HurtTimer >= k_Shade_HurtTime)
        {
            SetColorAlpha(1.0f);
            m_EState = k_State::Idle;
            m_HurtTimer = 0;
        }
        else {
            sin(m_HurtTimer * 60) <= 0 ? SetColorAlpha(1.0f) : SetColorAlpha(0.2f);
        }
    }
    else
    {
        SetColorAlpha(1.0f);
        switch (m_ECurrentBehavior)
        {
        case Enemy_Shade::k_Behavior::Hunt:
        {
            status += "Hunting";

            if (m_DistanceToPlayer > m_HuntRange)
            {
                m_ECurrentBehavior = k_Behavior::Wander;
                m_PathList.clear();
            }

            if (m_DistanceToPlayer < m_AttackRange)
            {
                status += " and Attacking";
                if (m_Anim_FrameNumber == 10)
                {
                    float a = m_pPlayer->GetPosition().DistanceFrom(GetPosition());
                    if (a < 0.85f)
                    {
                        m_pGameCore->GetEventManager()->AddEvent(new PlayerDamageEvent(this, m_ShadeDamage));
                    }
                }

                if (m_ECurrentAnimation != k_Animation::Attack)
                    Attack();
            }

            if (m_Position.SqrDistanceFrom(m_pTileMap->ToWorldSpace(m_NextStep)) < 0.1f)
            {
                if (TryGetPathTo(m_pTileMap->ToGridSpace(m_pPlayer->GetPosition())))
                {
                    IncrementStep();
                }
                else {
                    m_PathList.clear();
                    m_ECurrentBehavior = k_Behavior::Wander;
                    break;
                }
            }
        }
        break;

        case Enemy_Shade::k_Behavior::Wander:
        {
            status += "Wandering";

            if (m_DistanceToPlayer < m_HuntRange)
            {
                m_ECurrentBehavior = k_Behavior::Hunt;
            }

            m_ECurrentAnimation = k_Animation::Idle;

            if (m_Position.SqrDistanceFrom(m_pTileMap->ToWorldSpace(m_NextStep)) < 0.1f)
            {
                if (m_PathList.size() == 0)
                {
                    GetRandomPath();
                };
                IncrementStep();
            }
        }
        break;

        default:
            break;
        };

    }
    ImGui::Text(status.c_str(), m_Health);
    ImGui::End();
    
    //if (m_DistanceToPlayer < m_HuntRange && TryGetPathTo(m_pTileMap->ToGridSpace(m_pPlayer->GetPosition())))
    //{
    //    SetBehavior(k_Behavior::Hunt);
    //}
    //else {
    //    SetBehavior(k_Behavior::Wander);
    //}

    if(m_CanMove)
    { 
    m_Acceleration = Vector2(m_pTileMap->ToWorldSpace(m_NextStep) - m_Position).Normalized() * k_Shade_Acceleration;
    m_Velocity += m_Acceleration * deltaTime;
    m_Velocity.ClampLength(k_Shade_MaxSpeed);
    m_Velocity = Vector2::Lerp(m_Velocity, Vector2::Zero(), deltaTime / k_Shade_FrictionCoefficient);
    m_Position += m_Velocity * deltaTime;
    }
}

void Enemy_Shade::Attack()
{
    m_PathList.clear();
    m_CanMove = false;
    m_ECurrentAnimation = k_Animation::Attack;
    m_Anim_FrameTimer = 0;
    m_EState = k_State::Attacking;
}



void Enemy_Shade::GetRandomPath()
{
    bool success = false;

    do // Get a path that's traversible
    {
        m_FinalStep = IVector2(rand() % m_pTileMap->m_LayoutDimensions.x, rand() % m_pTileMap->m_LayoutDimensions.y);
        success = TryGetPathTo(m_FinalStep);
    } 
    while (!success);
}

bool Enemy_Shade::TryGetPathTo(IVector2 _targetPos)
{
    IVector2 thisTile = m_pTileMap->ToGridSpace(GetPosition());

    if(m_pPathfinder->FindPath(thisTile.x, thisTile.y, _targetPos.x, _targetPos.y)) // Check if the path is traversible
    {
        m_FinalStep = _targetPos;
        m_pPathfinder->GetPath(path, 255, m_FinalStep.x, m_FinalStep.y);
        
        unsigned int length = m_pPathfinder->GetPath(nullptr, 255, m_FinalStep.x, m_FinalStep.y);
        m_PathList.resize(length);

        for (size_t i = 0; i < length; i++)
        {
            m_PathList.at(i) = IVector2(m_pTileMap->IndexToGrid(path[length - i - 1]));
        }

        return true;
    } 
    else
    {
        return false;
    }
}

void Enemy_Shade::IncrementStep() {
    
    unsigned int NumSteps = (unsigned int)m_PathList.size();

    IVector2 FutureStep;

    if (NumSteps > 0)
    {
        m_ThisStep = m_PathList.at(0);
    }

    if (NumSteps > 1)
    {
        m_NextStep = m_PathList.at(1);
    }
    else
    {
        m_NextStep = m_ThisStep;
    }

    if (NumSteps > 2)
    {
        FutureStep = m_PathList.at(2);
    }
    else
    {
        FutureStep = m_ThisStep;
    }

    if (k_AI_DiagonalMovementAllowed && FutureStep != m_ThisStep)
    {
        bool isStraightMovement = ( abs(m_ThisStep.x - FutureStep.x) == 2 || abs(m_ThisStep.y - FutureStep.y) == 2 );

        if (!isStraightMovement)
        { 
            m_NextStep = FutureStep;
            m_PathList.erase(m_PathList.begin()+1);
        }
    }

    m_PathList.erase(m_PathList.begin());

}


void Enemy_Shade::SetSpriteSheet(fw::SpriteSheet* _pspritesheet)
{
	m_pSpriteSheet = _pspritesheet;
}

void Enemy_Shade::SetSprite(fw::SpriteSheet::SpriteInfo* _spriteinfo)
{
    m_UVOffset = _spriteinfo->UV_Offset;
    m_UVScale = _spriteinfo->UV_Scale;
}

void Enemy_Shade::UpdateAnimation(float _deltaTime)
{
    Vector2 DirectionVec;

    if (m_ECurrentAnimation == k_Animation::Idle)
    {
        DirectionVec = m_Velocity;
    }
    else if (m_ECurrentAnimation == k_Animation::Attack)
    {
        DirectionVec = m_pPlayer->GetPosition() - GetPosition();
    }

        if (abs(DirectionVec.x) > abs(DirectionVec.y))
        {
            DirectionVec.x > 0 ? m_ECurrentDirection = k_Direction::Right : m_ECurrentDirection = k_Direction::Left;
        }
        else
        {
            DirectionVec.y > 0 ? m_ECurrentDirection = k_Direction::Up : m_ECurrentDirection = k_Direction::Down;
        }

    std::string FrameName =
        "Shade_"
        +
        k_AnimationNames[(int)m_ECurrentAnimation]   // WHAT ANIMATION
        + "_"
        + k_DirectionNames[(int)m_ECurrentDirection] // WHICH DIRECTION
        + "_"
        + (m_Anim_FrameNumber < 10 ? "0" : "")  // PAD THE 0s
        + std::to_string(m_Anim_FrameNumber);   // ADD FRAME NUM

    SetSprite(m_pSpriteSheet->Sprites[FrameName]);  // SET SPRITE

    if (m_ECurrentAnimation == k_Animation::Attack)
    {
        m_Anim_FrameRate = m_Anim_AttackFrameRate;
    }
    else if (m_ECurrentAnimation == k_Animation::Idle)
    {
        m_Anim_FrameRate = m_Velocity.Magnitude() * 3 + (float)m_Anim_IdleFrameRate - 4;
        
        if (m_ECurrentBehavior == k_Behavior::Hunt)
        m_Anim_FrameRate += 30;

    }

    m_Anim_FrameTimer += m_Anim_FrameRate * _deltaTime;

    if (m_Anim_FrameTimer > m_Anim_IdleFrameCount)
    {
        if (m_ECurrentAnimation == k_Animation::Attack)
        {
            m_ECurrentAnimation = k_Animation::Idle;
            if(m_EState != k_State::Dying)
            m_EState = k_State::Idle;
            m_CanMove = true;
        }

        m_Anim_FrameTimer = 0;
    }

    m_Anim_FrameNumber = (int)m_Anim_FrameTimer % (m_Anim_IdleFrameCount);
    //ImGui::Text("%s",FrameName.c_str());


}

