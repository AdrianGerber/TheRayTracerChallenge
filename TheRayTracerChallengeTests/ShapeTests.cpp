#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <IntersectionBuffer.h>
#include <Transform.h>
#include <memory>
#include <cmath>
#include <Shape.h>
#include <Sphere.h>
#include <LightSource.h>
#include <HitCalculations.h>
#include <Material.h>
#include <ShapeGroup.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(ShapeTests)
	{
	private:
		//Class that implements the virtual methods, so the base class can be tested
		class TestShape : public Shape {
		public:
			Ray receivedRay;

			IntersectionBuffer FindObjectSpaceIntersections(Ray ray) {
				receivedRay = ray;
				return IntersectionBuffer();
			}

			Vector FindObjectSpaceNormal(Point p) {
				return Vector::CreateVector(p.x, p.y, p.z);
			}

		private:
			std::shared_ptr<Shape> ShapeSpecificCopy() override {
				return Shape::MakeShared<TestShape>(*this);
			}
		};

	public:
		TEST_METHOD(DefaultTransform) {
			TestShape s;
			Assert::IsTrue(s.GetTransformCopy() == Matrix::IndentityMatrix4x4());
		}

		TEST_METHOD(SetTransformation) {
			TestShape s;
			Transform t = Transform::CreateTranslation(2.0, 3.0, 4.0);

			s.SetTransform(t);

			Assert::IsTrue(s.GetTransformCopy() == t);
		}

		TEST_METHOD(DefaultMaterial) {
			TestShape s;
			Material m;

			m.pattern = s.GetMaterial().pattern;

			Assert::IsTrue(m.pattern->ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == Color(1.0f, 1.0f, 1.0f));

			Assert::IsTrue(s.GetMaterial() == m);
		}

		TEST_METHOD(SetMaterial) {
			TestShape s;
			Material m;

			m.ambient = 1.0f;

			s.SetMaterial(m);

			Assert::IsTrue(s.GetMaterial() == m);
		}

		TEST_METHOD(ParentAttribute) {
			auto s = Shape::MakeShared<TestShape>();
			Assert::IsTrue(s->GetParent() == nullptr);
		}

		TEST_METHOD(ScaledIntersections) {
			Ray ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0));
			TestShape s;

			s.SetTransform(Transform::CreateScale(2.0, 2.0, 2.0));

			auto xs = s.FindIntersections(ray);

			Assert::IsTrue(s.receivedRay.origin == Point::CreatePoint(0.0, 0.0, -2.5));
			Assert::IsTrue(s.receivedRay.direction == Vector::CreateVector(0.0, 0.0, 0.5));
		}

		TEST_METHOD(TranslatedIntersections) {
			Ray ray(Point::CreatePoint(0.0, 0.0, -5.0), Vector::CreateVector(0.0, 0.0, 1.0));
			TestShape s;

			s.SetTransform(Transform::CreateTranslation(5.0, 0.0, 0.0));

			auto xs = s.FindIntersections(ray);

			Assert::IsTrue(s.receivedRay.origin == Point::CreatePoint(-5.0, 0.0, -5.0));
			Assert::IsTrue(s.receivedRay.direction == Vector::CreateVector(0.0, 0.0, 1.0));
		}

		TEST_METHOD(TranslatedNormal) {
			TestShape s;
			s.SetTransform(Transform::CreateTranslation(0.0, 1.0, 0.0));

			Assert::IsTrue(
				s.SurfaceNormal(Point::CreatePoint(0.0, 1.70711, -0.70711))
				==
				Vector::CreateVector(0.0, 0.70711, -0.70711)
			);
		}

		TEST_METHOD(TransformedNormal) {
			TestShape s;
			s.SetTransform(Transform::CreateScale(1.0, 0.5, 1.0)
				* Transform::CreateRotationZ(Constants::PI / 5.0)
			);

			Assert::IsTrue(
				s.SurfaceNormal(Point::CreatePoint(0.0, sqrt(2.0)/2.0, -sqrt(2.0) / 2.0))
				==
				Vector::CreateVector(0.0, 0.97014, -0.24254)
			);
		}

		TEST_METHOD(WorldToObjectSpace) {
			auto g1 = Shape::MakeShared<ShapeGroup>();
			auto g2 = Shape::MakeShared<ShapeGroup>();
			auto s = Shape::MakeShared<Sphere>();

			g1->SetTransform(Transform::CreateRotationY(Constants::PI / 2.0));
			g2->SetTransform(Transform::CreateScale(2.0, 2.0, 2.0));
			s->SetTransform(Transform::CreateTranslation(5.0, 0.0, 0.0));

			g1->AddShape(g2);
			g2->AddShape(s);

			Point p = s->PointToObjectSpace(Point::CreatePoint(-2.0, 0.0, -10.0));
			Assert::IsTrue(p == Point::CreatePoint(0.0, 0.0, -1.0));
		}

		TEST_METHOD(ObjectToWorldSpace) {
			auto g1 = Shape::MakeShared<ShapeGroup>();
			auto g2 = Shape::MakeShared<ShapeGroup>();
			auto s = Shape::MakeShared<Sphere>();

			g1->SetTransform(Transform::CreateRotationY(Constants::PI / 2.0));
			g2->SetTransform(Transform::CreateScale(1.0, 2.0, 3.0));
			s->SetTransform(Transform::CreateTranslation(5.0, 0.0, 0.0));

			g1->AddShape(g2);
			g2->AddShape(s);

			Vector v = s->NormalToWorldSpace(Vector::CreateVector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
			Assert::IsTrue(v == Vector::CreateVector(0.285714, 0.428571, -0.857142));
		}

		TEST_METHOD(SurfaceNormalInGroup) {
			auto g1 = Shape::MakeShared<ShapeGroup>();
			auto g2 = Shape::MakeShared<ShapeGroup>();
			auto s = Shape::MakeShared<Sphere>();

			g1->SetTransform(Transform::CreateRotationY(Constants::PI / 2.0));
			g2->SetTransform(Transform::CreateScale(1.0, 2.0, 3.0));
			s->SetTransform(Transform::CreateTranslation(5.0, 0.0, 0.0));

			g1->AddShape(g2);
			g2->AddShape(s);

			Vector v = s->SurfaceNormal(Point::CreatePoint(1.7321, 1.1547, -5.5774));
			Assert::IsTrue(v == Vector::CreateVector(0.285703, 0.428543, -0.857160));
		}
	};
}
