#pragma once
#include "Math/Vector.h"

namespace fw {

    class Camera {

    public:

        Camera(Vector2 _position, Vector2 _dimensions, float _zoom);

        Vector2 GetPosition();
        void SetPosition(Vector2 _position);

        Vector2 GetDimensions();
        void SetDimensions(Vector2 _dimensions);

        Vector2 GetCorrectedPosition();
        void SetCorrectedPosition(Vector2 _position);

        float GetZoom();
        void SetZoom(float _zoom);

    private:
        Vector2 m_Position;
        Vector2 m_Dimensions;
        float m_CameraZoom;
    };
} 
