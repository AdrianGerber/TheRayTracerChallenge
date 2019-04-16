#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <IntersectionBuffer.h>
#include <Transform.h>
#include <memory>
#include <Shape.h>
#include <Plane.h>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(PlaneTests)
	{
	public:

		TEST_METHOD(IsShape) {
			Assert::IsTrue(std::is_base_of<Shape, Plane>());
		}
		
		TEST_METHOD(SurfaceNormal) {
			Plane p;
			//Normal vector should be the same at every point
			Assert::IsTrue(
				p.FindObjectSpaceNormal(Point::CreatePoint(0.0, 0.0, 0.0))
				==
				Vector::CreateVector(0.0, 1.0, 0.0)
			);
			Assert::IsTrue(
				p.FindObjectSpaceNormal(Point::CreatePoint(10.0, 0.0, -10.0))
				==
				Vector::CreateVector(0.0, 1.0, 0.0)
			);
			Assert::IsTrue(
				p.FindObjectSpaceNormal(Point::CreatePoint(-5.0, 0.0, 150.0))
				==
				Vector::CreateVector(0.0, 1.0, 0.0)
			);
		}

		TEST_METHOD(PlaneIntersection1) {
			//Ray is parallel to the plane
			Plane p;
			Ray r(Point::CreatePoint(0.0, 10.0, 0.0), Vector::CreateVector(0.0, 0.0, 1.0));
			IntersectionBuffer xs = p.FindObjectSpaceIntersections(r);
			Assert::IsTrue(xs.GetCount() == 0);
		}
		TEST_METHOD(PlaneIntersection2) {
			//Ray is coplanar to the plane
			Plane p;
			Ray r(Point::CreatePoint(0.0, 0.0, 0.0), Vector::CreateVector(0.0, 0.0, 1.0));
			IntersectionBuffer xs = p.FindObjectSpaceIntersections(r);
			Assert::IsTrue(xs.GetCount() == 0);
		}
		TEST_METHOD(PlaneIntersection3) {
			//Ray hits plane from above
			Plane p;
			p.SetID(12);

			Ray r(Point::CreatePoint(0.0, 1.0, 0.0), Vector::CreateVector(0.0, -1.0, 0.0));
			IntersectionBuffer xs = p.FindObjectSpaceIntersections(r);
			Assert::IsTrue(xs.GetCount() == 1);
			Assert::IsTrue(Constants::DoubleEqual(xs[0].t, 1.0));
			Assert::IsTrue(xs[0].objectID == p.GetID());
		}
		TEST_METHOD(PlaneIntersection4) {
			//Ray hits plane from below
			Plane p;
			p.SetID(12);

			Ray r(Point::CreatePoint(0.0, -1.0, 0.0), Vector::CreateVector(0.0, 1.0, 0.0));
			IntersectionBuffer xs = p.FindObjectSpaceIntersections(r);
			Assert::IsTrue(xs.GetCount() == 1);
			Assert::IsTrue(Constants::DoubleEqual(xs[0].t, 1.0));
			Assert::IsTrue(xs[0].objectID == p.GetID());
		}
	};
}
