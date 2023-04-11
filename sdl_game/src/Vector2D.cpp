#include "Vector2D.h"

Vector2D::Vector2D(float x_, float y_): x(x_), y(y_) {}

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
