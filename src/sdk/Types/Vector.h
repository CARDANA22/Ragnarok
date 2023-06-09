#pragma once
#include <cmath>

struct Vector3 {
    float x, y, z;
    
    inline static float distance(Vector3 lhs, Vector3 rhs) {
        Vector3 delta = Vector3{ rhs.x - lhs.x, rhs.y - lhs.y, rhs.z - lhs.z };
        return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    }
    
    inline Vector3 operator+ (Vector3 v) const {
        return Vector3{ x + v.x, y + v.y, z + v.z };
    }
    
    inline Vector3 operator- (Vector3 v) const {
        return Vector3{ x - v.x, y - v.y, z - v.z };
    }
    
    inline Vector3 operator* (float scale) const {
        return Vector3{ x * scale, y * scale, z * scale };
    }

    void Normalize()
	{
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}
};

struct Vector2 {
    float x, y;
    
    inline static float distance(Vector2 lhs, Vector2 rhs) {
        Vector2 delta = Vector2{ rhs.x - lhs.x, rhs.y - lhs.y };
        return sqrt(delta.x * delta.x + delta.y * delta.y);
    }
    
    inline Vector2 operator+ (Vector2 v) const {
        return Vector2{ x + v.x, y + v.y };
    }
    
    inline Vector2 operator- (Vector2 v) const {
        return Vector2{ x - v.x, y - v.y };
    }
    
    inline Vector2 operator* (float scale) const {
        return Vector2{ x * scale, y * scale };
    }
};