#pragma once
#include <cmath>

struct Vector2{
    float x = 0.0f;
    float y = 0.0f;

    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y){}

    Vector2 operator+(const Vector2 other){
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator+=(const Vector2 other){
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2 operator*(float scalar) const{
        return Vector2(x * scalar, y * scalar);
    }

    float length() const{
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalized(){
        float len = length();
        if(len > 0.0f){
            return Vector2(x / len, y / len);
        }
        return Vector2(0.0f, 0.0f);
    }
};