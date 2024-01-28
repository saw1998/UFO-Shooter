//
// Created by Sachin Saw on 23/01/24.
//

#include <cmath>
#include "Vec2.h"

Vec2::Vec2() = default;

Vec2::Vec2(float xin, float yin)
    : x(xin)
    ,y(yin){
}

bool Vec2::operator==(const Vec2 &rhs) const {
    return (x == rhs.x) && (y == rhs.y);
}

bool Vec2::operator!=(const Vec2 &rhs) const {
    return (x != rhs.x) || (y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2 &rhs) const {
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const {
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float val) const {
    return Vec2(x * val, y * val);
}

Vec2 Vec2::operator/(const float val) const {
    return Vec2(x / val, y / val);
}

void Vec2::operator+=(const Vec2 &rhs) {
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs) {
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator*=(const float val) {
    x *= val;
    y *= val;
}

void Vec2::operator/=(const float val) {
    x /= val;
    y /= val;
}

float Vec2::dist(const Vec2 &rhs) const {
    //TODO implement
    return 0;
}

float Vec2::length() const {
    return std::sqrtf(x*x + y*y);
}

void Vec2::normalize() {
    float length = this->length();
    x /= length;
    y /= length;
}










