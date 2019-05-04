#pragma once

#include <vector>
#include <memory>
#include "Constants.h"

namespace RayTracer {
	class Shape;

	//Type used to store the intersection of a ray and a shape
	class Intersection {

	public:
		//Shape that was hit
		std::shared_ptr<Shape> shape;

		//Value of parameter 't' at the intersection
		double t;

		//Hit positions for triangles, undefined for other shapes
		double u, v;
		bool hitPositionSet;

		Intersection();
		Intersection(double t, std::shared_ptr<Shape> shapePointer);
		Intersection(double t, std::shared_ptr <Shape> shapePointer, double u, double v);
		~Intersection() = default;

		bool IsValid() const { return shape != nullptr; }
	};

	inline Intersection::Intersection() {
		t = 0.0f;
		u = 0.0;
		v = 0.0;
		hitPositionSet = false;
	}

	inline Intersection::Intersection(double t, std::shared_ptr<Shape> shapePointer) {
		this->t = t;
		this->shape = shapePointer;
		u = 0.0;
		v = 0.0;
		hitPositionSet = false;
	}

	inline Intersection::Intersection(double t, std::shared_ptr <Shape> shapePointer, double u, double v) {
		this->t = t;
		this->shape = shapePointer;
		this->u = u;
		this->v = v;
		hitPositionSet = true;
	}

	inline bool operator== (const Intersection i1, const Intersection i2) {
		bool equal =
			Constants::DoubleEqual(i1.t, i2.t)
			&& (i1.shape == i2.shape)
			&& i1.hitPositionSet == i2.hitPositionSet;


		//Also compare position of the hit, if it was set
		if (i1.hitPositionSet) {
			equal == equal
				&& Constants::DoubleEqual(i1.u, i2.u)
				&& Constants::DoubleEqual(i1.v, i2.v);
		}

		return equal;
	}


	inline bool operator>(const Intersection i1, const Intersection i2) {
		return i1.t > i2.t;
	}

	inline bool operator<(const Intersection i1, const Intersection i2) {
		return i1.t < i2.t;
	}
}