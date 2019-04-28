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
#include <ShapeGroup.h>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(ShapeGroupTests)
	{
	private:
		//Class that implements the virtual methods, so the base class can be tested
		class TestShape : public Shape {
		public:
			Ray receivedRay;

			void FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) override {
				receivedRay = ray;
			}

			Vector FindObjectSpaceNormal(Point p) {
				return Vector::CreateVector(p.x, p.y, p.z);
			}

			BoundingBox GetObjectSpaceBounds() override {
				return BoundingBox(Point::CreatePoint(-1.0, -1.0, -1.0), Point::CreatePoint(1.0, 1.0, 1.0));
			}

			void PartitionChildren(size_t maximumShapeCount) override {}

		private:
			std::shared_ptr<Shape> ShapeSpecificCopy() override {
				return Shape::MakeShared<TestShape>(*this);
			}
		};
	public:

		TEST_METHOD(IsShape) {
			Assert::IsTrue(std::is_base_of<Shape, ShapeGroup>());
		}

		TEST_METHOD(Creation) {
			auto group = Shape::MakeShared<ShapeGroup>();

			Assert::IsTrue(group->GetShapeCount() == 0);
			Assert::IsTrue(group->GetTransformCopy().GetMatrix() == Matrix::IndentityMatrix4x4());
		}

		TEST_METHOD(AddingShape) {
			auto group = Shape::MakeShared<ShapeGroup>();
			auto testshape = Shape::MakeShared<TestShape>();

			group->AddShape(testshape);

			Assert::IsTrue(group->GetShapeCount() == 1);
			Assert::IsTrue(group->ContainsShape(testshape));
			Assert::IsTrue(testshape->GetParent() == group);
		}

		TEST_METHOD(EmptyGroupIntersection) {
			auto group = Shape::MakeShared<ShapeGroup>();

			Ray ray(
				Point::CreatePoint(0.0, 0.0, 0.0),
				Vector::CreateVector(0.0, 0.0, 1.0)
			);
			IntersectionBuffer xs;
			group->FindIntersections(ray, xs);
			Assert::IsTrue(xs.GetCount() == 0);
		}

		TEST_METHOD(NonemtpyGroupIntersection) {
			auto group = Shape::MakeShared<ShapeGroup>();
			auto s1 = Shape::MakeShared<Sphere>();
			auto s2 = Shape::MakeShared<Sphere>();
			auto s3 = Shape::MakeShared<Sphere>();

			s2->SetTransform(Transform::CreateTranslation(0.0, 0.0, -3.0));
			s3->SetTransform(Transform::CreateTranslation(5.0, 0.0, 0.0));

			group->AddShape(s1);
			group->AddShape(s2);
			group->AddShape(s3);

			Ray ray(
				Point::CreatePoint(0.0, 0.0, -5.0),
				Vector::CreateVector(0.0, 0.0, 1.0)
			);

			IntersectionBuffer xs;
			group->FindObjectSpaceIntersections(ray, xs);

			Assert::IsTrue(xs.GetCount() == 4);

			xs.Sort();

			Assert::IsTrue(xs[0].shape == s2);
			Assert::IsTrue(xs[1].shape == s2);
			Assert::IsTrue(xs[2].shape == s1);
			Assert::IsTrue(xs[3].shape == s1);
		}

		TEST_METHOD(TransformedGroupIntersections) {
			auto group = Shape::MakeShared<ShapeGroup>();
			auto s = Shape::MakeShared<Sphere>();

			group->SetTransform(Transform::CreateScale(2.0, 2.0, 2.0));
			s->SetTransform(Transform::CreateTranslation(5.0, 0.0, 0.0));

			group->AddShape(s);

			Ray ray(
				Point::CreatePoint(10.0, 0.0, -10.0),
				Vector::CreateVector(0.0, 0.0, 1.0)
			);

			IntersectionBuffer xs;
			group->FindIntersections(ray, xs);
			Assert::IsTrue(xs.GetCount() == 2);
		}

	};
}
