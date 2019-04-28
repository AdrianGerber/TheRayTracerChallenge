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
#include <Plane.h>
#include <LightSource.h>
#include <HitCalculations.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(HitCalculationTests)
	{
	public:
		TEST_METHOD(NormalIntersection) {
			Ray r(Point::CreatePoint(0.0f, 0.0f, -5.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));
			
			auto shape = Shape::MakeShared<Sphere>();

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);

			IntersectionBuffer i;
			shape->FindIntersections(r, i);

			HitCalculations comps(i.GetFirstHit(), i, r, shapes);

			Assert::IsTrue(Constants::DoubleEqual(comps.t, i.GetFirstHit().t));
			Assert::IsTrue(comps.shape == shape);
			Assert::IsTrue(comps.point == Point::CreatePoint(0.0f, 0.0f, -1.0f));
			Assert::IsTrue(comps.eyeVector == Vector::CreateVector(0.0f, 0.0f, -1.0f));
			Assert::IsTrue(comps.normalVector == Vector::CreateVector(0.0f, 0.0f, -1.0f));
		}


		TEST_METHOD(HitOutsideOfShape) {
			Ray r(Point::CreatePoint(0.0f, 0.0f, -5.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));

			auto shape = Shape::MakeShared<Sphere>();

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);



			IntersectionBuffer i(Intersection(4.0f,shape));

			HitCalculations comps(i.GetFirstHit(), i, r, shapes);

			Assert::IsTrue(comps.insideShape == false);
		}

		TEST_METHOD(HitInsideOfShape) {
			Ray r(Point::CreatePoint(0.0f, 0.0f, 0.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));

			auto shape = Shape::MakeShared<Sphere>();


			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);



			IntersectionBuffer i(Intersection(1.0f, shape));

			HitCalculations comps(i.GetFirstHit(), i, r, shapes);

			Assert::IsTrue(comps.insideShape == true);
			Assert::IsTrue(comps.point == Point::CreatePoint(0.0f, 0.0f, 1.0f));
			Assert::IsTrue(comps.eyeVector == Vector::CreateVector(0.0f, 0.0f, -1.0f));
			Assert::IsTrue(comps.normalVector == Vector::CreateVector(0.0f, 0.0f, -1.0f));
		}

		TEST_METHOD(OverPoint) {
			Ray ray(
				Point::CreatePoint(0.0f, 0.0f, -5.0f),
				Vector::CreateVector(0.0f, 0.0f, 1.0f)
			);

			auto shape = Shape::MakeShared<Sphere>();
			shape->SetTransform(Transform::CreateTranslation(0.0, 0.0, 1.0));
			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);

			IntersectionBuffer i(Intersection(5.0f, shape));

			HitCalculations comps(i.GetFirstHit(), i, ray, shapes);

			//OverPoint is correctly adjusted, so floating point errors are compensated
			Assert::IsTrue(comps.overPoint.z < (-Constants::EPSILON / 2.0f));
			Assert::IsTrue(comps.point.z > comps.overPoint.z);
		}

		TEST_METHOD(UnderPoint) {
			Ray ray(
				Point::CreatePoint(0.0, 0.0, -5.0),
				Vector::CreateVector(0.0, 0.0, 1.0)
			);

			auto sphere = Shape::MakeShared<Sphere>();

			sphere->SetTransform(Transform::CreateTranslation(0.0, 0.0, 1.0));
			sphere->SetMaterial(Material::CreateGlass());
			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(sphere);


			Intersection i(5.0, sphere);
			IntersectionBuffer xs(i);

			HitCalculations hit(i, xs, ray, shapes);

			Assert::IsTrue(hit.underPoint.z > (Constants::EPSILON / 2.0));
			Assert::IsTrue(hit.underPoint.z > hit.point.z);

		}

		TEST_METHOD(ReflectionVector) {
			Ray ray(
				Point::CreatePoint(0.0, 1.0, -1.0),
				Vector::CreateVector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0)
			);

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(Shape::MakeShared<Plane>());

			IntersectionBuffer xs(Intersection(sqrt(2.0), shapes[0]));
			HitCalculations hit(xs.GetFirstHit(), xs, ray, shapes);

			Assert::IsTrue(
				hit.reflectionVector
				==
				Vector::CreateVector(0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0)
			);
		}
	};
}
