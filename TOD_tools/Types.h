#pragma once

template <typename T>
struct Vector2 {
	T	x;
	T	y;
};

typedef Vector2<int> ScreenResolution;

template <typename T>
struct Vector3 {
	T	x;
	T	y;
	T	z;
};

template <typename T>
struct Vector4 {
	T	x;
	T	y;
	T	z;
	T	a;
};

struct ColorRGB {
	float	r;
	float	g;
	float	b;
	float	a;
};

typedef Vector4<float> Vector4f;

template<typename T>
struct Quaternion {
	T	w;
	T	x;
	T	y;
	T	z;
};

typedef Quaternion<float> Orientation;