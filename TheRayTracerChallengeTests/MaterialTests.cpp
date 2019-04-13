#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <Intersection.h>
#include <Transform.h>
#include <memory>
#include <cmath>
#include <Sphere.h>
#include <LightSource.h>
#include <Material.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(MaterialTests)
    {
    public:
        TEST_METHOD(DefaultMaterial) {
            Material m;

            Assert::IsTrue(m.color == Color(1.0f, 1.0f, 1.0f));
            Assert::IsTrue(Constants::FloatEqual(m.ambient, 0.1f));
            Assert::IsTrue(Constants::FloatEqual(m.diffuse, 0.9f));
            Assert::IsTrue(Constants::FloatEqual(m.specular, 0.9f));
            Assert::IsTrue(Constants::FloatEqual(m.shininess, 200.0f));

        }
    };
}