#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <iostream>

class Vector2D {
public:
	float x = 0.0f;
	float y = 0.0f;
	
	Vector2D() {}
	Vector2D(float x_, float y_);
	
	void add(const Vector2D& other);
	void sub(const Vector2D& other);
	void mult(float k);
	
	Vector2D& operator+=(const Vector2D& other);
	Vector2D& operator-=(const Vector2D& other);
};

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs);
Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs);

Vector2D operator*(const Vector2D& lhs, float rhs);
Vector2D operator*(float lhs, const Vector2D& rhs);

std::ostream& operator<<(std::ostream& os, const Vector2D& vec);


#endif // VECTOR2D_H
