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
			std::vector<std::tuple<Point, Vector, std::shared_ptr<Cone>>> testData = {
				//Input, Expected output, cone

				{Point::CreatePoint(0.0, 0.0, 0.0), Vector::CreateVector(0.0, 0.0, 0.0).Normalize(),  Shape::MakeShared<Cone>() },
				{Point::CreatePoint(1.0, 1.0, 1.0), Vector::CreateVector(1.0, -sqrt(2.0), 1.0).Normalize(),  Shape::MakeShared<Cone>() },
				{Point::CreatePoint(-1.0, -1.0, 0.0), Vector::CreateVector(-1.0, 1.0, 0.0).Normalize(),  Shape::MakeShared<Cone>() },
			};

			size_t testNr = 0;
			for (auto test : testData) {

				Logger::WriteMessage(std::to_string(testNr).c_str());
				Point input = std::get<0>(test);
				auto cone = std::get<2>(test);
				Vector expectedOutput = std::get<1>(test);
				Vector output = cone->SurfaceNormal(input);

				Assert::IsTrue(output == expectedOutput);
				testNr++;
			}

		}
		TEST_METHOD(ConeIntersections) {

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

			//Cylinders that will be intersected
			std::vector<std::shared_ptr<Shape>> cones = {
				//Untruncated cones
				//Rays that hit the cone
				Shape::MakeShared<Cone>(),
				Shape::MakeShared<Cone>(),
				Shape::MakeShared<Cone>(),
				//Ray parallel to one wall of the cone
				Shape::MakeShared<Cone>(),
				//Ray hits the cone's end caps
				Shape::MakeShared<Cone>(-0.5, 0.5, true),
				Shape::MakeShared<Cone>(-0.5, 0.5, true),
				Shape::MakeShared<Cone>(-0.5, 0.5, true)
			};

			//Expected results of the intersection tests
			std::vector<IntersectionBuffer> expected = {
				//Untruncated cones
				//Rays that hit the cone
				IntersectionBuffer(Intersection(5.0, cones[0]), Intersection(5.0, cones[0])),
				IntersectionBuffer(Intersection(8.66025, cones[1]), Intersection(8.66025, cones[1])),
				IntersectionBuffer(Intersection(4.55006, cones[2]), Intersection(49.44994, cones[2])),
				//Ray parallel to one wall of the cone
				IntersectionBuffer(Intersection(0.35355, cones[3])),
				//Ray hits the cone's end caps
				IntersectionBuffer(),
				IntersectionBuffer(Intersection(1234.0, cones[4]), Intersection(1234.0, cones[4])),
				IntersectionBuffer(Intersection(1234.0, cones[5]), Intersection(1234.0, cones[5]), Intersection(1234.0, cones[5]), Intersection(1234.0, cones[5]))
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
				auto testCone = cones[testNr];
				IntersectionBuffer intersections;
				testCone->FindIntersections(testRay, intersections);

				Logger::WriteMessage(std::to_string(testNr).c_str());

				//Make sure the correct number of hits was found
				Assert::IsTrue(intersections.GetCount() == expectedIntersections.GetCount());

				//If there were any intersections, make sure that they were calculated correctly
				if (intersections.GetCount() != 0 && !onlyCheckIntersectionCount[testNr]) {
					for (size_t i = 0; i < intersections.GetCount(); i++) {
						Logger::WriteMessage(std::to_string(intersections[i].t).c_str());

						Assert::IsTrue(Constants::DoubleEqual(intersections[i].t, expectedIntersections[i].t));
						Assert::IsTrue(intersections[i].shape == expectedIntersections[i].shape);
					}
				}
			}
		}

		TEST_METHOD(Bounds) {
			auto s = Shape::MakeShared<Cone>();
			auto bounds = s->GetObjectSpaceBounds();

			//Unbounded Cone	
			auto max = bounds.GetMax();
			auto min = bounds.GetMin();

			Assert::IsTrue(std::isinf(min.x) && min.x < 0.0);
			Assert::IsTrue(std::isinf(min.y) && min.y < 0.0);
			Assert::IsTrue(std::isinf(min.z) && min.z < 0.0);

			Assert::IsTrue(std::isinf(max.x) && max.x > 0.0);
			Assert::IsTrue(std::isinf(max.y) && max.y > 0.0);
			Assert::IsTrue(std::isinf(max.z) && max.z > 0.0);

			//Bounded Cone
			s->SetMaximum(3.0);
			s->SetMinimum(-5.0);
			bounds = s->GetObjectSpaceBounds();
			max = bounds.GetMax();
			min = bounds.GetMin();


			Assert::IsTrue(min == Point::CreatePoint(-5.0, -5.0, -5.0));
			Assert::IsTrue(max == Point::CreatePoint(5.0, 3.0, 5.0));
		}
	};
}
