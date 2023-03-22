#pragma once

namespace fw {

class Event
{

    friend class EventManager;
public:
    Event();
    virtual ~Event();

    virtual const char* GetType() = 0;

protected:

    void SetDelay(float delay) { m_Delay = delay; }
    float m_Delay = 0.0f;
};

class InputEvent : public Event
{
public:
    enum class DeviceType
    {
        Keyboard,
        Mouse
    };

    enum class DeviceState
    {
        Pressed,
        Released,
        ScrollUp,
        ScrollDown,
    };

    InputEvent(DeviceType deviceType, DeviceState deviceState, unsigned int keyCode)
    {
        m_DeviceType = deviceType;
        m_DeviceState = deviceState;
        m_KeyCode = keyCode;
    }

    virtual ~InputEvent() {}

    static const char* GetStaticEventType() { return "InputEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    DeviceType GetDeviceType() { return m_DeviceType; }
    DeviceState GetDeviceState() { return m_DeviceState; }
    unsigned int GetKeyCode() { return m_KeyCode; }

protected:
    DeviceType m_DeviceType;
    DeviceState m_DeviceState;
    unsigned int m_KeyCode;
};

} // namespace fw
