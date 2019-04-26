#pragma once

#include <vector>
#include <memory>
#include "Constants.h"

class Shape;

//Type used to store the intersection of a ray and a shape
class Intersection {
    
public:
	//Shape that was hit
	std::shared_ptr<Shape> shape;

    //Value of parameter 't' at the intersection
	double t;

    Intersection();
    Intersection(double t, std::shared_ptr<Shape> shapePointer);
    ~Intersection() = default;

	bool IsValid() const { return shape != nullptr; }
};

inline Intersection::Intersection() {
	t = 0.0f;
}

inline Intersection::Intersection(double t, std::shared_ptr<Shape> shapePointer) {
    this->t = t;
	this->shape = shapePointer;
}

inline bool operator== (const Intersection i1, const Intersection i2) {
    return Constants::DoubleEqual(i1.t, i2.t) && (i1.shape == i2.shape);
}


inline bool operator>(const Intersection i1, const Intersection i2) {
    return i1.t > i2.t;
}

inline bool operator<(const Intersection i1, const Intersection i2) {
    return i1.t < i2.t;
}
