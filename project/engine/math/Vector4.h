#pragma once

struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	// += ���Z�q�̃I�[�o�[���[�h
	Vector4& operator+=(const Vector4& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->w += other.w;
		return *this;
	}

	// + ���Z�q�̃I�[�o�[���[�h
	friend Vector4 operator+(const Vector4& lhs, const Vector4& rhs) {
		Vector4 result = lhs;
		result += rhs;
		return result;
	}

	// -= ���Z�q�̃I�[�o�[���[�h
	Vector4& operator-=(const Vector4& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->w -= other.w;
		return *this;
	}

	// - ���Z�q�̃I�[�o�[���[�h
	friend Vector4 operator-(const Vector4& lhs, const Vector4& rhs) {
		Vector4 result = lhs;
		result -= rhs;
		return result;
	}

	// *= ���Z�q�̃I�[�o�[���[�h
	Vector4& operator*=(float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;
		return *this;
	}

	// * ���Z�q�̃I�[�o�[���[�h
	friend Vector4 operator*(const Vector4& vec, float scalar) {
		Vector4 result = vec;
		result *= scalar;
		return result;
	}

	// * ���Z�q�̃I�[�o�[���[�h
	friend Vector4 operator*(float scalar, const Vector4& vec) {
		return vec * scalar;
	}

	// /= ���Z�q�̃I�[�o�[���[�h
	Vector4& operator/=(float scalar) {
		this->x /= scalar;
		this->y /= scalar;
		this->z /= scalar;
		this->w /= scalar;
		return *this;
	}

	// / ���Z�q�̃I�[�o�[���[�h
	friend Vector4 operator/(const Vector4& vec, float scalar) {
		Vector4 result = vec;
		result /= scalar;
		return result;
	}

	// - ���Z�q�̃I�[�o�[���[�h
	Vector4 operator-() const {
		return *this * -1;
	}

	// == ���Z�q�̃I�[�o�[���[�h
	bool operator==(const Vector4& other) const {
		return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
	}

	// != ���Z�q�̃I�[�o�[���[�h
	bool operator!=(const Vector4& other) const {
		return !(*this == other);
	}

	// �P��+���Z�q�̃I�[�o�[���[�h
	Vector4 operator+() const {
		return *this;
	}

	// �x�N�g���̒������v�Z����֐�
	float Length() const {
		return std::sqrt(LengthSquared());
	}

	// �x�N�g���̒�����2����v�Z����֐�
	float LengthSquared() const {
		return x * x + y * y + z * z + w * w;
	}

	// �x�N�g���𐳋K������֐�
	void Normalize() {
		*this /= Length();
	}

	// �x�N�g���𐳋K������֐�
	Vector4 Normalized() const {
		return *this / Length();
	}

	// �x�N�g���̓��ς��v�Z����֐�
	static float Dot(const Vector4& v1, const Vector4& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	// �x�N�g���̊O�ς��v�Z����֐�
	static Vector4 Cross(const Vector4& v1, const Vector4& v2) {
		Vector4 result;
		result.x = v1.y * v2.z - v1.z * v2.y;
		result.y = v1.z * v2.x - v1.x * v2.z;
		result.z = v1.x * v2.y - v1.y * v2.x;
		result.w = 0;
		return result;
	}

	// �x�N�g���𐳋K������֐�
	static Vector4 Normalize(const Vector4& vec) {
		return vec / vec.Length();
	}


};