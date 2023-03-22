#include "Camera.h"
#include "Framework.h"
namespace fw {

    Camera::Camera(Vector2 _position, Vector2 _dimensions, float _zoom) : m_Position(_position), m_Dimensions(_dimensions), m_CameraZoom(_zoom) {}



    Vector2 Camera::GetDimensions()
    {
        return m_Dimensions;
    }
    void Camera::SetDimensions(Vector2 _dimensions)
    {
        m_Dimensions = _dimensions;
    }

    Vector2 Camera::GetPosition()
    {
        return m_Position;
    }
    void Camera::SetPosition(Vector2 _position)
    {
        m_Position = _position;
    }

    float Camera::GetZoom()
    {
        return m_CameraZoom;
    }
    void Camera::SetZoom(float _zoom)
    {
        m_CameraZoom = _zoom;
    }

    Vector2 Camera::GetCorrectedPosition()
    {
        return m_Position + m_Dimensions * 0.5f;
    }
    void Camera::SetCorrectedPosition(Vector2 _position)
    {
        m_Position = _position - m_Dimensions * 0.5f - Vector2(0.5f, 0);
    }
}