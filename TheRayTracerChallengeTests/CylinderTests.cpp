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
#include <Cylinder.h>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(CylinderTests)
	{
	public:

		TEST_METHOD(IsShape) {
			Assert::IsTrue(std::is_base_of<Shape, Cylinder>());
		}

		TEST_METHOD(CylinderCreation) {
			Cylinder cylinder;

			Assert::IsTrue(std::isinf(cylinder.GetMaximum()) && std::isinf(cylinder.GetMinimum()));
			Assert::IsTrue(cylinder.GetMaximum() > cylinder.GetMinimum());
			Assert::IsTrue(cylinder.IsClosed() == false);
		}
		TEST_METHOD(SurfaceNormal) {

			std::vector<std::tuple<Point, Vector, std::shared_ptr<Cylinder>>> testData = {
				//Input, Expected output, cylinder
				//Normal vectors on the cylinder's walls
				{Point::CreatePoint(1.0, 0.0, 0.0), Vector::CreateVector(1.0, 0.0, 0.0), Shape::MakeShared<Cylinder>() },
				{Point::CreatePoint(0.0, 5.0, -1.0), Vector::CreateVector(0.0, 0.0, -1.0), Shape::MakeShared<Cylinder>() },
				{Point::CreatePoint(0.0, -2.0, 1.0), Vector::CreateVector(0.0, 0.0, 1.0),  Shape::MakeShared<Cylinder>() },
				{Point::CreatePoint(-1.0, 1.0, 0.0), Vector::CreateVector(-1.0, 0.0, 0.0),  Shape::MakeShared<Cylinder>() },
				//Normal vectors on the end caps
				{Point::CreatePoint(0.0, 1.0, 0.0), Vector::CreateVector(0.0, -1.0, 0.0),  Shape::MakeShared<Cylinder>(1.0, 2.0, true) },
				{Point::CreatePoint(0.5, 1.0, 0.0), Vector::CreateVector(0.0, -1.0, 0.0),  Shape::MakeShared<Cylinder>(1.0, 2.0, true) },
				{Point::CreatePoint(0.0, 1.0, 0.5), Vector::CreateVector(0.0, -1.0, 0.0),  Shape::MakeShared<Cylinder>(1.0, 2.0, true) },
				{Point::CreatePoint(0.0, 2.0, 0.0), Vector::CreateVector(0.0, 1.0, 0.0),  Shape::MakeShared<Cylinder>(1.0, 2.0, true) },
				{Point::CreatePoint(0.5, 2.0, 0.0), Vector::CreateVector(0.0, 1.0, 0.0),  Shape::MakeShared<Cylinder>(1.0, 2.0, true) },
				{Point::CreatePoint(0.0, 2.0, 0.5), Vector::CreateVector(0.0, 1.0, 0.0),  Shape::MakeShared<Cylinder>(1.0, 2.0, true) }
			};


			for (auto test : testData) {
				Assert::IsTrue(std::get<2>(test)->SurfaceNormal(std::get<0>(test)) == std::get<1>(test));
			}

		}
		TEST_METHOD(CylinderIntersections) {
			size_t id = 3;

			//Rays that will be tested for intersections
			std::vector<Ray> rays = {
				//Untruncated cylinders
				//Rays that miss the cylinder
				Ray(Point::CreatePoint(1.0, 0.0, 0.0), Vector::CreateVector(0.0, 1.0, 0.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 0.0, 0.0), Vector::CreateVector(0.0, 1.0, 0.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(1.0, 1.0, 1.0).Normalize()),

				//Rays that hit the cylinder
				Ray(Point::CreatePoint(1.0, 0.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(0.5, 0.0, -5.0), Vector::CreateVector(0.1, 1.0, 1.0).Normalize()),

				//Truncated cylinder
				Ray(Point::CreatePoint(0.0, 1.5, 0.0), Vector::CreateVector(0.1, 1.0, 0.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 3.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 2.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 1.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 1.5, -2.0), Vector::CreateVector(0.0, 0.0, 1.0).Normalize()),

				//Cylinder with end caps
				Ray(Point::CreatePoint(0.0, 3.0, 0.0), Vector::CreateVector(0.0, -1.0, 0.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 3.0, -2.0), Vector::CreateVector(0.0, -1.0, 2.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 4.0, -2.0), Vector::CreateVector(0.0, -1.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 0.0, -2.0), Vector::CreateVector(0.0, 1.0, 2.0).Normalize()),
				Ray(Point::CreatePoint(0.0, -1.0, -2.0), Vector::CreateVector(0.0, 1.0, 1.0).Normalize())
			};

			//Cylinders that will be intersected
			std::vector<std::shared_ptr<Shape>> cylinders = {
				//Untruncated cylinders
				//Results for rays that miss the cylinder
				Shape::MakeShared<Cylinder>(),
				Shape::MakeShared<Cylinder>(),
				Shape::MakeShared<Cylinder>(),

				//Expected results for rays hitting the cylinder
				Shape::MakeShared<Cylinder>(),
				Shape::MakeShared<Cylinder>(),
				Shape::MakeShared<Cylinder>(),

				//Truncated cylinder
				Shape::MakeShared<Cylinder>(1.0, 2.0, false),
				Shape::MakeShared<Cylinder>(1.0, 2.0, false),
				Shape::MakeShared<Cylinder>(1.0, 2.0, false),
				Shape::MakeShared<Cylinder>(1.0, 2.0, false),
				Shape::MakeShared<Cylinder>(1.0, 2.0, false),
				Shape::MakeShared<Cylinder>(1.0, 2.0, false),

				//Cylinder with end caps
				Shape::MakeShared<Cylinder>(1.0, 2.0, true),
				Shape::MakeShared<Cylinder>(1.0, 2.0, true),
				Shape::MakeShared<Cylinder>(1.0, 2.0, true),
				Shape::MakeShared<Cylinder>(1.0, 2.0, true),
				Shape::MakeShared<Cylinder>(1.0, 2.0, true)
			};

			//Expected results of the intersection tests
			std::vector<IntersectionBuffer> expected = {
				//Untruncated cylinders
				//Results for rays that miss the cylinder
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),

				//Expected results for rays hitting the cylinder
				IntersectionBuffer(Intersection(5.0, cylinders[3]), Intersection(5.0, cylinders[3])),
				IntersectionBuffer(Intersection(4.0, cylinders[4]), Intersection(6.0, cylinders[4])),
				IntersectionBuffer(Intersection(6.80798, cylinders[5]), Intersection(7.08872, cylinders[5])),

				//Truncated cylinder
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(Intersection(0.0, cylinders[11]), Intersection(0.0, cylinders[11])),
				
				//Cylinder with end caps
				IntersectionBuffer(Intersection(0.0, cylinders[12]), Intersection(0.0, cylinders[12])),
				IntersectionBuffer(Intersection(0.0, cylinders[13]), Intersection(0.0, cylinders[13])),
				IntersectionBuffer(Intersection(0.0, cylinders[14]), Intersection(0.0, cylinders[14])),
				IntersectionBuffer(Intersection(0.0, cylinders[15]), Intersection(0.0, cylinders[15])),
				IntersectionBuffer(Intersection(0.0, cylinders[16]), Intersection(0.0, cylinders[16]))
			};

			//Does only the number of intersections matter (skip verifying the exact values)
			std::vector<bool> onlyCheckIntersectionCount = {
				//Results for rays that miss / hit the cylinder
				false, false, false,
				false, false, false,
				
				//Truncated cylinder
				true, true, true,
				true, true, 
				
				//Cylinder with end caps
				true, true, true,
				true, true, true
			};

			//Simulate each test case
			for (size_t testNr = 0; testNr < expected.size(); testNr++) {
				Ray testRay = rays[testNr];
				IntersectionBuffer expectedIntersections = expected[testNr];
				auto testCylinder = cylinders[testNr];

				IntersectionBuffer intersections;
				testCylinder->FindIntersections(testRay, intersections);
				
				//Logger::WriteMessage(std::to_string(testNr).c_str());

				//Make sure the correct number of hits was found
				Assert::IsTrue(intersections.GetCount() == expectedIntersections.GetCount());

				//If there were any intersections, make sure that they were calculated correctly
				if (intersections.GetCount() != 0 && !onlyCheckIntersectionCount[testNr]) {
					//Logger::WriteMessage(std::to_string(intersections[0].t).c_str());
					//Logger::WriteMessage(std::to_string(intersections[1].t).c_str());
					
					Assert::IsTrue(Constants::DoubleEqual(intersections[0].t, expectedIntersections[0].t));
					Assert::IsTrue(intersections[0].shape == expectedIntersections[0].shape);

					Assert::IsTrue(Constants::DoubleEqual(intersections[1].t, expectedIntersections[1].t));
					Assert::IsTrue(intersections[1].shape == expectedIntersections[1].shape);
				}
			}
		}

		TEST_METHOD(Bounds) {
			auto s = Shape::MakeShared<Cylinder>();
			auto bounds = s->GetObjectSpaceBounds();

			//Unbounded Cylinder	
			auto max = bounds.GetMax();
			auto min = bounds.GetMin();
			Assert::IsTrue(Constants::DoubleEqual(min.x, -1.0) && Constants::DoubleEqual(min.z, -1.0));
			Assert::IsTrue(Constants::DoubleEqual(max.x, 1.0) && Constants::DoubleEqual(max.z, 1.0));
			Assert::IsTrue(std::isinf(min.y) && min.y < 0.0);
			Assert::IsTrue(std::isinf(max.y) && max.y > 0.0);

			//Bounded Cylinder
			s->SetMaximum(3.0);
			s->SetMinimum(-5.0);
			bounds = s->GetObjectSpaceBounds();
			max = bounds.GetMax();
			min = bounds.GetMin();

			Assert::IsTrue(min == Point::CreatePoint(-1.0, -5.0, -1.0));
			Assert::IsTrue(max == Point::CreatePoint(1.0, 3.0, 1.0));
		}
		
	};
}
