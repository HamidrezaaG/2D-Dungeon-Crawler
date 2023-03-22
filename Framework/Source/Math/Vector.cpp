#include "Vector.h"
#include <cmath>
#include <cassert>
#include "Math.h"

namespace fw {




    Vector2::Vector2() : x(0), y(0) {}
    Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}
    Vector2::Vector2(const Vector2& _v) : x(_v.x), y(_v.y) {}
    Vector2::Vector2(const float &_f) : x(_f), y(_f) {}
    Vector2::Vector2(const IVector2& _v) : x((float)_v.x), y((float)_v.y) {}

    Vector2 Vector2::operator+(const Vector2& _v) const {return Vector2(x + _v.x, y + _v.y);}
    Vector2  Vector2::operator-(const Vector2& _v) const { return Vector2(x - _v.x, y - _v.y); }
    Vector2& Vector2::operator+=(const Vector2& _v) { x += _v.x; y += _v.y; return *this;}
    Vector2& Vector2::operator-=(const Vector2& _v) { x -= _v.x; y -= _v.y; return *this;}

    Vector2& Vector2::operator=(const Vector2& _v) { x = _v.x; y = _v.y; return *this; }
    Vector2 Vector2::operator+(float _val) const{return Vector2(x + _val, y + _val);}
    Vector2 Vector2::operator-(float _val) const{return Vector2(x - _val, y - _val);}
    Vector2 Vector2::operator*(float _val) const{return Vector2(x * _val, y * _val);}
    Vector2 Vector2::operator/(float _val) const{ return Vector2(x / _val, y / _val);}
    Vector2& Vector2::operator+=(float _val) { x += _val; y += _val; return *this;}
    Vector2& Vector2::operator-=(float _val) { x -= _val; y -= _val; return *this;}
    Vector2& Vector2::operator*=(float _val) { x *= _val; y *= _val; return *this;}
    Vector2& Vector2::operator/=(float _val) { x /= _val; y /= _val; return *this;}

    //bool Vector2::operator==(const Vector2& _v) const
    //{
    //    Vector2 difference = *this - _v;
    //    return difference.x >= 0.0001f && difference.y >= 0.0001f;
    //}
    //
    //bool Vector2::operator!=(const Vector2& _v) const
    //{
    //    Vector2 difference = *this - _v;
    //    return difference.x <= 0.0001f && difference.y <= 0.0001f;
    //}

    float Vector2::Dot(const Vector2& v) const { return x * v.x + y * v.y; }

    float Vector2::AngleBetween(const Vector2& v) const
    {
        float dotproduct = Normalized().Dot(v.Normalized());
        float alpha = acos(dotproduct);

        return alpha;
    }

    float Vector2::GetAngle() const
    {
        float alpha = atan2f(y, x);

        if (alpha < 0)
            alpha += (float)(2 * k_PI);

        return alpha;
    }

    float Vector2::DistanceFrom(const Vector2& _v) const
    {
        return (*this - _v).Magnitude();
    }

    float Vector2::SqrDistanceFrom(const Vector2& _v) const
    {
        return (*this - _v).SqrMagnitude();
    }

    Vector2 Vector2::Ortho() const
    {
        return Vector2(y,-x);
    }

    void Vector2::RemoveDirection(const Vector2& _v) 
    {
        if (Dot(_v) == 0)
            return;
        *this -= _v.Normalized() * Dot(_v);
    }

    void Vector2::IsolateDirection(const Vector2& _v)
    {
        if (Dot(_v) == 0)
            return;
        *this = _v.Normalized() * Dot(_v);
    }

    Vector2 Vector2::RemovedDirection(const Vector2& _v) const
    {
        if (Dot(_v) == 0)
            return *this;

        Vector2 to_return = *this;
        to_return -= _v.Normalized() * Dot(_v);
        return to_return;
    }
    
    Vector2 Vector2::IsolatedDirection(const Vector2& _v) const
    {
        if (Dot(_v) == 0)
            return *this;

        return _v.Normalized() * Dot(_v);
    }

    Vector2 Vector2::SnappedToNearestAxis() const
    {
            float ax = abs(x);
            float ay = abs(y);

            if (ax > ay) {
                return Vector2(x > 0 ? 1.0f : -1.0f, 0.0f);
            }
            else {
                return Vector2(0.0f, y > 0 ? 1.0f : -1.0f);
            }
    }

    float Vector2::Magnitude() const { return sqrtf(x * x + y * y); }
    float Vector2::SqrMagnitude() const { return x * x + y * y; }
    void Vector2::Set(float _x, float _y) { x = _x; y = _y; }

    Vector2 Vector2::Normalized() const
    {
        Vector2 v = *this;
        if (!IsZero())
            v /= Magnitude();
        return v;
    }

    void Vector2::Normalize()
    {
        if (!IsZero())
            (*this) /= Magnitude();
    }

    bool Vector2::IsZero() const
    {
        return x == 0 && y == 0;
    }


    Vector2 Vector2::Lerp(const Vector2& _from, const Vector2& _to, const float& _percent) 
    {
        //if (_percent < 0)
        //    _percent = 0;
        //
        //if (_percent > 1)
        //    _percent = 1;
        //
        return _from + (_to - _from) * _percent;
    }

    float Vector2::operator[](int i) const
    {
        assert(i > 0 && i <= 1);
        return *(&x+i);
    }

    void Vector2::ClampLength(float _max)
    {
        if (SqrMagnitude() > _max * _max)
        {
            double angle = atan2f(y, x);
            x = (float)(_max * cos(angle));
            y = (float)(_max * sin(angle));
        }
    }

    Vector2 operator*(float scalar, const Vector2& vec) { return Vector2(scalar * vec.x, scalar * vec.y); };
    Vector2 Vector2::operator-() { return Vector2(-x, -y); };
}
namespace fw {

