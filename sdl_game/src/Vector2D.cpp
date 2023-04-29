#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D(float x_, float y_): x(x_), y(y_) {}

Vector2D::Vector2D(int x_, int y_):
    x(static_cast<float>(x_)),
    y(static_cast<float>(y_))
{
}

void Vector2D::add(const Vector2D& other) {
	x += other.x;
	y += other.y;
}

void Vector2D::sub(const Vector2D& other){
	x -= other.x;
	y -= other.y;
}

void Vector2D::mult(float k) {
	x *= k;
	y *= k;
}

void Vector2D::normalize() {
    //if not null vector
    if (x || y)
        mult(1/length());
}

float Vector2D::length() {
    return std::sqrt(x*x + y*y);
}

void Vector2D::rotate(float angle) {
    float sin = std::sin(angle);
    float cos = std::cos(angle);
    float x0 = x;
    float y0 = y;
    x = x0 * cos - y0 * sin;
    y = x0 * sin + y0 * cos;
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
	add(other);
	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other){
	sub(other);
	return *this;
}

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs) {
	Vector2D tmp{lhs};
	tmp += rhs;
	return tmp;
}

Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs){
	Vector2D tmp{lhs};
	tmp -= rhs;
	return tmp;
}

Vector2D operator*(const Vector2D& lhs, float rhs) {
	Vector2D tmp{lhs};
	tmp.mult(rhs);
	return tmp;
}
	
Vector2D operator*(float lhs, const Vector2D& rhs) {
    return rhs * lhs;
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
	os << "(" << vec.x << "," << vec.y << ")";
	return os;
}	
