#pragma once

#include <vector>
#include <memory>
#include "Sphere.h"
#include "Constants.h"
#include <algorithm>

class Intersection {
    
public:
    const size_t invalidID = std::numeric_limits<size_t>::max();

    //Value of parameter 't' at the intersection
    float t;
    size_t objectID;

    

    Intersection();
    Intersection(float t, size_t objectID);
    ~Intersection() = default;


    bool operator==(Intersection i);
    bool IsValid() { return objectID != invalidID; }
};

inline Intersection::Intersection() {
    objectID = invalidID;
}

inline Intersection::Intersection(float t, size_t objectID) {
    this->t = t;
    this->objectID = objectID;
}

inline bool Intersection::operator==(Intersection i) {
    return Constants::FloatEqual(t, i.t) && (objectID == i.objectID);
}

class IntersectionBuffer {
public:

    IntersectionBuffer(Intersection i);
    IntersectionBuffer(Intersection i1, Intersection i2);

    IntersectionBuffer() = default;
    ~IntersectionBuffer() = default;

    void Add(Intersection intersection);
    Intersection& operator[] (size_t i);
    size_t GetCount();

    Intersection GetFirstHit();

private:
    std::vector<Intersection> intersections;
};


inline IntersectionBuffer::IntersectionBuffer(Intersection i) : intersections { i }{

}
inline IntersectionBuffer::IntersectionBuffer(Intersection i1, Intersection i2) : intersections{ i1, i2 } {

}

inline void IntersectionBuffer::Add(Intersection intersection) {
    intersections.push_back(intersection);
}

inline Intersection& IntersectionBuffer::operator[] (size_t i) {
    return intersections[i];
}

inline size_t IntersectionBuffer::GetCount() {
    return intersections.size();
}

inline Intersection IntersectionBuffer::GetFirstHit() {

    if (!intersections.size()) return Intersection();

    auto i = std::min_element(intersections.begin(), intersections.end(), [](Intersection& a, Intersection& b) {
        bool aValid = a.t >= 0.0f;
        bool bValid = b.t >= 0.0f;

        if (aValid && bValid) {
            return a.t < b.t;
        }

        if (aValid && !bValid) {
            return true;
        }

        if (!aValid && bValid) {
            return false;
        }

        return false;
    });
    
    if (i->t >= 0.0f) {
        return *i;
    }

    return Intersection();
}