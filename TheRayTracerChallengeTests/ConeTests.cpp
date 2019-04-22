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
#include <Cone.h>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(ConeTests)
	{
	public:

		TEST_METHOD(IsShape) {
			Assert::IsTrue(std::is_base_of<Shape, Cone>());
		}

		TEST_METHOD(ConeCreation) {
			Cone cone;

			Assert::IsTrue(std::isinf(cone.GetMaximum()) && std::isinf(cone.GetMinimum()));
			Assert::IsTrue(cone.GetMaximum() > cone.GetMinimum());
			Assert::IsTrue(cone.IsClosed() == false);
		}
		TEST_METHOD(SurfaceNormal) {
			std::vector<std::tuple<Point, Vector, Cone>> testData = {
				//Input, Expected output, cone

				{Point::CreatePoint(0.0, 0.0, 0.0), Vector::CreateVector(0.0, 0.0, 0.0).Normalize(),  Cone() },
				{Point::CreatePoint(1.0, 1.0, 1.0), Vector::CreateVector(1.0, -sqrt(2.0), 1.0).Normalize(),  Cone() },
				{Point::CreatePoint(-1.0, -1.0, 0.0), Vector::CreateVector(-1.0, 1.0, 0.0).Normalize(),  Cone() },
			};

			size_t testNr = 0;
			for (auto test : testData) {

				Logger::WriteMessage(std::to_string(testNr).c_str());
				Point input = std::get<0>(test);
				Cone cone = std::get<2>(test);
				Vector expectedOutput = std::get<1>(test);
				Vector output = cone.SurfaceNormal(input);

				Assert::IsTrue(output == expectedOutput);
				testNr++;
			}

		}
		TEST_METHOD(ConeIntersections) {
			size_t id = 3;

			//Rays that will be tested for intersections
			std::vector<Ray> rays = {
				//Untruncated cones
				//Rays that hit the cone
				Ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(1.0, 1.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(1.0, 1.0, -5.0), Vector::CreateVector(-0.5, -1.0, 1.0).Normalize()),
				//Ray parallel to one wall of the cone
				Ray(Point::CreatePoint(0.0, 0.0, -1.0), Vector::CreateVector(0.0, 1.0, 1.0).Normalize()),
				//Ray hits the cone's end caps
				Ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(0.0, 1.0, 0.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 0.0, -0.25), Vector::CreateVector(0.0, 1.0, 1.0).Normalize()),
				Ray(Point::CreatePoint(0.0, 0.0, -0.25), Vector::CreateVector(0.0, 1.0, 0.0).Normalize())
			};

			//Expected results of the intersection tests
			std::vector<IntersectionBuffer> expected = {
				//Untruncated cones
				//Rays that hit the cone
				IntersectionBuffer(Intersection(5.0, id), Intersection(5.0, id)),
				IntersectionBuffer(Intersection(8.66025, id), Intersection(8.66025, id)),
				IntersectionBuffer(Intersection(4.55006, id), Intersection(49.44994, id)),
				//Ray parallel to one wall of the cone
				IntersectionBuffer(Intersection(0.35355, id)),
				//Ray hits the cone's end caps
				IntersectionBuffer(),
				IntersectionBuffer(Intersection(1234.0, id), Intersection(1234.0, id)),
				IntersectionBuffer(Intersection(1234.0, id), Intersection(1234.0, id), Intersection(1234.0, id), Intersection(1234.0, id))
			};

			//Cylinders that will be intersected
			std::vector<Cone> cones = {
				//Untruncated cones
				//Rays that hit the cone
				Cone(),
				Cone(),
				Cone(),
				//Ray parallel to one wall of the cone
				Cone(),
				//Ray hits the cone's end caps
				Cone(-0.5, 0.5, true),
				Cone(-0.5, 0.5, true),
				Cone(-0.5, 0.5, true)
			};

			//Does only the number of intersections matter (skip verifying the exact values)
			std::vector<bool> onlyCheckIntersectionCount = {
				//Untruncated cones
				//Rays that hit the cone
				false, false, false,
				//Ray parallel to one wall of the cone
				false,
				//Ray hits the cone's end caps
				true, true, true
			};

			//Simulate each test case
			for (size_t testNr = 0; testNr < expected.size(); testNr++) {
				Ray testRay = rays[testNr];
				IntersectionBuffer expectedIntersections = expected[testNr];
				Cone testCone = cones[testNr];
				testCone.SetID(id);

				IntersectionBuffer intersections = testCone.FindIntersections(testRay);

				Logger::WriteMessage(std::to_string(testNr).c_str());

				//Make sure the correct number of hits was found
				Assert::IsTrue(intersections.GetCount() == expectedIntersections.GetCount());

				//If there were any intersections, make sure that they were calculated correctly
				if (intersections.GetCount() != 0 && !onlyCheckIntersectionCount[testNr]) {
					for (size_t i = 0; i < intersections.GetCount(); i++) {
						Logger::WriteMessage(std::to_string(intersections[i].t).c_str());

						Assert::IsTrue(Constants::DoubleEqual(intersections[i].t, expectedIntersections[i].t));
						Assert::IsTrue(intersections[i].objectID == expectedIntersections[i].objectID);
					}
				}
			}
		}

	};
}
