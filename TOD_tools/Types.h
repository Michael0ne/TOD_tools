#pragma once

template <typename T>
struct Vector2 {
	T	x;
	T	y;

	Vector2(T vx, T vy)
		: x(vx), y(vy)
	{}

	Vector2()
		: x(0), y(0)
	{}
};

typedef Vector2<int> ScreenResolution;
typedef Vector2<float> Vector2f;

template <typename T>
struct Vector3 {
	T	x;
	T	y;
	T	z;

	Vector3(T vx, T vy, T vz)
		: x(vx), y(vy), z(vz)
	{}

	Vector3()
		: x(0), y(0), z(0)
	{}
};

typedef Vector3<float> Vector3f;

template <typename T>
struct Vector4 {
	T	x;
	T	y;
	T	z;
	T	a;

	Vector4(T vx, T vy, T vz, T va)
		: x(vx), y(vy), z(vz), a(va)
	{}

	Vector4()
		: x(0), y(0), z(0), a(0)
	{}
};

struct ColorRGB {
	float	r;
	float	g;
	float	b;
	float	a;

	ColorRGB(float vr, float vg, float vb, float va)
		: r(vr), g(vg), b(vb), a(va)
	{}

	ColorRGB(float vr, float vg, float vb)
		: r(vr), g(vg), b(vb), a(0)
	{}

	ColorRGB()
		: r(0), g(0), b(0), a(0)
	{}
};

typedef Vector4<float> Vector4f;

template<typename T>
struct Quaternion {
	T	w;
	T	x;
	T	y;
	T	z;

	Quaternion(T vw, T vx, T vy, T vz)
		: w(vw), x(vx), y(vy), z(vz)
	{}

	Quaternion(T vx, T vy, T vz)
		: w(0), x(vx), y(vy), z(vz)
	{}

	Quaternion()
		: w(0), x(0), y(0), z(0)
	{}
};

typedef Quaternion<float> Orientation;

template <typename K, typename V>
struct Map {
	K	key;
	V	value;

	Map* next;

	Map()
		: key(NULL), value(NULL), next(nullptr)
	{}
};