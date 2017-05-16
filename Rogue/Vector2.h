#pragma once
#include <cmath>

/// 2D向量库，用于实现游戏中的物理效果

class Vector2
{
public:

	Vector2() :m_x(0), m_y(0) {
	}
	Vector2(double x, double y) :m_x(x), m_y(y) {
	}
	Vector2(const Vector2 &v): m_x(v.m_x), m_y(v.m_y){
	}
	const Vector2& operator= (const Vector2 &v) {
		m_x = v.m_x;
		m_y = v.m_y;
		return *this;
	}

	const double getX() const {
		return m_x;
	}

	const double getY() const {
		return m_y;
	}

	void setX(double x) {
		m_x = x;
	}

	void setY(double y) {
		m_y = y;
	}

	double length() const {
		return std::hypotl(m_x, m_y);
	}

	const Vector2 normalized() {
		double l = length();
		if (l > 1e-6) {
			return Vector2(m_x / l, m_y / l);
		} else {
			return *this;
		}
	}

	void normalize() {
		*this = normalized();
	}

	const Vector2 operator + (const Vector2 &rhs) const {
		return Vector2(m_x + rhs.m_x, m_y + rhs.m_y);
	}

	const Vector2& operator += (const Vector2 &rhs) {
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		return *this;
	}

	const Vector2 operator * (const double scalar) const {
		return Vector2(scalar*m_x, scalar*m_y);
	}

	friend Vector2& operator * (const double scalar, const Vector2 &rhs) {
		return Vector2(rhs.m_x*scalar, rhs.m_y*scalar);
	}

	const Vector2& operator *= (const double scalar) {
		m_x *= scalar;
		m_y *= scalar;
		return *this;
	}

	const Vector2 operator - (const Vector2 &rhs) const {
		return Vector2(m_x - rhs.m_x, m_y - rhs.m_y);
	}

	const Vector2& operator -= (const Vector2 &rhs) {
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		return *this;
	}

	friend const Vector2 operator - (const Vector2 &v) {
		return Vector2(-v.m_x, -v.m_y);
	}

	const Vector2 operator / (const double scalar) const {
		return Vector2(m_x / scalar, m_y / scalar);
	}

	const Vector2& operator /= (const double scalar) {
		m_x /= scalar;
		m_y /= scalar;
		return *this;
	}
	const double dot(const Vector2 &v) const {
		return m_x*v.m_x + m_y*v.m_y;
	}

private:
	double m_x;
	double m_y;
};
