#pragma once

#include "Intersection.h"
#include <algorithm>

//Stores an arbitrary number of intersections
class IntersectionBuffer {
public:
	//Constructors with initial elements
    IntersectionBuffer(Intersection i);
    IntersectionBuffer(Intersection i1, Intersection i2);
	IntersectionBuffer(Intersection i1, Intersection i2, Intersection i3, Intersection i4);

	IntersectionBuffer() { IsSorted = true; }
    ~IntersectionBuffer() = default;

	//Add one or more intersections
    void Add(Intersection intersection);
    void Add(IntersectionBuffer intersections);

	//Access individual intersections
    Intersection operator[](size_t i) const;
    size_t GetCount();
	void Reset() { IsSorted = false; intersections.clear(); }

    void Sort();

	//Find the first 'hit' (Intersection with smallest, positive 't value')
    const Intersection GetFirstHit();

private:
    bool IsSorted;
    std::vector<Intersection> intersections;
};


inline IntersectionBuffer::IntersectionBuffer(Intersection i) : intersections{ i } {
    IsSorted = true;
}
inline IntersectionBuffer::IntersectionBuffer(Intersection i1, Intersection i2) : intersections{ i1, i2 } {
    IsSorted = false;
}

inline IntersectionBuffer::IntersectionBuffer(Intersection i1, Intersection i2, Intersection i3, Intersection i4) : intersections{ i1, i2, i3, i4 } {
	IsSorted = false;
}

inline void IntersectionBuffer::Add(Intersection intersection) {
    intersections.push_back(intersection);

	//List of intersections is no longer sorted
    IsSorted = false;
}

inline void IntersectionBuffer::Add(IntersectionBuffer intersections) {
    size_t count = intersections.GetCount();

    for (size_t i = 0; i < count; i++) {
        Add(intersections[i]);
    }

	//List of intersections is no longer sorted
    IsSorted = false;
}

inline Intersection IntersectionBuffer::operator[] (size_t i) const {
    return intersections[i];
}

inline size_t IntersectionBuffer::GetCount() {
    return intersections.size();
}

inline void IntersectionBuffer::Sort() {
	//Sort the list if it is not already sorted
    if(!IsSorted) std::sort(intersections.begin(), intersections.end());
    IsSorted = true;
}

inline const Intersection IntersectionBuffer::GetFirstHit() {
    if (!intersections.size()) return Intersection();

    //Sort in ascending order of t
    Sort();

    //Find lowest, non negative t value
    for (const auto& i : intersections) {
        if (i.t >= 0.0f) return i;
    }
    
    //No valid intersection found
    return Intersection();
}