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
using namespace RayTracer;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(MaterialTests)
    {
    public:
        TEST_METHOD(DefaultMaterial) {
            Material m;

            Assert::IsTrue(m.pattern->ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == Color(1.0, 1.0, 1.0));
            Assert::IsTrue(Constants::DoubleEqual(m.ambient, 0.1));
            Assert::IsTrue(Constants::DoubleEqual(m.diffuse, 0.9));
            Assert::IsTrue(Constants::DoubleEqual(m.specular, 0.9));
            Assert::IsTrue(Constants::DoubleEqual(m.shininess, 200.0));
            Assert::IsTrue(Constants::DoubleEqual(m.reflective, 0.0));
            Assert::IsTrue(Constants::DoubleEqual(m.transparency, 0.0));
            Assert::IsTrue(Constants::DoubleEqual(m.refractiveIndex, 1.0));
        }

		TEST_METHOD(GlassMaterialTests) {
			Material m = Material::CreateGlass();

			Assert::IsTrue(Constants::DoubleEqual(m.transparency, 1.0));
			Assert::IsTrue(Constants::DoubleEqual(m.refractiveIndex, 1.5));
			
		}
    };
}
