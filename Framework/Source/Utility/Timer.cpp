#include "FrameworkPCH.h"
#include "Timer.h"

namespace fw
{
	Timer::Timer():
		m_Duration(0.0),
		m_Elapsed(0.0),
		m_IsRunning(false),
		m_DoesLoop(false)
	{
	}

	Timer::Timer(double duration, bool startTimer) :
		m_Duration(duration),
		m_Elapsed(0.0),
		m_IsRunning(startTimer),
		m_DoesLoop(false)
	{
	}

	Timer::Timer(double duration) :
		m_Duration(duration),
		m_Elapsed(0.0),
		m_IsRunning(false),
		m_DoesLoop(false)
	{
	}

	void Timer::Update(double delta)
	{
		if (m_IsRunning == true)
		{
			m_Elapsed += delta;
			if (m_Elapsed > m_Duration)
			{
				if (m_DoesLoop == true)
				{
					m_Elapsed -= m_Duration;
				}
				else
				{
					m_Elapsed = m_Duration;
					m_IsRunning = false;
				}
			}
		}
	}

	void Timer::Start()
	{
		m_IsRunning = true;
	}

	void Timer::Stop()
	{
		m_IsRunning = false;
	}

	void Timer::Reset()
	{
		m_IsRunning = false;
		m_Elapsed = 0.0;
	}

	void Timer::Restart()
	{
		m_IsRunning = true;
		m_Elapsed = 0.0;
	}

	void Timer::SetDuration(double duration)
	{
		m_Duration = duration;
	}

	double Timer::GetDuration()
	{
		return m_Duration;
	}


	double Timer::GetElapsed()
	{
		return m_Elapsed;
	}

	double Timer::GetRemaining()
	{
		return m_Duration - m_Elapsed;
	}

	float Timer::GetPercentageElapsed()
	{
		return (float)(m_Elapsed / m_Duration);
	}

	float Timer::GetPercentageRemaining()
	{
		return 1.0f - GetPercentageElapsed();
	}

	void Timer::SetDoesLoop(bool doesLoop)
	{
		m_DoesLoop = doesLoop;
	}

	bool Timer::GetDoesLoop()
	{
		return m_DoesLoop;
	}

	bool Timer::IsRunning()
	{
		return m_IsRunning;
	}

	bool Timer::IsDone()
	{
		return m_Elapsed == m_Duration;
	}
}