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
#include <Sphere.h>
#include <Cylinder.h>
#include <ShapeGroup.h>
#include <BoundingBox.h>
#include <type_traits>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(BoundingBoxTests)
	{
	private:
		class TestShape : public Shape {

		public:
			IntersectionBuffer FindObjectSpaceIntersections(Ray ray) override { findIntersectionsCalled = true; return IntersectionBuffer(); }

			Vector FindObjectSpaceNormal(Point p) override { return Vector(); }

			BoundingBox GetObjectSpaceBounds() override {
				return BoundingBox(Point::CreatePoint(-1.0, -1.0, -1.0), Point::CreatePoint(1.0, 1.0, 1.0));
			}
			bool findIntersectionsCalled = false;

			void PartitionChildren(size_t maximumShapeCount) override {}

		private:
			std::shared_ptr<Shape> ShapeSpecificCopy() override {
				return Shape::MakeShared<TestShape>(*this);
			}
		};

	public:
		TEST_METHOD(DefaultCreation) {
			BoundingBox b;

			auto max = b.GetMax();
			auto min = b.GetMin();

			constexpr double infinity = std::numeric_limits<double>::infinity();
			Assert::IsTrue(std::isinf(max.x));
			Assert::IsTrue(std::isinf(max.y));
			Assert::IsTrue(std::isinf(max.z));
			Assert::IsTrue(std::isinf(min.x));
			Assert::IsTrue(std::isinf(min.y));
			Assert::IsTrue(std::isinf(min.z));

			Assert::IsTrue(max.x < 0 && max.y < 0 && max.z < 0);
			Assert::IsTrue(min.x > 0 && min.y > 0 && min.z > 0);
		}

		TEST_METHOD(Initialization) {
			Point min = Point::CreatePoint(-1.0, -2.0, -3.0);
			Point max = Point::CreatePoint(3.0, 2.0, 1.0);
			BoundingBox b(min, max);

			Assert::IsTrue(b.GetMax() == max);
			Assert::IsTrue(b.GetMin() == min);
		}

		TEST_METHOD(AddingPoints) {
			BoundingBox b;

			Point p1 = Point::CreatePoint(-5.0, 2.0, 0.0);
			Point p2 = Point::CreatePoint(7.0, 0.0, -3.0);

			b.Add(p1);
			b.Add(p2);

			Assert::IsTrue(b.GetMin() == Point::CreatePoint(-5.0, 0.0, -3.0));
			Assert::IsTrue(b.GetMax() == Point::CreatePoint(7.0, 2.0, 0.0));
		}

		TEST_METHOD(AddBoundingBoxed) {
			BoundingBox b1(Point::CreatePoint(-5.0, -2.0, 0.0), Point::CreatePoint(7.0, 4.0, 4.0));
			BoundingBox b2(Point::CreatePoint(8.0, -7.0, -2.0), Point::CreatePoint(14.0, 2.0, 8.0));

			b1.Add(b2);

			Assert::IsTrue(b1.GetMin() == Point::CreatePoint(-5.0, -7.0, -2.0));
			Assert::IsTrue(b1.GetMax() == Point::CreatePoint(14.0, 4.0, 8.0));
		}

		TEST_METHOD(ContainsPoint) {
			BoundingBox b(Point::CreatePoint(5.0, -2.0, 0.0), Point::CreatePoint(11.0, 4.0, 7.0));

			std::vector<std::pair<Point, bool>> tests = {
				{Point::CreatePoint(5.0, -2.0, 0.0), true },
				{Point::CreatePoint(11.0, 4.0, 7.0), true},
				{Point::CreatePoint(8.0, 1.0, 3.0), true},
				{Point::CreatePoint(3.0, 0.0, 3.0), false },
				{Point::CreatePoint(8.0, -4.0, 3.0), false },
				{Point::CreatePoint(8.0, 1.0, -1.0), false },
				{Point::CreatePoint(13.0, 1.0, 3.0), false },
				{Point::CreatePoint(8.0, 5.0, 3.0), false },
				{Point::CreatePoint(8.0, 1.0, 8.0), false}
			};

			for (auto test : tests) {
				Assert::IsTrue(b.Contains(test.first) == test.second);
			}

		}

		TEST_METHOD(ContainsBox) {
			BoundingBox b(Point::CreatePoint(5.0, -2.0, 0.0), Point::CreatePoint(11.0, 4.0, 7.0));


			std::vector<std::pair<BoundingBox, bool>> tests = {
				{BoundingBox(Point::CreatePoint(5.0, -2.0, 0.0), Point::CreatePoint(11.0, 4.0, 7.0)), true },
				{BoundingBox(Point::CreatePoint(6.0, -1.0, 1.0), Point::CreatePoint(10.0, 3.0, 6.0)), true },
				{BoundingBox(Point::CreatePoint(4.0, -3.0, -1.0), Point::CreatePoint(10.0, 3.0, 6.0)), false },
				{BoundingBox(Point::CreatePoint(6.0, -1.0, 1.0), Point::CreatePoint(12.0, 5.0, 8.0)), false }
			};

			for (auto test : tests) {
				Assert::IsTrue(b.Contains(test.first) == test.second);
			}
		}

		TEST_METHOD(TransformedBoundingBox) {
			BoundingBox box(Point::CreatePoint(-1.0, -1.0, -1.0), Point::CreatePoint(1.0, 1.0, 1.0));

			Transform t = Transform::CreateRotationX(Constants::PI / 4.0) * Transform::CreateRotationY(Constants::PI / 4.0);

			BoundingBox box2 = box.ApplyTransform(t);

			Assert::IsTrue(box2.GetMin() == Point::CreatePoint(-1.414213, -1.707106, -1.707106));
			Assert::IsTrue(box2.GetMax() == Point::CreatePoint(1.414213, 1.707106, 1.707106));
		}

		TEST_METHOD(TransformedShapeBoundingBox) {
			auto shape = Shape::MakeShared<Sphere>();
			shape->SetTransform(
				Transform::CreateTranslation(1.0, -3.0, 5.0) * Transform::CreateScale(0.5, 2.0, 4.0)
			);

			auto box = shape->GetParentSpaceBounds();

			Assert::IsTrue(box.GetMin() == Point::CreatePoint(0.5, -5.0, 1.0));
			Assert::IsTrue(box.GetMax() == Point::CreatePoint(1.5, -1.0, 9.0));
		}

		TEST_METHOD(GroupBounds) {
			auto s = Shape::MakeShared<Sphere>();
			s->SetTransform(Transform::CreateTranslation(2.0, 5.0, -3.0) * Transform::CreateScale(2.0, 2.0, 2.0));

			auto c = Shape::MakeShared<Cylinder>(-2.0, 2.0, false);
			c->SetTransform(Transform::CreateTranslation(-4.0, -1.0, 4.0) * Transform::CreateScale(0.5, 1.0, 0.5));

			auto group = Shape::MakeShared<ShapeGroup>();
			group->AddShape(s);
			group->AddShape(c);

			auto box = group->GetParentSpaceBounds();
			Assert::IsTrue(box.GetMin() == Point::CreatePoint(-4.5, -3.0, -5.0));
			Assert::IsTrue(box.GetMax() == Point::CreatePoint(4.0, 7.0, 4.5));
		}

		/*Scenario: A CSG shape has a bounding box that contains its children
  Given left ? sphere()
	And right ? sphere() with:
	  | transform | translation(2, 3, 4) |
	And shape ? csg("difference", left, right)
  When box ? bounds_of(shape)
  Then box.min = point(-1, -1, -1)
	And box.max = point(3, 4, 5)*/


		TEST_METHOD(BoundingBoxIntersection) {
			BoundingBox box(Point::CreatePoint(-1.0, -1.0, -1.0), Point::CreatePoint(1.0, 1.0, 1.0));

			//Cubic bounding boxes
			std::vector<std::tuple<Point, Vector, bool>> tests = {
				{Point::CreatePoint(5, 0.5, 0), Vector::CreateVector(-1, 0, 0), true},
				{ Point::CreatePoint(-5, 0.5, 0), Vector::CreateVector(1, 0, 0), true },
				{ Point::CreatePoint(0.5, 5, 0), Vector::CreateVector(0, -1, 0), true },
				{ Point::CreatePoint(0.5, -5, 0), Vector::CreateVector(0, 1, 0), true },
				{ Point::CreatePoint(0.5, 0, 5), Vector::CreateVector(0, 0, -1), true },
				{ Point::CreatePoint(0.5, 0, -5), Vector::CreateVector(0, 0, 1), true },
				{ Point::CreatePoint(0, 0.5, 0), Vector::CreateVector(0, 0, 1), true },
				{ Point::CreatePoint(-2, 0, 0), Vector::CreateVector(2, 4, 6), false },
				{ Point::CreatePoint(0, -2, 0), Vector::CreateVector(6, 2, 4), false },
				{ Point::CreatePoint(0, 0, -2), Vector::CreateVector(4, 6, 2), false },
				{ Point::CreatePoint(2, 0, 2), Vector::CreateVector(0, 0, -1), false },
				{ Point::CreatePoint(0, 2, 2), Vector::CreateVector(0, -1, 0), false },
				{ Point::CreatePoint(2, 2, 0), Vector::CreateVector(-1, 0, 0), false }
			};

			for (auto test : tests) {
				Ray ray(std::get<0>(test), std::get<1>(test).Normalize());
				Assert::IsTrue(box.CheckIntersection(ray) == std::get<2>(test));

			}

			//Non cubic bounding box
			box = BoundingBox(Point::CreatePoint(5.0, -2.0, 0.0), Point::CreatePoint(11.0, 4.0, 7.0));
			tests = {
				{ Point::CreatePoint(15, 1, 2), Vector::CreateVector(-1, 0, 0), true  },
				{ Point::CreatePoint(-5, -1, 4), Vector::CreateVector(1, 0, 0), true  },
				{ Point::CreatePoint(7, 6, 5), Vector::CreateVector(0, -1, 0), true  },
				{ Point::CreatePoint(9, -5, 6), Vector::CreateVector(0, 1, 0), true  },
				{ Point::CreatePoint(8, 2, 12), Vector::CreateVector(0, 0, -1), true  },
				{ Point::CreatePoint(6, 0, -5), Vector::CreateVector(0, 0, 1), true  },
				{ Point::CreatePoint(8, 1, 3.5), Vector::CreateVector(0, 0, 1), true  },
				{ Point::CreatePoint(9, -1, -8), Vector::CreateVector(2, 4, 6), false },
				{ Point::CreatePoint(8, 3, -4), Vector::CreateVector(6, 2, 4), false },
				{ Point::CreatePoint(9, -1, -2), Vector::CreateVector(4, 6, 2), false },
				{ Point::CreatePoint(4, 0, 9), Vector::CreateVector(0, 0, -1), false },
				{ Point::CreatePoint(8, 6, -1), Vector::CreateVector(0, -1, 0), false },
				{ Point::CreatePoint(12, 5, 4), Vector::CreateVector(-1, 0, 0), false}
			};

			for (auto test : tests) {
				Ray ray(std::get<0>(test), std::get<1>(test).Normalize());
				Assert::IsTrue(box.CheckIntersection(ray) == std::get<2>(test));
			}
		}

		TEST_METHOD(GroupBoundingBoxIntersection) {
			auto group = Shape::MakeShared<ShapeGroup>();
			auto shape = Shape::MakeShared<TestShape>();
			group->AddShape(shape);

			//Ray misses the bounding box
			Ray ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(0.0, 1.0, 0.0));
			auto xs = group->FindIntersections(ray);
			//The shape was not checked for intersections because the ray misses the bounding box
			Assert::IsTrue(shape->findIntersectionsCalled == false);

			//Ray hits the bounding box
			ray = Ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0));
			xs = group->FindIntersections(ray);
			//The shape needs to be checked for intersections
			Assert::IsTrue(shape->findIntersectionsCalled == true);
		}


		/*Scenario: Intersecting ray+csg doesn't test children if box is missed
  Given left ? test_shape()
    And right ? test_shape()
    And shape ? csg("difference", left, right)
    And r ? ray(point(0, 0, -5), vector(0, 1, 0))
  When xs ? intersect(shape, r)
  Then left.saved_ray is unset
    And right.saved_ray is unset

Scenario: Intersecting ray+csg tests children if box is hit
  Given left ? test_shape()
    And right ? test_shape()
    And shape ? csg("difference", left, right)
    And r ? ray(point(0, 0, -5), vector(0, 0, 1))
  When xs ? intersect(shape, r)
  Then left.saved_ray is set
    And right.saved_ray is set*/

		TEST_METHOD(SplitBox) {
			//10x10x10 cube
			BoundingBox box(Point::CreatePoint(-1.0, -4.0, -5.0), Point::CreatePoint(9.0, 6.0, 5.0));
			auto split = box.SplitBox();
			Assert::IsTrue(split.first.GetMin() == Point::CreatePoint(-1.0, -4.0, -5.0));
			Assert::IsTrue(split.first.GetMax() == Point::CreatePoint(4.0, 6.0, 5.0));
			Assert::IsTrue(split.second.GetMin() == Point::CreatePoint(4.0, -4.0, -5.0));
			Assert::IsTrue(split.second.GetMax() == Point::CreatePoint(9.0, 6.0, 5.0));

			//Longest side in x direction
			box = BoundingBox(Point::CreatePoint(-1.0, -2.0, -3.0), Point::CreatePoint(9.0, 5.5, 3.0));
			split = box.SplitBox();
			Assert::IsTrue(split.first.GetMin() == Point::CreatePoint(-1.0, -2.0, -3.0));
			Assert::IsTrue(split.first.GetMax() == Point::CreatePoint(4.0, 5.5, 3.0));
			Assert::IsTrue(split.second.GetMin() == Point::CreatePoint(4.0, -2.0, -3.0));
			Assert::IsTrue(split.second.GetMax() == Point::CreatePoint(9.0, 5.5, 3.0));

			//Longest side in y direction
			box = BoundingBox(Point::CreatePoint(-1.0, -2.0, -3.0), Point::CreatePoint(5.0, 8.0, 3.0));
			split = box.SplitBox();
			Assert::IsTrue(split.first.GetMin() == Point::CreatePoint(-1.0, -2.0, -3.0));
			Assert::IsTrue(split.first.GetMax() == Point::CreatePoint(5.0, 3.0, 3.0));
			Assert::IsTrue(split.second.GetMin() == Point::CreatePoint(-1.0, 3.0, -3.0));
			Assert::IsTrue(split.second.GetMax() == Point::CreatePoint(5.0, 8.0, 3.0));

			//Longest side in z direction
			box = BoundingBox(Point::CreatePoint(-1.0, -2.0, -3.0), Point::CreatePoint(5.0, 3.0, 7.0));
			split = box.SplitBox();
			Assert::IsTrue(split.first.GetMin() == Point::CreatePoint(-1.0, -2.0, -3.0));
			Assert::IsTrue(split.first.GetMax() == Point::CreatePoint(5.0, 3.0, 2.0));
			Assert::IsTrue(split.second.GetMin() == Point::CreatePoint(-1.0, -2.0, 2.0));
			Assert::IsTrue(split.second.GetMax() == Point::CreatePoint(5.0, 3.0, 7.0));
		}

		TEST_METHOD(SubdivideChildren) {
			auto s1 = Shape::MakeShared<Sphere>();
			auto s2 = Shape::MakeShared<Sphere>();
			auto s3 = Shape::MakeShared<Sphere>();

			s1->SetTransform(Transform::CreateTranslation(-2.0, -2.0, 0.0));
			s2->SetTransform(Transform::CreateTranslation(-2.0, 2.0, 0.0));
			s3->SetTransform(Transform::CreateScale(4.0, 4.0, 4.0));

			auto g = Shape::MakeShared<ShapeGroup>();
			g->AddShape(s1);
			g->AddShape(s2);
			g->AddShape(s3);

			g->PartitionChildren(1);

			Assert::IsTrue(g->GetShape(0) == s3);

			//Other shapes were parititioned into new groups
			Assert::IsTrue(g->GetShapeCount() == 2);
			Assert::IsTrue(g->GetShape(1) != s2);
		}
	};
}
