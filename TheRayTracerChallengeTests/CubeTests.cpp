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
#include <Cube.h>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace RayTracer;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(CubeTests)
	{
	public:

		TEST_METHOD(IsShape) {
			Assert::IsTrue(std::is_base_of<Shape, Cube>());
		}

		TEST_METHOD(SurfaceNormal) {
			Cube cube;

			std::vector<std::pair<Point, Vector>> testData = {
				//Input, Expected output
				{Point::CreatePoint(1.0, 0.5, -0.8), Vector::CreateVector(1.0, 0.0, 0.0) },
				{Point::CreatePoint(-1.0, -0.2, 0.9), Vector::CreateVector(-1.0, 0.0, 0.0) },
				{Point::CreatePoint(-0.4, 1.0, -0.1), Vector::CreateVector(0.0, 1.0, 0.0) },
				{Point::CreatePoint(0.3, -1.0, -0.7), Vector::CreateVector(0.0, -1.0, 0.0) },
				{Point::CreatePoint(-0.6, 0.3, 1.0), Vector::CreateVector(0.0, 0.0, 1.0) },
				{Point::CreatePoint(0.4, 0.4, -1.0), Vector::CreateVector(0.0, 0.0, -1.0) },
				{Point::CreatePoint(1.0, 1.0, 1.0), Vector::CreateVector(1.0, 0.0, 0.0) },
				{Point::CreatePoint(-1.0, -1.0, -1.0), Vector::CreateVector(-1.0, 0.0, 0.0) }
			};


			for (auto test : testData) {
				Intersection i;
				Assert::IsTrue(cube.SurfaceNormal(test.first, i) == test.second);
			}

		}

		TEST_METHOD(CubeIntersections) {
			auto cube = Shape::MakeShared<Cube>();
			size_t id = 3;

			std::vector<Ray> rays = {
				//Rays that hit the cube
				Ray(Point::CreatePoint(5.0, 0.5, 0.0), Vector::CreateVector(-1.0, 0.0, 0.0)),
				Ray(Point::CreatePoint(-5.0, 0.5, 0.0), Vector::CreateVector(1.0, 0.0, 0.0)),
				Ray(Point::CreatePoint(0.5, 5.0, 0.0), Vector::CreateVector(0.0, -1.0, 0.0)),
				Ray(Point::CreatePoint(0.5, -5.0, 0.0), Vector::CreateVector(0.0, 1.0, 0.0)),
				Ray(Point::CreatePoint(0.5, 0.0, 5.0), Vector::CreateVector(0.0, 0.0, -1.0)),
				Ray(Point::CreatePoint(0.5, 0.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0)),
				Ray(Point::CreatePoint(0.0, 0.5, 0.0), Vector::CreateVector(0.0, 0.0, 1.0)),

				//Rays that miss the cube
				Ray(Point::CreatePoint(-2.0, 0.0, 0.0), Vector::CreateVector(0.2673, 0.5345, 0.8018)),
				Ray(Point::CreatePoint(0.0, -2.0, 0.0), Vector::CreateVector(0.8018, 0.2673, 0.5345)),
				Ray(Point::CreatePoint(0.0, 0.0, -2.0), Vector::CreateVector(0.5345, 0.8018, 0.2673)),
				Ray(Point::CreatePoint(2.0, 0.0, 2.0), Vector::CreateVector(0.0, 0.0, -1.0)),
				Ray(Point::CreatePoint(0.0, 2.0, 2.0), Vector::CreateVector(0.0, -1.0, 0.0)),
				Ray(Point::CreatePoint(2.0, 2.0, 0.0), Vector::CreateVector(-1.0, 0.0, 0.0))
			};

			std::vector<IntersectionBuffer> expected = {
				//Expected results for rays hitting the cube
				IntersectionBuffer(Intersection(4.0, cube), Intersection(6.0, cube)),
				IntersectionBuffer(Intersection(4.0, cube), Intersection(6.0, cube)),
				IntersectionBuffer(Intersection(4.0, cube), Intersection(6.0, cube)),
				IntersectionBuffer(Intersection(4.0, cube), Intersection(6.0, cube)),
				IntersectionBuffer(Intersection(4.0, cube), Intersection(6.0, cube)),
				IntersectionBuffer(Intersection(4.0, cube), Intersection(6.0, cube)),
				IntersectionBuffer(Intersection(-1.0, cube), Intersection(1.0, cube)),

				//Results for rays that miss the cube
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer()
			};

			for (size_t testNr = 0; testNr < expected.size(); testNr++) {
				Ray testRay = rays[testNr];
				IntersectionBuffer expectedIntersections = expected[testNr];
				IntersectionBuffer intersections;
				cube->FindIntersections(testRay, intersections);


				//Make sure the correct number of hits was found
				Assert::IsTrue(intersections.GetCount() == expectedIntersections.GetCount());

				//If there were any intersections, make sure that they were calculated correctly
				if (intersections.GetCount() != 0) {
					Assert::IsTrue(intersections[0].t == expectedIntersections[0].t);
					Assert::IsTrue(intersections[0].shape == expectedIntersections[0].shape);

					Assert::IsTrue(intersections[1].t == expectedIntersections[1].t);
					Assert::IsTrue(intersections[1].shape == expectedIntersections[1].shape);
				}
			}
		}

		TEST_METHOD(Bounds) {
			auto s = Shape::MakeShared<Cube>();
			auto bounds = s->GetObjectSpaceBounds();

			auto max = bounds.GetMax();
			auto min = bounds.GetMin();

			Assert::IsTrue(max == Point::CreatePoint(1.0, 1.0, 1.0));
			Assert::IsTrue(min == Point::CreatePoint(-1.0, -1.0, -1.0));
		}

	};
}
