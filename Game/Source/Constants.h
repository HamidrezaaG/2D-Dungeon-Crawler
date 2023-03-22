#include "../../Framework/Source/Utility/Color.h"

const char k_Controls_UP = 'W';
const char k_Controls_LEFT = 'A';
const char k_Controls_DOWN = 'S';
const char k_Controls_RIGHT = 'D';
const char k_Controls_RESTART = 'R';

const bool  k_Game_ShowDebug = false;
const float k_Game_TransitionTime = 1.5f;
const float k_Counter_InitScale = 1.0f;
const float k_Counter_FinScale = 2.0f;

const float k_World_Gravity = 20.0f;

const float k_Arena_MinRadius = 0.05f;
const float k_Arena_PlayRadius = 4.0f;
const float k_Arena_MenuRadius = 2.0f;

const float k_Arena_MaxRadius = 5.0f;
const int   k_Arena_VertCount = 72;

const float k_Player_MinRadius = 0.25f;
const float k_Player_DefaultRadius = 0.5f;
const float k_Player_Acceleration = 30;
const float k_Player_MaxSpeed = 50;
const float k_Player_FrictionCoefficient = 1 - 0.01f;
const float k_Player_HurtTimer = 0.15f;
const float k_Player_HurtBumpPower = 1.0f;
const int   k_Player_DefaultVertCount = 6;
const int   k_Player_Health = 6;

const int   k_Player_Trail_Count = 6;
const float k_Player_Trail_BaseRadius = 0.5f;
const float k_Player_Trail_Radius_pow = 0.9f;
const float k_Player_Trail_BaseLerpMultiplier = 40.0f;
const float k_Player_Trail_LerpMultiplier_pow = 0.8f;

const bool  k_Enemy_IsFilled = true;
const int   k_Enemy_VertCount = 4;
const float k_Enemy_ShapeStartingAngle = 45.0f;
const float k_Enemy_Radius = 0.30f;
const float k_Enemy_Speed = 5.0f;
const float k_Enemy_SpawnTime = 0.5f;
const float k_Enemy_MinRotSpeedDeg = 450.0f;
const float k_Enemy_MaxRotSpeedDeg = 1350.0f;

const Color k_Color_Background = Color(0, 0.3f, 0.3f, 1);

const Color k_Color_Player = Color::Cyan();
const float k_Color_Player_DeadAlpha = 0.6f;
const Color k_Color_PlayerHurt = Color::Red();
const Color k_Color_Player_Trail = Color(0, 1, 1, 0.8f);

const Color k_Color_Arena = Color::White();
const Color k_Color_SimpleEnemy = Color(1, 0.2f, 0, 1);
const Color k_Color_BoomerangEnemy = Color(1, 0.9f, 0, 1.0f);
const Color k_Color_Counter = Color::White();

