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
#include <Sphere.h>
#include <Cylinder.h>
#include <ShapeGroup.h>
#include <BoundingBox.h>
#include <type_traits>
#include <vector>
#include <CSGShape.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(CSGShape_Tests)
	{


	public:
		TEST_METHOD(Creation) {
			auto s1 = Shape::MakeShared<Sphere>();
			auto s2 = Shape::MakeShared<Cube>();

			auto c = Shape::MakeShared<CSGShape>();
			c->SetOperation(CSGShape::Operation::Union);
			c->SetLeft(s1);
			c->SetRight(s2);

			Assert::IsTrue(c->GetLeft() == s1);
			Assert::IsTrue(c->GetRight() == s2);
			Assert::IsTrue(c->GetLeft()->GetParent() == c);
			Assert::IsTrue(c->GetRight()->GetParent() == c);
		}
		TEST_METHOD(OperationRules) {
			std::vector <std::tuple<CSGShape::Operation, bool, bool, bool, bool>> tests = {
				//Operation, leftHit, inLeft, inRight, expected result

				//Union
				{CSGShape::Operation::Union, true, true, true, false},
				{CSGShape::Operation::Union, true, true, false, true},
				{CSGShape::Operation::Union, true, false, true, false},
				{CSGShape::Operation::Union, true, false, false, true},
				{CSGShape::Operation::Union, false, true, true, false},
				{CSGShape::Operation::Union, false, true, false, false},
				{CSGShape::Operation::Union, false, false, true, true},
				{CSGShape::Operation::Union, false, false, false, true},

				//Intersection
				{CSGShape::Operation::Intersection, true, true, true, true},
				{CSGShape::Operation::Intersection, true, true, false, false},
				{CSGShape::Operation::Intersection, true, false, true, true},
				{CSGShape::Operation::Intersection, true, false, false, false},
				{CSGShape::Operation::Intersection, false, true, true, true},
				{CSGShape::Operation::Intersection, false, true, false, true},
				{CSGShape::Operation::Intersection, false, false, true, false},
				{CSGShape::Operation::Intersection, false, false, false, false},

				//Difference
				{CSGShape::Operation::Difference, true, true, true, false},
				{CSGShape::Operation::Difference, true, true, false, true},
				{CSGShape::Operation::Difference, true, false, true, false},
				{CSGShape::Operation::Difference, true, false, false, true},
				{CSGShape::Operation::Difference, false, true, true, true},
				{CSGShape::Operation::Difference, false, true, false, true},
				{CSGShape::Operation::Difference, false, false, true, false},
				{CSGShape::Operation::Difference, false, false, false, false}
			};

			size_t testIndex = 0;
			for (auto& test : tests) {
				Logger::WriteMessage(std::to_string(testIndex).c_str());
				Assert::IsTrue(
					CSGShape::IntersectionAllowed(std::get<0>(test), std::get<1>(test), std::get<2>(test), std::get<3>(test)) == std::get<4>(test)
				);

				testIndex++;
			}
		}

		TEST_METHOD(IntersectionFiltering) {
			std::vector<std::tuple<CSGShape::Operation, size_t, size_t>> tests = {
				{CSGShape::Operation::Union, 0, 3},
				{CSGShape::Operation::Intersection, 1, 2},
				{CSGShape::Operation::Difference, 0, 1}
			};


			auto s1 = Shape::MakeShared<Sphere>();
			auto s2 = Shape::MakeShared<Cube>();

			for (auto& test : tests) {
				auto csg = Shape::MakeShared<CSGShape>();
				csg->SetLeft(s1);
				csg->SetRight(s2);
				csg->SetOperation(std::get<0>(test));

				IntersectionBuffer in;
				in.Add(Intersection(1.0, s1));
				in.Add(Intersection(2.0, s2));
				in.Add(Intersection(3.0, s1));
				in.Add(Intersection(4.0, s2));
				IntersectionBuffer out;


				csg->FilterIntersections(in, out);
				out.Sort();

				Assert::IsTrue(out.GetCount() == 2);
				Assert::IsTrue(in[std::get<1>(test)] == out[0]);
				Assert::IsTrue(in[std::get<2>(test)] == out[1]);
			}
		}

		TEST_METHOD(RayMissesShapes) {
			auto c = Shape::MakeShared<CSGShape>();
			c->SetLeft(Shape::MakeShared<Sphere>());
			c->SetRight(Shape::MakeShared<Cube>());
			c->SetOperation(CSGShape::Operation::Union);

			Ray ray(Point::CreatePoint(0.0, 2.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0));

			IntersectionBuffer xs;
			c->FindObjectSpaceIntersections(ray, xs);

			Assert::IsTrue(xs.GetCount() == 0);
		}

		TEST_METHOD(RayHitsShapes) {
			auto s1 = Shape::MakeShared<Sphere>();
			auto s2 = Shape::MakeShared<Sphere>();
			auto c = Shape::MakeShared<CSGShape>();

			s2->SetTransform(Transform::CreateTranslation(0.0, 0.0, 0.5));

			c->SetLeft(s1);
			c->SetRight(s2);
			c->SetOperation(CSGShape::Operation::Union);

			Ray ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0));

			IntersectionBuffer xs;
			c->FindObjectSpaceIntersections(ray, xs);

			xs.Sort();
			Assert::IsTrue(xs.GetCount() == 2);
			Assert::IsTrue(Constants::DoubleEqual(xs[0].t, 4.0));
			Assert::IsTrue(Constants::DoubleEqual(xs[1].t, 6.5));
			Assert::IsTrue(xs[0].shape == s1);
			Assert::IsTrue(xs[1].shape == s2);
		}
	};
}
