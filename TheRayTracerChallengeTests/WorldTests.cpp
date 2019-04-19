#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <Intersection.h>
#include <Transform.h>
#include <memory>
#include <Shape.h>
#include <Sphere.h>
#include <Plane.h>
#include <World.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(WorldTests)
	{
	public:
		TEST_METHOD(Initialisation) {
			World world;

			Assert::IsTrue(world.lightSources.size() == 0);
			Assert::IsTrue(world.shapes.size() == 0);
		}
		TEST_METHOD(DefaultWorld) {
			World world;
			world.LoadDefaultWorld();

			LightSource l(Point::CreatePoint(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

			Material m;
			m.pattern = world.shapes[0]->GetMaterial().pattern;
			m.diffuse = 0.7f;
			m.specular = 0.2f;

			Sphere s1;
			s1.SetMaterial(m);
			s1.SetID(0);

			Material m2;
			m2.pattern = world.shapes[1]->GetMaterial().pattern;
			Sphere s2;
			s2.SetTransform(Transform::CreateScale(0.5f, 0.5f, 0.5f));
			s2.SetMaterial(m2);
			s2.SetID(1);

			Assert::IsTrue(world.shapes.size() == 2);
			Assert::IsTrue(world.lightSources.size() == 1);

			Assert::IsTrue(*world.shapes[0] == s1);
			Assert::IsTrue(*world.shapes[1] == s2);
			Assert::IsTrue(*world.lightSources[0] == l);
		}
		TEST_METHOD(Intersections) {
			World world;
			world.LoadDefaultWorld();
			Ray ray(Point::CreatePoint(0.0f, 0.0f, -5.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));

			auto i = world.IntersectRay(ray);

			Assert::IsTrue(i.GetCount() == 4);
			Assert::IsTrue(Constants::DoubleEqual(i[0].t, 4.0f));
			Assert::IsTrue(Constants::DoubleEqual(i[1].t, 4.5f));
			Assert::IsTrue(Constants::DoubleEqual(i[2].t, 5.5f));
			Assert::IsTrue(Constants::DoubleEqual(i[3].t, 6.0f));
		}

		TEST_METHOD(Shading) {
			World world;
			world.LoadDefaultWorld();

			Ray ray(Point::CreatePoint(0.0f, 0.0f, -5.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));
			auto shape = world.shapes[0];

			IntersectionBuffer intersections(Intersection(4.0f, shape->GetID()));
			HitCalculations hit(intersections, ray, world.shapes);

			Assert::IsTrue(world.ShadeHit(hit) == Color(0.38066f, 0.47583f, 0.2855f));
		}
	
		TEST_METHOD(InsideShading) {
			World world;
			world.LoadDefaultWorld();

			world.lightSources[0]->SetIntensity(Color(1.0f, 1.0f, 1.0f));
			world.lightSources[0]->SetPosition(Point::CreatePoint(0.0f, 0.25f, 0.0f));


			Ray ray(Point::CreatePoint(0.0f, 0.0f, 0.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));
			
			auto shape = world.shapes[1];

			IntersectionBuffer intersections(Intersection(0.5f, shape->GetID()));
			HitCalculations hit(intersections, ray, world.shapes);

			Assert::IsTrue(world.ShadeHit(hit) == Color(0.90498f, 0.90498f, 0.90498f));
		}

		TEST_METHOD(RayNoHit) {
			World world;
			world.LoadDefaultWorld();

			Ray ray(Point::CreatePoint(0.0f, 0.0f, -5.0f), Vector::CreateVector(0.0f, 1.0f, 0.0f));

			Assert::IsTrue(
				world.FindRayColor(ray) == Color(0.0f, 0.0f, 0.0f)
			);
		}
		TEST_METHOD(RayHit) {
			World world;
			world.LoadDefaultWorld();

			Ray ray(Point::CreatePoint(0.0f, 0.0f, -5.0f), Vector::CreateVector(0.0f, 0.0f, 1.0f));

			Assert::IsTrue(
				world.FindRayColor(ray) == Color(0.38066f, 0.47583f, 0.2855f)
			);
		}

		TEST_METHOD(RayUsesCorrectHit) {
			World world;
			world.LoadDefaultWorld();

			auto outer = world.shapes[0];
			auto inner = world.shapes[1];

			Material m;

			m = outer->GetMaterial();
			m.ambient = 1.0f;
			outer->SetMaterial(m);

			m = inner->GetMaterial();
			m.ambient = 1.0f;
			inner->SetMaterial(m);



			Ray ray(Point::CreatePoint(0.0f, 0.0f, 0.75f), Vector::CreateVector(0.0f, 0.0f, -1.0f));

			Assert::IsTrue(
				world.FindRayColor(ray) == inner->GetMaterial().pattern->ColorAtPoint(Point::CreatePoint(0.0, 0.0, 0.0), Transform())
			);
		}

		TEST_METHOD(PointNotInShadow) {
			World w;
			w.LoadDefaultWorld();
			Point p = Point::CreatePoint(0.0f, 10.0f, 0.0f);

			//No shape between light source and point
			Assert::IsFalse(
				w.PointIsInShadow(
					w.lightSources[0],
					p
				)
			);
		}

		TEST_METHOD(PointInShadow) {
			World w;
			w.LoadDefaultWorld();
			Point p = Point::CreatePoint(10.0f, -10.0f, 10.0f);
		
			//Point in shadow of a shape
			Assert::IsTrue(
				w.PointIsInShadow(
					w.lightSources[0],
					p
				)
			);
		}

		TEST_METHOD(PointBehindLightSource) {
			World w;
			w.LoadDefaultWorld();
			Point p = Point::CreatePoint(-20.0f, 20.0f, -20.0f);

			//Point not in shadow, but behind the light source
			Assert::IsFalse(
				w.PointIsInShadow(
					w.lightSources[0],
					p
				)
			);
		}

		TEST_METHOD(PointInFrontOfObject) {
			World w;
			w.LoadDefaultWorld();
			Point p = Point::CreatePoint(-2.0f, 2.0f, -2.0f);

			//Point not in shadow, but in front of an object
			Assert::IsFalse(
				w.PointIsInShadow(
					w.lightSources[0],
					p
				)
			);
		}

		TEST_METHOD(IntersectionInShadow) {
			World w;
			w.AddLightSource(std::make_shared<LightSource>());
			w.lightSources[0]->SetIntensity(Color(1.0f, 1.0f, 1.0f));
			w.lightSources[0]->SetPosition(Point::CreatePoint(0.0f, 0.0f, -10.0f));

			w.AddShape(std::make_shared<Sphere>());
			w.AddShape(std::make_shared<Sphere>());
			w.shapes[1]->SetTransform(Transform::CreateTranslation(0.0f, 0.0f, 10.0f));
			
			Ray ray(
				Point::CreatePoint(0.0f, 0.0f, 5.0f),
				Vector::CreateVector(0.0f, 0.0f, 1.0f)
			);

			Intersection intersection(4.0f, w.shapes[1]->GetID());
			IntersectionBuffer intersections(intersection);

			HitCalculations comps(intersections, ray, w.shapes);

			//Sphere in shadow is correctly shaded
			Assert::IsTrue(
				w.ShadeHit(comps)
				==
				Color(0.1f, 0.1f, 0.1f)
			);
		}

		TEST_METHOD(HitUnreflectiveShape) {
			World w;
			w.LoadDefaultWorld();
			Ray ray(Point::CreatePoint(0.0, 0.0, 0.0),
				Vector::CreateVector(0.0, 0.0, 1.0));


			auto shape = w.shapes[1];
			auto material = shape->GetMaterial();
			material.ambient = 1.0;
			shape->SetMaterial(material);

			IntersectionBuffer intersections(Intersection(1.0, shape->GetID()));

			HitCalculations hit(intersections, ray, w.shapes);

			Assert::IsTrue(
				w.FindReflectedColor(hit)
				==
				Color(0.0, 0.0, 0.0)
			);
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
			HitCalculations hit(intersections, ray, w.shapes);

			Color c = w.FindReflectedColor(hit);

			Assert::IsTrue(
				c
				==
				Color(0.19033, 0.23791, 0.142749)
			);
		}

		TEST_METHOD(ReflectiveShadeHit) {
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
			HitCalculations hit(intersections, ray, w.shapes);
			
			Color shadedColor = w.ShadeHit(hit);
			Assert::IsTrue(shadedColor == Color(0.87676, 0.92435, 0.82917));
		}

		TEST_METHOD(AvoidInfiniteRecursion){
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
			HitCalculations hit(intersections, ray, w.shapes);

			Color reflectedColor = w.FindReflectedColor(hit, 0);
			//Recursion should stop and return black
			Assert::IsTrue(reflectedColor == Color(0.0, 0.0, 0.0));
		}
    };
}
