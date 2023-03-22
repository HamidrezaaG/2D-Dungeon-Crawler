#pragma once

#include "../../Framework/Source/Utility/Color.h"
//#include "../../Framework/Source/Objects/Camera.h"

const char k_Controls_UP = 'W';
const char k_Controls_LEFT = 'A';
const char k_Controls_DOWN = 'S';
const char k_Controls_RIGHT = 'D';
const char k_Controls_RESTART = 'R';
const char k_Controls_ATTACK = 32;

const float k_World_Gravity = 20.0f;

const float k_Player_Acceleration = 12;
const float k_Player_MaxSpeed = 8;
const float k_Player_FrictionCoefficient = 1 - 0.5f;

const int   k_Player_Anim_FrameCount = 11;
const float k_Player_Anim_DefaultFrameRate = 15.0f;
const float k_Player_Anim_VelocityIdleThreshold = 0.3f;
const float k_Player_Anim_RunFramerateBase = 3.0f;
const float k_Player_Anim_RunFramerateMultiplier = 7.0f;

const float k_Player_Kickback_Intensity = 1.0f;
const float k_Player_InvincibilityTime = 1.0f;
const int  k_Player_MaxHealth = 5;
const int  k_Player_AttackDamage = 1;

const float k_Shade_MaxSpeed = 7.0f;
const float k_Shade_Acceleration = 3.0f;
const float k_Shade_FrictionCoefficient = 0.6f;
const int   k_Shade_MaxHealth = 3;
const float k_Shade_Kickback_Intensity = 3.f;
const float k_Shade_DeathRemovalTime = 1.0f;
const float k_Shade_HurtTime = 1.25f;


const bool k_AI_DiagonalMovementAllowed = true;

const fw::Vector2 k_LayoutOffset = fw::Vector2(0.0f, 0.f);

const Color k_Color_Player = Color::Cyan();
const Color k_Color_Background = Color(0.2f, 0.2f, 0.2f, 1);

const float k_Camera_Speed = 5.0f;
const float k_Camera_ZoomSpeed = 10.0f;
const float k_Camera_ZoomStep = 0.1f;
const float k_Camera_MaxZoom = 2.0f;
const float k_Camera_MinZoom = 0.1f;

