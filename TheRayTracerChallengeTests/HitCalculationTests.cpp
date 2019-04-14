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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(HitCalculationTests)
	{
	public:
		TEST_METHOD(NormalIntersection) {
			Ray r(Point::CreatePoint(0.0f, 0.0f, -5.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));
			
			auto shape = std::make_shared<Sphere>();
			shape->SetID(0);

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);

			IntersectionBuffer i = shape->FindIntersections(r);

			HitCalculations comps(i, r, shapes);

			Assert::IsTrue(Constants::FloatEqual(comps.t, i.GetFirstHit().t));
			Assert::IsTrue(comps.shapeID == shape->GetID());
			Assert::IsTrue(comps.point == Point::CreatePoint(0.0f, 0.0f, -1.0f));
			Assert::IsTrue(comps.eyeVector == Vector::CreateVector(0.0f, 0.0f, -1.0f));
			Assert::IsTrue(comps.normalVector == Vector::CreateVector(0.0f, 0.0f, -1.0f));
		}


		TEST_METHOD(HitOutsideOfShape) {
			Ray r(Point::CreatePoint(0.0f, 0.0f, -5.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));

			auto shape = std::make_shared<Sphere>();
			shape->SetID(0);

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);



			IntersectionBuffer i(Intersection(4.0f, shape->GetID()));

			HitCalculations comps(i, r, shapes);

			Assert::IsTrue(comps.insideShape == false);
		}

		TEST_METHOD(HitInsideOfShape) {
			Ray r(Point::CreatePoint(0.0f, 0.0f, 0.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));

			auto shape = std::make_shared<Sphere>();
			shape->SetID(0);

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);



			IntersectionBuffer i(Intersection(1.0f, shape->GetID()));

			HitCalculations comps(i, r, shapes);

			Assert::IsTrue(comps.insideShape == true);
			Assert::IsTrue(comps.point == Point::CreatePoint(0.0f, 0.0f, 1.0f));
			Assert::IsTrue(comps.eyeVector == Vector::CreateVector(0.0f, 0.0f, -1.0f));
			Assert::IsTrue(comps.normalVector == Vector::CreateVector(0.0f, 0.0f, -1.0f));
		}
	};
}
