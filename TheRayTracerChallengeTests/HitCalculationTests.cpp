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
			
			auto shape = std::make_shared<Sphere>();
			shape->SetID(0);

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);

			IntersectionBuffer i = shape->FindIntersections(r);

			HitCalculations comps(i.GetFirstHit(), i, r, shapes);

			Assert::IsTrue(Constants::DoubleEqual(comps.t, i.GetFirstHit().t));
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

			HitCalculations comps(i.GetFirstHit(), i, r, shapes);

			Assert::IsTrue(comps.insideShape == false);
		}

		TEST_METHOD(HitInsideOfShape) {
			Ray r(Point::CreatePoint(0.0f, 0.0f, 0.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));

			auto shape = std::make_shared<Sphere>();
			shape->SetID(0);

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);



			IntersectionBuffer i(Intersection(1.0f, shape->GetID()));

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

			auto shape = std::make_shared<Sphere>();
			shape->SetTransform(Transform::CreateTranslation(0.0, 0.0, 1.0));
			shape->SetID(0);
			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);

			IntersectionBuffer i(Intersection(5.0f, shape->GetID()));

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

			std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>();
			sphere->SetTransform(Transform::CreateTranslation(0.0, 0.0, 1.0));
			sphere->SetMaterial(Material::CreateGlass());
			sphere->SetID(0);

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(sphere);


			Intersection i(5.0, sphere->GetID());
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
			shapes.push_back(std::make_shared<Plane>());

			IntersectionBuffer xs(Intersection(sqrt(2.0), 0));
			HitCalculations hit(xs.GetFirstHit(), xs, ray, shapes);

			Assert::IsTrue(
				hit.reflectionVector
				==
				Vector::CreateVector(0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0)
			);
		}

		TEST_METHOD(FindingRefractiveIndex) {
			//Shapes
			auto A = std::make_shared<Sphere>();
			auto materialA = Material::CreateGlass();
			materialA.refractiveIndex = 1.5;
			A->SetMaterial(materialA);
			A->SetTransform(Transform::CreateScale(2.0, 2.0, 2.0));
			A->SetID(0);

			auto B = std::make_shared<Sphere>();
			auto materialB = Material::CreateGlass();
			materialB.refractiveIndex = 2.0;
			B->SetMaterial(materialB);
			B->SetTransform(Transform::CreateTranslation(0.0, 0.0, -0.25));
			B->SetID(1);

			auto C = std::make_shared<Sphere>();
			auto materialC = Material::CreateGlass();
			materialC.refractiveIndex = 2.5;
			C->SetMaterial(materialC);
			C->SetTransform(Transform::CreateTranslation(0.0, 0.0, 0.25));
			C->SetID(2);

			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(A);
			shapes.push_back(B);
			shapes.push_back(C);



			Ray ray(
				Point::CreatePoint(0.0, 0.0, -4.0),
				Vector::CreateVector(0.0, 0.0, 1.0)
			);

			//Simulated intersections
			std::vector<Intersection> testIntersections;
			testIntersections.push_back(Intersection(2.0, A->GetID()));
			testIntersections.push_back(Intersection(2.75, B->GetID()));
			testIntersections.push_back(Intersection(3.25, C->GetID()));
			testIntersections.push_back(Intersection(4.75, B->GetID()));
			testIntersections.push_back(Intersection(5.25, C->GetID()));
			testIntersections.push_back(Intersection(6.0, A->GetID()));

			//Buffer with all intersections
			IntersectionBuffer intersections;
			for (auto i : testIntersections) {
				intersections.Add(i);
			}

			//Expected results
			std::vector<double> n1 = {1.0, 1.5, 2.0, 2.5, 2.5, 1.5};
			std::vector<double> n2 = {1.5, 2.0, 2.5, 2.5, 1.5, 1.0};

			//6 test cases
			for (size_t testNr = 0; testNr < 6; testNr++) {
				Assert::IsTrue(testNr < 12);

				//Calculate the current test case
				HitCalculations hit(testIntersections[testNr], intersections, ray, shapes);

				//Validate the results
				Assert::IsTrue(Constants::DoubleEqual(hit.refractiveIndex1, n1[testNr]));
				Assert::IsTrue(Constants::DoubleEqual(hit.refractiveIndex2, n2[testNr]));
			}
		}

		TEST_METHOD(SchlickApproximation_TotalInternalReflection) {
			auto shape = std::make_shared<Sphere>();
			shape->SetID(0);
			shape->SetMaterial(Material::CreateGlass());
			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);

			Ray ray(
				Point::CreatePoint(0.0, 0.0, sqrt(2.0) / 2.0),
				Vector::CreateVector(0.0, 1.0, 0.0)
			);

			IntersectionBuffer xs(Intersection(-sqrt(2.0) / 2.0, shape->GetID()), Intersection(sqrt(2.0) / 2.0, shape->GetID()));

			HitCalculations hit(xs[1], xs, ray, shapes);
			Assert::IsTrue(Constants::DoubleEqual(hit.SchlickApproximation(), 1.0));
		}

		TEST_METHOD(SchlickApproximation_PerpendicularView) {
			auto shape = std::make_shared<Sphere>();
			shape->SetID(0);
			shape->SetMaterial(Material::CreateGlass());
			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);

			Ray ray(
				Point::CreatePoint(0.0, 0.0, 0.0),
				Vector::CreateVector(0.0, 1.0, 0.0)
			);

			IntersectionBuffer xs(Intersection(-1.0, shape->GetID()), Intersection(1.0, shape->GetID()));

			HitCalculations hit(xs[1], xs, ray, shapes);
			Assert::IsTrue(Constants::DoubleEqual(hit.SchlickApproximation(), 0.04));
		}

		TEST_METHOD(SchlickApproximation_N1LESSTHANN2) {
			auto shape = std::make_shared<Sphere>();
			shape->SetID(0);
			shape->SetMaterial(Material::CreateGlass());
			std::vector<std::shared_ptr<Shape>> shapes;
			shapes.push_back(shape);

			Ray ray(
				Point::CreatePoint(0.0, 0.99, -2.0),
				Vector::CreateVector(0.0, 0.0, 1.0)
			);

			IntersectionBuffer xs(Intersection(1.8589, shape->GetID()));

			HitCalculations hit(xs[0], xs, ray, shapes);
			Assert::IsTrue(Constants::DoubleEqual(hit.SchlickApproximation(), 0.48873));
		}
	};
}
