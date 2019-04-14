#pragma once

#include <vector>
#include <memory>
#include "Constants.h"

class Intersection {
    
public:
    static const size_t invalidID = std::numeric_limits<size_t>::max();

    //Value of parameter 't' at the intersection
    float t;
    size_t objectID;

    

    Intersection();
    Intersection(float t, size_t objectID);
    ~Intersection() = default;

    bool IsValid() const { return objectID != invalidID; }
};

inline Intersection::Intersection() {
    objectID = invalidID;
}

inline Intersection::Intersection(float t, size_t objectID) {
    this->t = t;
    this->objectID = objectID;
}

inline bool operator== (const Intersection i1, const Intersection i2) {
    return Constants::FloatEqual(i1.t, i2.t) && (i1.objectID == i2.objectID);
}


inline bool operator>(const Intersection i1, const Intersection i2) {
    return i1.t > i2.t;
}

inline bool operator<(const Intersection i1, const Intersection i2) {
    return i1.t < i2.t;
}
