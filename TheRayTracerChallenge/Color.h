#pragma once

#include "Constants.h"

namespace RayTracer {
	class Color
	{
	public:
		double r, g, b;

		Color();
		Color(double r, double g, double b);
		~Color() = default;

		//Operator overloads
		bool operator== (Color c);
		bool operator!= (Color c);
		Color operator+(Color c);
		Color operator- (Color c);
		Color operator* (double scalar);
		Color operator* (Color c);
	};

	inline Color::Color()
	{
		r = g = b = 0.0f;
	}

	inline Color::Color(double r, double g, double b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	inline bool Color::operator==(Color c)
	{
		return
			Constants::DoubleEqual(r, c.r)
			&& Constants::DoubleEqual(g, c.g)
			&& Constants::DoubleEqual(b, c.b);
	}

	inline bool Color::operator!=(Color c)
	{
		return !(*this == c);
	}

	inline Color Color::operator+(Color c)
	{
		return Color(r + c.r, g + c.g, b + c.b);
	}

	inline Color Color::operator-(Color c)
	{
		return Color(r - c.r, g - c.g, b - c.b);
	}

	inline Color Color::operator*(double scalar)
	{
		return Color(
			r * scalar,
			g * scalar,
			b * scalar
		);
	}

	//Calculate the hadamard product
	inline Color Color::operator*(Color c)
	{
		return Color(
			r * c.r,
			g * c.g,
			b * c.b
		);
	}
}