#pragma once

namespace fw
{
	class Timer
	{
	public:
		Timer();
		Timer(double duration, bool startTimer);
		Timer(double duration);

		void Update(double delta);

		void Start();
		void Stop();

		void Reset(); //Resets a timer to zero but doesn't start it
		void Restart(); //Resets AND Starts the timer

		void SetDuration(double duration);
		double GetDuration();

		double GetElapsed();
		double GetRemaining();

		float GetPercentageElapsed();
		float GetPercentageRemaining();

		void SetDoesLoop(bool doesLoop);
		bool GetDoesLoop();

		bool IsRunning();
		bool IsDone();

	private:
		double m_Duration;
		double m_Elapsed;
		bool m_IsRunning;
		bool m_DoesLoop;
	};
}