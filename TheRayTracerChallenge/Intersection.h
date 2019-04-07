#pragma once

#include <vector>
#include <memory>
#include "Sphere.h"
#include "Constants.h"
#include <algorithm>

class Intersection {
    
public:
    //Value of parameter 't' at the intersection
    float t;
    std::shared_ptr<Sphere> object;

    Intersection();
    Intersection(float t, std::shared_ptr<Sphere> object);
    ~Intersection() = default;


    bool operator==(Intersection i);
};

inline Intersection::Intersection() {
    object = nullptr;
}

inline Intersection::Intersection(float t, std::shared_ptr<Sphere> object) {
    this->t = t;
    this->object = object;
}

inline bool Intersection::operator==(Intersection i) {
    return Constants::FloatEqual(t, i.t) && (object == i.object);
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