 #pragma once

namespace fw {

	class IVector2;

    class Vector2
    {
    public:

		float x = 0;
		float y = 0;

        Vector2();
        Vector2(float _x, float _y);
		Vector2(const float& _f);
		Vector2(const Vector2& _v);
        Vector2(const IVector2& _v);

		static const Vector2 Vector2::Zero()  { return Vector2( 0,  0); }
		static const Vector2 Vector2::One()   { return Vector2( 1,  1); }

		static const Vector2 Vector2::Up()	  { return Vector2( 0,  1); }
		static const Vector2 Vector2::Down()  { return Vector2( 0, -1); }
		static const Vector2 Vector2::Left()  { return Vector2(-1,  0); }
		static const Vector2 Vector2::Right() { return Vector2( 1,  0); }

		Vector2 Normalized() const;

		Vector2 operator+ (const Vector2& _v) const;
		Vector2 operator- (const Vector2& _v) const;
		Vector2& operator=(const Vector2& _v);
		Vector2& operator+=(const Vector2& _v);
		Vector2& operator-=(const Vector2& _v);

		Vector2 operator+(float _val) const;
		Vector2 operator-(float _val) const;
		Vector2 operator*(float _val) const;
		Vector2 operator/(float _val) const;
		Vector2& operator+=(float _val);
		Vector2& operator-=(float _val);
		Vector2& operator*=(float _val);
		Vector2& operator/=(float _val);

		Vector2 operator-();

		bool operator==(const Vector2& _v) const;
	 	bool operator!=(const Vector2& _v) const;

		float Dot(const Vector2& _v) const;
		float AngleBetween(const Vector2& v) const;

		float GetAngle() const;

		float DistanceFrom(const Vector2& _v) const;
        float SqrDistanceFrom(const Vector2& _v) const;

		Vector2 Ortho() const;


		void RemoveDirection(const Vector2& _v);
		void IsolateDirection(const Vector2& _v);

		Vector2 RemovedDirection(const Vector2& _v) const;
		Vector2 IsolatedDirection(const Vector2& _v) const;
		Vector2 SnappedToNearestAxis() const;

		float Magnitude() const;
		float SqrMagnitude() const;
		void Set(float _x, float _y);
		void ClampLength(float _max);
		void Normalize();
		bool IsZero() const;

		static Vector2 Lerp(const Vector2& _from, const Vector2& _to, const float& _percent);

		float operator[](int i) const;
	};
	class Vector3
	{
	public:

		float x = 0;
		float y = 0;
		float z = 0;

		Vector3();
		Vector3(float _x, float _y, float _z);
		Vector3(const Vector3& _v);

		static const Vector3 Vector3::Zero()	 { return Vector3( 0,  0,  0); }
		static const Vector3 Vector3::One()		 { return Vector3( 1,  1,  1); }

		static const Vector3 Vector3::Up()		 { return Vector3( 0,  1,  0); }
		static const Vector3 Vector3::Down()	 { return Vector3( 0, -1,  0); }
		static const Vector3 Vector3::Left()	 { return Vector3(-1,  0,  0); }
		static const Vector3 Vector3::Right()	 { return Vector3( 1,  0,  0); }
		static const Vector3 Vector3::Forward()  { return Vector3( 0,  0,  1); }
		static const Vector3 Vector3::Backward() { return Vector3( 0,  0, -1); }

		Vector3 Normalized() const;

		Vector3  operator+ (const Vector3& _v) const;
		Vector3  operator- (const Vector3& _v) const;
		Vector3& operator= (const Vector3& _v);
		Vector3& operator+=(const Vector3& _v);
		Vector3& operator-=(const Vector3& _v);

		Vector3 operator+(float _val) const;
		Vector3 operator-(float _val) const;
		Vector3 operator*(float _val) const;
		Vector3 operator/(float _val) const;
		Vector3& operator+=(float _val);
		Vector3& operator-=(float _val);
		Vector3& operator*=(float _val);
		Vector3& operator/=(float _val);

		bool operator==(const Vector3& _v) const;
		bool operator!=(const Vector3& _v) const;

		float DistanceFrom(const Vector3& _v) const;
		float Dot(Vector3 _v) const;
		float Magnitude() const;
		float SqrMagnitude() const;
		void Set(float _x, float _y, float _z);
		void ClampLength(float _max);
		void Normalize();

		bool IsZero() const;
	};
	class Vector4
	{
	public:

			float x;
			float y;
			float z;
			float w;

		Vector4();
		Vector4(float _x, float _y, float _z, float _w);
		Vector4(Vector3 _color, float _alpha);
		Vector4(const Vector4& _v);

		static const Vector4 Vector4::Zero() { return Vector4(); }
		static const Vector4 Vector4::One() { return Vector4(1, 1, 1, 1); }

		Vector4  operator+ (const Vector4& _v) const;
		Vector4  operator- (const Vector4& _v) const;
		Vector4& operator= (const Vector4& _v);
		Vector4& operator+=(const Vector4& _v);
		Vector4& operator-=(const Vector4& _v);

		Vector4 operator+(float _val) const;
		Vector4 operator-(float _val) const;
		Vector4 operator*(float _val) const;
		Vector4 operator/(float _val) const;
		Vector4& operator+=(float _val);
		Vector4& operator-=(float _val);
		Vector4& operator*=(float _val);
		Vector4& operator/=(float _val);

		bool operator==(const Vector4& _v) const;
		bool operator!=(const Vector4& _v) const;

		void Set(float _x, float _y, float _z, float _w);

		bool IsZero() const;
	};

	class IVector2
	{
	public:
		int x = 0;
		int y = 0;

		IVector2();
		IVector2(int _x, int _y);
		IVector2(const IVector2& _v);
		IVector2(const Vector2& _v);

		static const IVector2 IVector2::Zero()	{ return IVector2(0 , 0); }
		static const IVector2 IVector2::One()	{ return IVector2(1 , 1); }

		static const IVector2 IVector2::Up()	{ return IVector2(0 , 1); }
		static const IVector2 IVector2::Down()	{ return IVector2(0 ,-1); }
		static const IVector2 IVector2::Left()	{ return IVector2(-1, 0); }
		static const IVector2 IVector2::Right() { return IVector2(1 , 0); }

		IVector2  operator+ (const IVector2& _v) const;
		IVector2  operator- (const IVector2& _v) const;

		IVector2& operator= (const IVector2& _v);
		IVector2& operator+=(const IVector2& _v);
		IVector2& operator-=(const IVector2& _v);

		IVector2 operator+(float _val) const;
		IVector2 operator-(float _val) const;
		IVector2 operator*(float _val) const;
		 Vector2 operator/(float _val) const;

		bool operator==(const IVector2& _v);
		bool operator!=(const IVector2& _v);

		bool IsZero() const;
		void Set(int _x, int _y);
	};
} // namespace fw
