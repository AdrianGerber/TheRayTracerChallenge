#pragma once

#include <vector>
#include <memory>
#include "Constants.h"

//Type used to store the intersection of a ray and a shape
class Intersection {
    
public:
    static const size_t invalidID = std::numeric_limits<size_t>::max();

    //Value of parameter 't' at the intersection
	double t;
	//ID of the object that was hit
    size_t objectID;

    Intersection();
    Intersection(double t, size_t objectID);
    ~Intersection() = default;

    bool IsValid() const { return objectID != invalidID; }
};

inline Intersection::Intersection() {
    objectID = invalidID;
	t = 0.0f;
}

inline Intersection::Intersection(double t, size_t objectID) {
    this->t = t;
    this->objectID = objectID;
}

inline bool operator== (const Intersection i1, const Intersection i2) {
    return Constants::DoubleEqual(i1.t, i2.t) && (i1.objectID == i2.objectID);
}


inline bool operator>(const Intersection i1, const Intersection i2) {
    return i1.t > i2.t;
}

inline bool operator<(const Intersection i1, const Intersection i2) {
    return i1.t < i2.t;
}
