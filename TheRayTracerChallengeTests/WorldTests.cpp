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
            m.color = Color(0.8f, 1.0f, 0.6f);
            m.diffuse = 0.7f;
            m.specular = 0.2f;

            Sphere s1;
            s1.SetMaterial(m);

            Sphere s2;
            s2.SetTransform(Transform::CreateScale(0.5f, 0.5f, 0.5f));

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
            Assert::IsTrue(Constants::FloatEqual(i[0].t, 4.0f));
            Assert::IsTrue(Constants::FloatEqual(i[1].t, 4.5f));
            Assert::IsTrue(Constants::FloatEqual(i[2].t, 5.5f));
            Assert::IsTrue(Constants::FloatEqual(i[3].t, 6.0f));
        }

    };
}
