#pragma once

#include "Math/Vector.h"
#include "Utility/Color.h"

namespace fw {

    const IVector2 k_Window_Dimensions = fw::Vector2(1440, 720);
    const float k_Window_Aspect = k_Window_Dimensions.x / k_Window_Dimensions.y;
    const float k_World_Scale = 10.0f;
    const Vector2 k_World_MidPoint = fw::Vector2(k_World_Scale / 2, k_World_Scale / 2);

    const Vector2 PositionRoundingFactor = k_Window_Dimensions / k_World_Scale;
    const Color k_Debug_Collider_Color = Color::Orange();

    const bool k_Debug_DrawColliders = true;
    
}
