 #pragma once

namespace fw {
	const double k_PI = 3.1415926536;
	const double k_Convert_Rad2Deg = 57.2957795131;
	const double k_Convert_Deg2Rad = 0.0174532925;

	class Math {
		public:

			static float Clamp01(const float& f) 
			{
				return f < 0 ? 0 : (f > 1 ? 1 : f);
			};

			static float Clamp(const float& f , const float& min, const float& max) 
			{
				return f < min ? min : (f > max ? max : f);
			};

			static float Lerp(const float& from, const float& to, const float& fraction) 
			{
				float f = Clamp01(fraction);
				return from - (from - to) * f;
			};

	};
} // namespace fw
