#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <IntersectionBuffer.h>
#include <Transform.h>
#include <memory>
#include <string>
#include <Shape.h>
#include <Triangle.h>
#include <BoundingBox.h>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(TriangleTests)
	{


	public:

		TEST_METHOD(IsShape) {
			Assert::IsTrue(std::is_base_of<Shape, Triangle>());
		}

		TEST_METHOD(Creation) {
			auto p0 = Point::CreatePoint(0.0, 1.0, 0.0);
			auto p1 = Point::CreatePoint(-1.0, 0.0, 0.0);
			auto p2 = Point::CreatePoint(1.0, 0.0, 0.0);

			Triangle triangle(p0, p1, p2);

			Assert::IsTrue(triangle.GetPoint(0) == p0);
			Assert::IsTrue(triangle.GetPoint(1) == p1);
			Assert::IsTrue(triangle.GetPoint(2) == p2);

			Assert::IsTrue(triangle.GetEdge(0) == Vector::CreateVector(-1.0, -1.0, 0.0));
			Assert::IsTrue(triangle.GetEdge(1) == Vector::CreateVector(1.0, -1.0, 0.0));
			Assert::IsTrue(triangle.FindObjectSpaceNormal(p0) == Vector::CreateVector(0.0, 0.0, -1.0));
		}
		TEST_METHOD(BoundingBox) {
			Triangle triangle(
				Point::CreatePoint(-3.0, 7.0, 2.0),
				Point::CreatePoint(6.0, 2.0, -4.0),
				Point::CreatePoint(2.0, -1.0, -1.0)
			);

			auto bounds = triangle.GetObjectSpaceBounds();
			Assert::IsTrue(bounds.GetMin() == Point::CreatePoint(-3.0, -1.0, -4.0));
			Assert::IsTrue(bounds.GetMax() == Point::CreatePoint(6.0, 7.0, 2.0));
		}

		TEST_METHOD(RayIntersections) {
			auto triangle = Shape::MakeShared<Triangle>(
				Point::CreatePoint(0.0, 1.0, 0.0),
				Point::CreatePoint(-1.0, 0.0, 0.0),
				Point::CreatePoint(1.0, 0.0, 0.0)
			);

			std::vector <std::pair<Ray, IntersectionBuffer>> tests = {
				//Ray parallel to triangle
				{Ray(Point::CreatePoint(0.0, 1.0, -2.0), Vector::CreateVector(0.0, 1.0, 0.0)), IntersectionBuffer()},
				//Ray misses p0-p2 edge
				{Ray(Point::CreatePoint(1.0, 1.0, -2.0), Vector::CreateVector(0.0, 0.0, 1.0)), IntersectionBuffer()},
				//Ray misses p0-p1 edge
				{Ray(Point::CreatePoint(-1.0, 1.0, -2.0), Vector::CreateVector(0.0, 0.0, 1.0)), IntersectionBuffer()},
				//Ray misses p1-p2 edge
				{Ray(Point::CreatePoint(0.0, -1.0, -2.0), Vector::CreateVector(0.0, 0.0, 1.0)), IntersectionBuffer()},
				//Ray hits the triangle
				{Ray(Point::CreatePoint(0.0, 0.5, -2.0), Vector::CreateVector(0.0, 0.0, 1.0)), IntersectionBuffer(Intersection(2.0, triangle))}
			};

			for (auto& test : tests) {
				auto ray = test.first;
				auto expected = test.second;
				IntersectionBuffer result;

				triangle->FindObjectSpaceIntersections(ray, result);

				Assert::IsTrue(expected.GetCount() == result.GetCount());

				if (expected.GetCount() > 0) {
					for (size_t i = 0; i < expected.GetCount(); i++) {
						Assert::IsTrue(expected[i] == result[i]);
					}
				}
			}
		}
	};
}