    Vector3::Vector3() :x(0), y(0), z(0) {}
    Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3::Vector3(const Vector3& _v) : x(_v.x), y(_v.y), z(_v.z) {}

    Vector3 Vector3::Normalized() const
    {
        return Vector3();
    }

    Vector3& Vector3::operator=(const Vector3& _v)
    {
        x = _v.x;
        y = _v.y;
        z = _v.z;
        return *this;
    }
    Vector3 Vector3::operator+(const Vector3& _v) const
    {
        return Vector3(x + _v.x, y + _v.y, z + _v.z);
    }
    Vector3 Vector3::operator-(const Vector3& _v) const
    {
        return Vector3(x - _v.x, y - _v.y, z - _v.z);
    }
    Vector3& Vector3::operator+=(const Vector3& _v)
    {
        x += _v.x;
        y += _v.y;
        z += _v.z;
        return *this;
    }
    Vector3& Vector3::operator-=(const Vector3& _v)
    {
        x -= _v.x;
        y -= _v.y;
        z -= _v.z;
        return *this;
    }
    Vector3 Vector3::operator+(float _val) const
    {
        return Vector3(x + _val, y + _val, z + _val);
    }
    Vector3 Vector3::operator-(float _val) const
    {
        return Vector3(x - _val, y - _val, z - _val);
    }
    Vector3 Vector3::operator*(float _val) const
    {
        return Vector3(x * _val, y * _val, z * _val);
    }
    Vector3 Vector3::operator/(float _val) const
    {
        return Vector3(x / _val, y / _val, z / _val);
    }
    Vector3& Vector3::operator+=(float _val)
    {
        x += _val;
        y += _val;
        z += _val;
        return *this;
    }
    Vector3& Vector3::operator-=(float _val)
    {
        x -= _val;
        y -= _val;
        z -= _val;
        return *this;
    }
    Vector3& Vector3::operator*=(float _val)
    {
        x *= _val;
        y *= _val;
        z *= _val;
        return *this;
    }
    Vector3& Vector3::operator/=(float _val)
    {
        x /= _val;
        y /= _val;
        z /= _val;
        return *this;
    }

    //bool Vector3::operator==(const Vector3& _v) const
    //{
    //    Vector3 difference = *this - _v;
    //    return difference.x <= 0.0001f && difference.y <= 0.0001f && difference.z <= 0.0001f;
    //}
    //
    //bool Vector3::operator!=(const Vector3& _v) const
    //{
    //    Vector3 difference = *this - _v;
    //    return difference.x > 0.0001f && difference.y > 0.0001f && difference.z > 0.0001f;
    //}

    float Vector3::DistanceFrom(const Vector3& _v) const
    {
        return (*this-_v).Magnitude();
    }

    float Vector3::Dot(Vector3 _v) const
    {
        return x * _v.x + y * _v.y;
    }
    float Vector3::Magnitude() const
    {
        return sqrt(x * x + y * y + z * z);
    }
    float Vector3::SqrMagnitude() const
    {
        return x * x + y * y + z * z;
    }
    void Vector3::Set(float _x, float _y, float _z)
    {
        x = _x; y = _y; z = _z;
    }
    void Vector3::ClampLength(float _max)
    {
        if (SqrMagnitude() > _max * _max)
        {
            Normalize();
            *this *= _max;
        }
    }
    void Vector3::Normalize()
    {
        if (!IsZero())
            *this /= Magnitude();
    }
    bool Vector3::IsZero() const
    {
        return x == 0 && y == 0 && z == 0;
    }
}

