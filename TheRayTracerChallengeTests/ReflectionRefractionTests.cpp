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
#include <World.h>
#include <HitCalculations.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(ReflectionRefractionTests)
	{
	public:
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
			std::vector<double> n1 = { 1.0, 1.5, 2.0, 2.5, 2.5, 1.5 };
			std::vector<double> n2 = { 1.5, 2.0, 2.5, 2.5, 1.5, 1.0 };

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
		TEST_METHOD(ReflectedColor) {
			World w;
			w.LoadDefaultWorld();

			std::shared_ptr<Plane> plane = std::make_shared<Plane>();
			Material material;
			material.reflective = 0.5;
			plane->SetMaterial(material);
			plane->SetTransform(Transform::CreateTranslation(0.0, -1.0, 0.0));
			w.AddShape(plane);

			Ray ray(
				Point::CreatePoint(0.0, 0.0, -3.0),
				Vector::CreateVector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0)
			);

			IntersectionBuffer intersections(Intersection(sqrt(2.0), plane->GetID()));
			HitCalculations hit(intersections.GetFirstHit(), intersections, ray, w.shapes);

			Color c = w.FindReflectedColor(hit);

			Assert::IsTrue(
				c
				==
				Color(0.19033, 0.23791, 0.142749)
			);
		}

		TEST_METHOD(ReflectiveHit) {
			World w;
			w.LoadDefaultWorld();

			auto plane = std::make_shared<Plane>();
			Material m;
			m.reflective = 0.5;
			plane->SetMaterial(m);
			plane->SetTransform(Transform::CreateTranslation(0.0, -1.0, 0.0));
			w.AddShape(plane);

			Ray ray(
				Point::CreatePoint(0.0, 0.0, -3.0),
				Vector::CreateVector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0)
			);

			IntersectionBuffer intersections(Intersection(sqrt(2.0), plane->GetID()));
			HitCalculations hit(intersections.GetFirstHit(), intersections, ray, w.shapes);

			Color shadedColor = w.ShadeHit(hit);
			Assert::IsTrue(shadedColor == Color(0.87676, 0.92435, 0.82917));
		}

		TEST_METHOD(AvoidInfiniteReflectionRecursion) {
			//Two fully reflective planes -> light keeps being reflected
			World w;

			auto plane1 = std::make_shared<Plane>();
			auto plane2 = std::make_shared<Plane>();

			Material m;
			m.reflective = 1.0;

			plane1->SetMaterial(m);
			plane2->SetMaterial(m);

			plane1->SetTransform(Transform::CreateTranslation(0.0, -1.0, 0.0));
			plane2->SetTransform(Transform::CreateTranslation(0.0, 1.0, 0.0));

			w.AddShape(plane1);
			w.AddShape(plane2);

			Ray ray(
				Point::CreatePoint(0.0, 0.0, 0.0),
				Vector::CreateVector(0.0, 1.0, 0.0)
			);

			//This function call must terminate
			Color c = w.FindRayColor(ray);

			//If execution reaches this point, the test should pass
			Assert::IsTrue(true);
		}

		TEST_METHOD(LimitRecursion) {
			World w;
			w.LoadDefaultWorld();

			auto plane = std::make_shared<Plane>();
			Material m;
			m.reflective = 0.5;
			plane->SetMaterial(m);
			plane->SetTransform(Transform::CreateTranslation(0.0, -1.0, 0.0));
			w.AddShape(plane);

			Ray ray(
				Point::CreatePoint(0.0, 0.0, -3.0),
				Vector::CreateVector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0)
			);

			IntersectionBuffer intersections(Intersection(sqrt(2.0), plane->GetID()));
			HitCalculations hit(intersections.GetFirstHit(), intersections, ray, w.shapes);

			Color reflectedColor = w.FindReflectedColor(hit, 0);
			//Recursion should stop and return black
			Assert::IsTrue(reflectedColor == Color(0.0, 0.0, 0.0));
		}

		TEST_METHOD(NoRefraction) {
			World w;
			w.LoadDefaultWorld();

			auto shape = w.shapes[0];

			Ray ray(
				Point::CreatePoint(0.0, 0.0, -5.0),
				Vector::CreateVector(0.0, 0.0, 1.0)
			);

			IntersectionBuffer xs(Intersection(4.0, shape->GetID()), Intersection(6.0, shape->GetID()));

			HitCalculations hit(xs[0], xs, ray, w.shapes);

			Color c = w.FindRefractedColor(hit);

			//Shape is not transparent -> color should be black
			Assert::IsTrue(c == Color(0.0, 0.0, 0.0));
		}

		TEST_METHOD(AvoidInfiniteRefractionRecursion) {
			World w;
			w.LoadDefaultWorld();

			auto shape = w.shapes[0];
			shape->SetMaterial(Material::CreateGlass());

			Ray ray(
				Point::CreatePoint(0.0, 0.0, -5.0),
				Vector::CreateVector(0.0, 0.0, 1.0)
			);

			IntersectionBuffer xs(Intersection(4.0, shape->GetID()), Intersection(6.0, shape->GetID()));

			HitCalculations hit(xs[0], xs, ray, w.shapes);

			Color c = w.FindRefractedColor(hit, 0);

			//Shape is not transparent -> color should be black
			Assert::IsTrue(c == Color(0.0, 0.0, 0.0));
		}

		TEST_METHOD(TotalInternalReflection) {
			World w;
			w.LoadDefaultWorld();

			auto shape = w.shapes[0];
			shape->SetMaterial(Material::CreateGlass());

			Ray ray(
				Point::CreatePoint(0.0, 0.0, sqrt(2.0) / 2.0),
				Vector::CreateVector(0.0, 1.0, 0.0)
			);

			IntersectionBuffer xs(Intersection(-sqrt(2.0) / 2.0, shape->GetID()), Intersection(sqrt(2.0) / 2.0, shape->GetID()));

			HitCalculations hit(xs[1], xs, ray, w.shapes);

			Color c = w.FindRefractedColor(hit, 5);

			//Total internal relfection -> ray stays inside the sphere -> black
			Assert::IsTrue(c == Color(0.0, 0.0, 0.0));
		}

		TEST_METHOD(RefractedColor) {
			//Pattern  that returns the point it received as a color
			class TestPattern : public Pattern {
			public:
				Color ReadPattern(Point point) override {
					return Color(point.x, point.y, point.z);
				}
			};

			World w;
			w.LoadDefaultWorld();
			auto A = w.shapes[0];
			Material mA;
			mA.ambient = 1.0;
			mA.pattern = std::make_shared<TestPattern>();
			A->SetMaterial(mA);

			auto B = w.shapes[1];
			B->SetMaterial(Material::CreateGlass());

			Ray ray(
				Point::CreatePoint(0.0, 0.0, 0.1),
				Vector::CreateVector(0.0, 1.0, 0.0)
			);

			IntersectionBuffer xs;
			xs.Add(Intersection(-0.9899, A->GetID()));
			xs.Add(Intersection(-0.4899, B->GetID()));
			xs.Add(Intersection(0.4899, B->GetID()));
			xs.Add(Intersection(0.9899, A->GetID()));

			HitCalculations hit(xs[2], xs, ray, w.shapes);

			Color c = w.FindRefractedColor(hit);

			Assert::IsTrue(
				c == Color(0.0, 0.99887, 0.04721)
			);
		}

		TEST_METHOD(ShadedRefraction) {
			World w;
			w.LoadDefaultWorld();

			auto floor = std::make_shared<Plane>();
			floor->SetTransform(Transform::CreateTranslation(0.0, -1.0, 0.0));
			Material floorMaterial;
			floorMaterial.reflective = 0.5;
			floorMaterial.transparency = 0.5;
			floorMaterial.refractiveIndex = 1.5;
			floor->SetMaterial(floorMaterial);
			w.AddShape(floor);

			auto ball = std::make_shared<Sphere>();
			Material ballMaterial;
			ballMaterial.pattern = std::make_shared<ColorPattern>(Color(1.0, 0.0, 0.0));
			ballMaterial.ambient = 0.5;
			ball->SetMaterial(ballMaterial);
			ball->SetTransform(Transform::CreateTranslation(0.0, -3.5, -0.5));
			w.AddShape(ball);

			Ray ray(
				Point::CreatePoint(0.0, 0.0, -3.0),
				Vector::CreateVector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0)
			);

			IntersectionBuffer xs(Intersection(sqrt(2), floor->GetID()));
			HitCalculations hit(xs[0], xs, ray, w.shapes);

			Color c = w.ShadeHit(hit, 5);
			Assert::IsTrue(
				c
				==
				Color(0.93391, 0.69643, 0.69243)
			);

		}
	};
}
