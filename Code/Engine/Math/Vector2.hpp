#pragma once
#include "Engine/Math/MathUtils.hpp"
#include <math.h>

class Vector2Int;
class Vector3;
class Vector4;

class Vector2
{
public:
    //CONSTRUCTORS//////////////////////////////////////////////////////////////////////////
    Vector2();
    explicit Vector2(float initialValue);
    explicit Vector2(float initialX, float initialY);
    Vector2(const Vector2& other);
    explicit Vector2(const Vector3& other);
    explicit Vector2(const Vector4& other);
    explicit Vector2(const Vector2Int& other);
    void SetXY(float newX, float newY);

    //FUNCTIONS//////////////////////////////////////////////////////////////////////////
    float CalculateMagnitudeSquared() const;
    float CalculateMagnitude() const;
    void ClampMagnitude(float topSpeed);
    float CalculateThetaDegrees();
    void Normalize();
    Vector2 GetNorm();
    float Dot(const Vector2& b) const;
    inline float GetDirectionDegrees() const { return RadiansToDegrees(MathUtils::HALF_PI - atan2f(y, x)); };

    //STATIC FUNCTIONS/////////////////////////////////////////////////////////////////////
    static Vector2 CalculateCorrectionVector(const Vector2& position, const Vector2& goal);
    static Vector2 GetMidpoint(const Vector2& start, const Vector2& end);
    static Vector2 DegreesToDirection(float rotationDegrees, float rotationalOffset = ZERO_DEGREES_RIGHT);
    static float Dot(const Vector2& a, const Vector2& b);

    //OPERATORS//////////////////////////////////////////////////////////////////////////
    Vector2& operator+=(const Vector2& rhs);
    Vector2& operator-=(const Vector2& rhs);
    Vector2& operator*=(const Vector2& rhs);
    Vector2& operator/=(const Vector2& rhs);
    Vector2& operator*=(const float& scalarConstant);
    Vector2& operator/=(const float& scalarConstant);
    static Vector2 CreateFromString(const char* xmlString);
    static Vector2 CreateFromPolar(float radius, float degrees);

    //CONSTANTS//////////////////////////////////////////////////////////////////////////
    static const Vector2 ZERO;
    static const Vector2 ONE;
    static const Vector2 UNIT_X;
    static const Vector2 UNIT_Y;
    static const Vector2 MAX;
    static const float ZERO_DEGREES_RIGHT;
    static const float ZERO_DEGREES_UP;

    //MEMBER VARIABLES//////////////////////////////////////////////////////////////////////////
    float x;
    float y;
};

//-----------------------------------------------------------------------------------
inline Vector2 operator-(const Vector2& vecToReverse)
{
    return Vector2(-vecToReverse.x, -vecToReverse.y);
}

//----------------------------------------------------------------------
inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

//----------------------------------------------------------------------
inline Vector2 operator-(Vector2 lhs, const Vector2& rhs)
{
    lhs -= rhs;
    return lhs;
}

//----------------------------------------------------------------------
inline Vector2 operator*(Vector2 lhs, const Vector2& rhs)
{
    lhs *= rhs;
    return lhs;
}

//----------------------------------------------------------------------
inline Vector2 operator/(Vector2 lhs, const Vector2& rhs)
{
    lhs /= rhs;
    return lhs;
}

//----------------------------------------------------------------------
inline Vector2 operator*(float scalarConstant, Vector2 lhs)
{
    lhs *= scalarConstant;
    return lhs;
}

//----------------------------------------------------------------------
inline Vector2 operator*(Vector2 lhs, float scalarConstant)
{
    lhs *= scalarConstant;
    return lhs;
}

//----------------------------------------------------------------------
inline Vector2 operator/(Vector2 lhs, float scalarConstant)
{
    lhs /= scalarConstant;
    return lhs;
}

//----------------------------------------------------------------------
inline bool operator==(const Vector2& lhs, const Vector2& rhs)
{ 
    return (lhs.x == rhs.x) && (lhs.y == rhs.y); 
}

//----------------------------------------------------------------------
inline bool operator!=(const Vector2& lhs, const Vector2& rhs)
{
    return !(lhs == rhs);
}

//-----------------------------------------------------------------------------------
//If this template specialization isn't inlined, you'll have linker errors. You'd have to add it as a definition and then put the body in the .cpp, like a regular function.
template <>
inline Vector2 Min(const Vector2& first, const Vector2& second)
{
    return Vector2(Min(first.x, second.x), Min(first.y, second.y));
}

//-----------------------------------------------------------------------------------
template <>
inline Vector2 Max(const Vector2& first, const Vector2& second)
{
    return Vector2(Max(first.x, second.x), Max(first.y, second.y));
}
