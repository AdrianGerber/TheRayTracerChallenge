#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <Intersection.h>
#include <Transform.h>
#include <memory>
#include <Sphere.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(SphereTests)
    {
    public:
        TEST_METHOD(SurfaceNormals) {
            Sphere s;

            //Normal vectors at different points
            Assert::IsTrue(s.SurfaceNormal(Point::CreatePoint(1.0f, 0.0f, 0.0f)) == Vector::CreateVector(1.0f, 0.0f, 0.0f));
            Assert::IsTrue(s.SurfaceNormal(Point::CreatePoint(0.0f, 1.0f, 0.0f)) == Vector::CreateVector(0.0f, 1.0f, 0.0f));
            Assert::IsTrue(s.SurfaceNormal(Point::CreatePoint(0.0f, 0.0f, 1.0f)) == Vector::CreateVector(0.0f, 0.0f, 1.0f));
            float result = sqrtf(3.0f) / 3.0f;
            Assert::IsTrue(
                s.SurfaceNormal(Point::CreatePoint(result, result, result))
                == Vector::CreateVector(result, result, result)
            );

            //Normal vectors should be normalized
            Vector v = s.SurfaceNormal(Point::CreatePoint(result, result, result));
            Assert::IsTrue(v == v.Normalize());
        }

        TEST_METHOD(TransformedNormals) {
            Sphere s;
            //Translated Sphere
            s.SetTransform(Transform::CreateTranslation(0.0f, 1.0f, 0.0f));
            Assert::IsTrue(
                s.SurfaceNormal(Point::CreatePoint(0.0f, 1.70711f, -0.70711f))
                == Vector::CreateVector(0.0f, 0.70711f, -0.70711f)
            );

            //Transformed Sphere
            s.SetTransform(
                Transform::CreateScale(1.0f, 0.5f, 1.0f) * Transform::CreateRotationZ(Constants::PI / 5.0f)
            );
            Assert::IsTrue(
                s.SurfaceNormal(Point::CreatePoint(0.0f, sqrtf(2.0f) / 2.0f, -sqrtf(2.0f) / 2.0f))
                == Vector::CreateVector(0.0f, 0.97014f, -0.24254f)
            );
        }

        TEST_METHOD(SphereMaterial) {
            Sphere s;
            //Default material
            Material m;
            Assert::IsTrue(s.GetMaterial() == m);


            //Other material
            m.ambient = 1.0f;
            s.SetMaterial(m);

            Assert::IsTrue(s.GetMaterial() == m);
        }
    };
}