namespace fw {

    Vector4::Vector4() :x(0), y(0), z(0), w(0) {}
    Vector4::Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {};
    Vector4::Vector4(Vector3 _color, float _alpha) : x(_color.x), y(_color.y), z(_color.z), w(_alpha) {};
    Vector4::Vector4(const Vector4& _v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w){}

    Vector4  Vector4::operator+ ( const Vector4& _v ) const { return Vector4(x + _v.x, y + _v.y, z + _v.z, w + _v.w); }
    Vector4  Vector4::operator- ( const Vector4& _v ) const { return Vector4(x - _v.x, y - _v.y, z - _v.z, w - _v.w); }
    Vector4& Vector4::operator= ( const Vector4& _v )       { x  = _v.x; y  = _v.y; z  = _v.z; w  = _v.w; return *this; }
    Vector4& Vector4::operator+=( const Vector4& _v )       { x += _v.x; y += _v.y; z += _v.z; w += _v.w; return *this; }
    Vector4& Vector4::operator-=( const Vector4& _v )       { x -= _v.x; y -= _v.y; z -= _v.z; w -= _v.w; return *this; }
    Vector4  Vector4::operator+ ( float _val )        const { return Vector4(x + _val, y + _val, z + _val, w + _val); }
    Vector4  Vector4::operator- ( float _val )        const { return Vector4(x -  _val, y -  _val, z -  _val, w -  _val); }
    Vector4  Vector4::operator* ( float _val )        const { return Vector4(x * _val, y * _val, z * _val, w * _val); }
    Vector4  Vector4::operator/ ( float _val )        const { return Vector4(x /  _val, y /  _val, z /  _val, w /  _val); }
    Vector4& Vector4::operator+=( float _val )              { x += _val; y += _val; z += _val; w += _val; return *this; }
    Vector4& Vector4::operator-=( float _val )              { x -= _val; y -= _val; z -= _val; w -= _val; return *this; }
    Vector4& Vector4::operator*=( float _val )              { x *= _val; y *= _val; z *= _val; w *= _val; return *this; }
    Vector4& Vector4::operator/=( float _val )              { x /= _val; y /= _val; z /= _val; w /= _val; return *this; }
    
    //bool     Vector4::operator==( const Vector4& _v ) const { return x == _v.x && y == _v.y && z == _v.z && w == _v.w; }
    //bool     Vector4::operator!=( const Vector4& _v ) const { return x != _v.x || y != _v.y || z != _v.z || w != _v.w; }

    void     Vector4::Set       (float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
    bool Vector4::IsZero() const { return x == 0 && y == 0 && z == 0 && w == 0; }
}

namespace fw {
    IVector2::IVector2() : x(0), y(0) {};
    IVector2::IVector2(int _x, int _y) : x(_x), y(_y) {};
    IVector2::IVector2(const IVector2& _v) : x(_v.x), y(_v.y) {};
    IVector2::IVector2(const Vector2& _v) : x((int)_v.x), y((int)_v.y) {};

    IVector2  IVector2::operator+ (const IVector2& _v) const { return IVector2(x - _v.x, y - _v.y); };
    IVector2  IVector2::operator- (const IVector2& _v) const { return IVector2(x - _v.x, y - _v.y); };
    IVector2& IVector2::operator= (const IVector2& _v) { x = _v.x;  y = _v.y;  return *this; };
    IVector2& IVector2::operator+=(const IVector2& _v) { x += _v.x; y -= _v.y; return *this; };
    IVector2& IVector2::operator-=(const IVector2& _v) { x += _v.x; y -= _v.y; return *this; };
    IVector2  IVector2::operator+ (float _val) const   { return Vector2(x + _val, y + _val); };
    IVector2  IVector2::operator- (float _val) const   { return Vector2(x - _val, y - _val); };
    IVector2  IVector2::operator* (float _val) const   { return Vector2(x * _val, y * _val); };

    bool IVector2::operator==(const IVector2& _v) { return x == _v.x && y == _v.y; };
    bool IVector2::operator!=(const IVector2& _v) { return x != _v.x || y != _v.y; };

    Vector2 IVector2::operator/(float _val) const { return Vector2(x / _val, y / _val); }

    void IVector2::Set(int _x, int _y) { x = _x; y = _y; };
    bool IVector2::IsZero() const { return (x + y == 0); }
}