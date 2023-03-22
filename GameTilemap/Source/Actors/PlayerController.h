#pragma once

class PlayerController
{
public:

    enum Mask
    {
        Up      = 1 << 1,
        Down    = 1 << 2,
        Left    = 1 << 3,
        Right   = 1 << 4,
        Restart = 1 << 5,
        Attack  = 1 << 6,
        ZoomIn  = 1 << 7,
        ZoomOut = 1 << 8,
    };

    PlayerController();
    virtual ~PlayerController();

    void StartFrame();
    void OnEvent(fw::Event* pEvent);

    bool IsHeld(Mask mask);
    bool WasNewlyPressed(Mask mask);
    bool WasNewlyReleased(Mask mask);

protected:
    unsigned int m_Flags = 0;
    unsigned int m_OldFlags = 0;

};
