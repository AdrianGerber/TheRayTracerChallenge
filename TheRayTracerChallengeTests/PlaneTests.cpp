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
#include <limits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace RayTracer;

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
			Intersection i;
			Assert::IsTrue(
				p.FindObjectSpaceNormal(Point::CreatePoint(0.0, 0.0, 0.0), i)
				==
				Vector::CreateVector(0.0, 1.0, 0.0)
			);
			Assert::IsTrue(
				p.FindObjectSpaceNormal(Point::CreatePoint(10.0, 0.0, -10.0), i)
				==
				Vector::CreateVector(0.0, 1.0, 0.0)
			);
			Assert::IsTrue(
				p.FindObjectSpaceNormal(Point::CreatePoint(-5.0, 0.0, 150.0), i)
				==
				Vector::CreateVector(0.0, 1.0, 0.0)
			);
		}

		TEST_METHOD(PlaneIntersection1) {
			//Ray is parallel to the plane
			Plane p;
			Ray r(Point::CreatePoint(0.0, 10.0, 0.0), Vector::CreateVector(0.0, 0.0, 1.0));
			IntersectionBuffer xs;
			p.FindObjectSpaceIntersections(r, xs);
			Assert::IsTrue(xs.GetCount() == 0);
		}
		TEST_METHOD(PlaneIntersection2) {
			//Ray is coplanar to the plane
			Plane p;
			Ray r(Point::CreatePoint(0.0, 0.0, 0.0), Vector::CreateVector(0.0, 0.0, 1.0));
			IntersectionBuffer xs;
			p.FindObjectSpaceIntersections(r, xs);
			Assert::IsTrue(xs.GetCount() == 0);
		}
		TEST_METHOD(PlaneIntersection3) {
			//Ray hits plane from above
			auto p = Shape::MakeShared<Plane>();

			Ray r(Point::CreatePoint(0.0, 1.0, 0.0), Vector::CreateVector(0.0, -1.0, 0.0));
			IntersectionBuffer xs;
			p->FindObjectSpaceIntersections(r, xs);
			Assert::IsTrue(xs.GetCount() == 1);
			Assert::IsTrue(Constants::DoubleEqual(xs[0].t, 1.0));
			Assert::IsTrue(xs[0].shape == p);
		}
		TEST_METHOD(PlaneIntersection4) {
			//Ray hits plane from below
			auto p = Shape::MakeShared<Plane>();

			Ray r(Point::CreatePoint(0.0, -1.0, 0.0), Vector::CreateVector(0.0, 1.0, 0.0));
			IntersectionBuffer xs;
			p->FindObjectSpaceIntersections(r, xs);
			Assert::IsTrue(xs.GetCount() == 1);
			Assert::IsTrue(Constants::DoubleEqual(xs[0].t, 1.0));
			Assert::IsTrue(xs[0].shape == p);
		}

		TEST_METHOD(Bounds) {
			auto s = Shape::MakeShared<Plane>();
			auto bounds = s->GetObjectSpaceBounds();

			auto max = bounds.GetMax();
			auto min = bounds.GetMin();

			Assert::IsTrue(std::isinf(min.x) && min.x < 0.0);
			Assert::IsTrue(std::isinf(min.z) && min.z < 0.0);
			Assert::IsTrue(Constants::DoubleEqual(min.y, 0.0));

			Assert::IsTrue(std::isinf(max.x) && max.x > 0.0);
			Assert::IsTrue(std::isinf(max.z) && max.z > 0.0);
			Assert::IsTrue(Constants::DoubleEqual(max.y, 0.0));
		}
	};
}
