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

			std::vector<std::tuple<Point, Vector, Cylinder>> testData = {
				//Input, Expected output, cylinder
				//Normal vectors on the cylinder's walls
				{Point::CreatePoint(1.0, 0.0, 0.0), Vector::CreateVector(1.0, 0.0, 0.0), Cylinder() },
				{Point::CreatePoint(0.0, 5.0, -1.0), Vector::CreateVector(0.0, 0.0, -1.0), Cylinder() },
				{Point::CreatePoint(0.0, -2.0, 1.0), Vector::CreateVector(0.0, 0.0, 1.0),  Cylinder() },
				{Point::CreatePoint(-1.0, 1.0, 0.0), Vector::CreateVector(-1.0, 0.0, 0.0),  Cylinder() },
				//Normal vectors on the end caps
				{Point::CreatePoint(0.0, 1.0, 0.0), Vector::CreateVector(0.0, -1.0, 0.0),  Cylinder(1.0, 2.0, true) },
				{Point::CreatePoint(0.5, 1.0, 0.0), Vector::CreateVector(0.0, -1.0, 0.0),  Cylinder(1.0, 2.0, true) },
				{Point::CreatePoint(0.0, 1.0, 0.5), Vector::CreateVector(0.0, -1.0, 0.0),  Cylinder(1.0, 2.0, true) },
				{Point::CreatePoint(0.0, 2.0, 0.0), Vector::CreateVector(0.0, 1.0, 0.0),  Cylinder(1.0, 2.0, true) },
				{Point::CreatePoint(0.5, 2.0, 0.0), Vector::CreateVector(0.0, 1.0, 0.0),  Cylinder(1.0, 2.0, true) },
				{Point::CreatePoint(0.0, 2.0, 0.5), Vector::CreateVector(0.0, 1.0, 0.0),  Cylinder(1.0, 2.0, true) }
			};


			for (auto test : testData) {
				Assert::IsTrue(std::get<2>(test).SurfaceNormal(std::get<0>(test)) == std::get<1>(test));
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

			//Expected results of the intersection tests
			std::vector<IntersectionBuffer> expected = {
				//Untruncated cylinders
				//Results for rays that miss the cylinder
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),

				//Expected results for rays hitting the cylinder
				IntersectionBuffer(Intersection(5.0, id), Intersection(5.0, id)),
				IntersectionBuffer(Intersection(4.0, id), Intersection(6.0, id)),
				IntersectionBuffer(Intersection(6.80798, id), Intersection(7.08872, id)),

				//Truncated cylinder
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(),
				IntersectionBuffer(Intersection(0.0, id), Intersection(0.0, id)),
				
				//Cylinder with end caps
				IntersectionBuffer(Intersection(0.0, id), Intersection(0.0, id)),
				IntersectionBuffer(Intersection(0.0, id), Intersection(0.0, id)),
				IntersectionBuffer(Intersection(0.0, id), Intersection(0.0, id)),
				IntersectionBuffer(Intersection(0.0, id), Intersection(0.0, id)),
				IntersectionBuffer(Intersection(0.0, id), Intersection(0.0, id))
			};

			//Cylinders that will be intersected
			std::vector<Cylinder> cylinders = {
				//Untruncated cylinders
				//Results for rays that miss the cylinder
				Cylinder(),
				Cylinder(),
				Cylinder(),

				//Expected results for rays hitting the cylinder
				Cylinder(),
				Cylinder(),
				Cylinder(),

				//Truncated cylinder
				Cylinder(1.0, 2.0, false),
				Cylinder(1.0, 2.0, false),
				Cylinder(1.0, 2.0, false),
				Cylinder(1.0, 2.0, false),
				Cylinder(1.0, 2.0, false),
				Cylinder(1.0, 2.0, false),

				//Cylinder with end caps
				Cylinder(1.0, 2.0, true),
				Cylinder(1.0, 2.0, true),
				Cylinder(1.0, 2.0, true),
				Cylinder(1.0, 2.0, true),
				Cylinder(1.0, 2.0, true)
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
				Cylinder testCylinder = cylinders[testNr];
				testCylinder.SetID(id);

				IntersectionBuffer intersections = testCylinder.FindIntersections(testRay);
				
				//Logger::WriteMessage(std::to_string(testNr).c_str());

				//Make sure the correct number of hits was found
				Assert::IsTrue(intersections.GetCount() == expectedIntersections.GetCount());

				//If there were any intersections, make sure that they were calculated correctly
				if (intersections.GetCount() != 0 && !onlyCheckIntersectionCount[testNr]) {
					//Logger::WriteMessage(std::to_string(intersections[0].t).c_str());
					//Logger::WriteMessage(std::to_string(intersections[1].t).c_str());
					
					Assert::IsTrue(Constants::DoubleEqual(intersections[0].t, expectedIntersections[0].t));
					Assert::IsTrue(intersections[0].objectID == expectedIntersections[0].objectID);

					Assert::IsTrue(Constants::DoubleEqual(intersections[1].t, expectedIntersections[1].t));
					Assert::IsTrue(intersections[1].objectID == expectedIntersections[1].objectID);
				}
			}
		}
		
	};
}
