#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Transform.h>
#include <Constants.h>
#include <Tuple.h>
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace RayTracer;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(TransformTests)
    {
    public:

        TEST_METHOD(Translation) {
            
            Transform translation = Transform::CreateTranslation(5.0f, -3.0f, 2.0f);
            Tuple point = Tuple::CreatePoint(-3.0f, 4.0f, 5.0f);
            Tuple vector = Tuple::CreateVector(-3.0f, 4.0f, 5.0f);

            //Translating a point
            Assert::IsTrue(translation * point == Tuple::CreatePoint(2.0f, 1.0f, 7.0f));

            //Translating a vector (no change)
            Assert::IsTrue(vector * translation == vector);

            //Inverse translation
            Assert::IsTrue(translation.Inversion() * point == Tuple::CreatePoint(-8.0f, 7.0f, 3.0f));
        }

        TEST_METHOD(Scaling) {
            Transform scale = Transform::CreateScale(2.0f, 3.0f, 4.0f);
            Tuple point = Tuple::CreatePoint(-4.0f, 6.0f, 8.0f);
            Tuple vector = Tuple::CreateVector(-4.0f, 6.0f, 8.0f);

            //Scaling a point
            Assert::IsTrue(scale * point == Tuple::CreatePoint(-8.0f, 18.0f, 32.0f));
            //Scaling a vector
            Assert::IsTrue(scale * vector == Tuple::CreateVector(-8.0f, 18.0f, 32.0f));
            //Inverse scaling
            Assert::IsTrue(scale.Inversion() * vector == Tuple::CreateVector(-2.0f, 2.0f, 2.0f));
            //Reflection (Scale by negative value)
            Assert::IsTrue(
                Transform::CreateScale(-1.0f, 1.0f, 1.0f) * Tuple::CreatePoint(2.0f, 3.0f, 4.0f)
                == Tuple::CreatePoint(-2.0f, 3.0f, 4.0f)
            );
        }

        TEST_METHOD(RotationX) {
            Tuple p = Tuple::CreatePoint(0.0f, 1.0f, 0.0f);


            //Rotation around x-axis
            Transform halfQuarter = Transform::CreateRotationX(Constants::PI / 4.0f);
            Transform fullQuarter = Transform::CreateRotationX(Constants::PI / 2.0f);

            Assert::IsTrue(halfQuarter * p == Tuple::CreatePoint(0.0f, sqrtf(2.0f) / 2.0f, sqrtf(2.0f) / 2.0f));
            Assert::IsTrue(fullQuarter * p == Tuple::CreatePoint(0.0f, 0.0f, 1.0f));
            Assert::IsTrue(halfQuarter.Inversion() * p == Tuple::CreatePoint(0.0f, sqrtf(2.0f) / 2.0f, -sqrtf(2.0f) / 2.0f));
        }

        TEST_METHOD(RotationY) {
            Tuple p = Tuple::CreatePoint(0.0f, 0.0f, 1.0f);


            //Rotation around y-axis
            Transform halfQuarter = Transform::CreateRotationY(Constants::PI / 4.0f);
            Transform fullQuarter = Transform::CreateRotationY(Constants::PI / 2.0f);

            Assert::IsTrue(halfQuarter * p == Tuple::CreatePoint(sqrtf(2.0f) / 2.0f, 0.0f, sqrtf(2.0f) / 2.0f));
            Assert::IsTrue(fullQuarter * p == Tuple::CreatePoint(1.0f, 0.0f, 0.0f));
        }

        TEST_METHOD(RotationZ) {
            Tuple p = Tuple::CreatePoint(0.0f, 1.0f, 0.0f);


            //Rotation around z-axis
            Transform halfQuarter = Transform::CreateRotationZ(Constants::PI / 4.0f);
            Transform fullQuarter = Transform::CreateRotationZ(Constants::PI / 2.0f);

            Assert::IsTrue(halfQuarter * p == Tuple::CreatePoint(-sqrtf(2.0f) / 2.0f, sqrtf(2.0f) / 2.0f, 0.0f));
            Assert::IsTrue(fullQuarter * p == Tuple::CreatePoint(-1.0f, 0.0f, 0.0f));
        }

        TEST_METHOD(Shearing) {
            Tuple point = Tuple::CreatePoint(2.0f, 3.0f, 4.0f);

            //x in proportion to y
            Assert::IsTrue(
                Transform::CreateShear(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f) * point == Tuple::CreatePoint(5.0f, 3.0f, 4.0f)
            );
            //x in proportion to z
            Assert::IsTrue(
                Transform::CreateShear(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f) * point == Tuple::CreatePoint(6.0f, 3.0f, 4.0f)
            );


            //y in proportion to x
            Assert::IsTrue(
                Transform::CreateShear(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f) * point == Tuple::CreatePoint(2.0f, 5.0f, 4.0f)
            );
            //y in proportion to z
            Assert::IsTrue(
                Transform::CreateShear(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f) * point == Tuple::CreatePoint(2.0f, 7.0f, 4.0f)
            );


            //z in proportion to x
            Assert::IsTrue(
                Transform::CreateShear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f) * point == Tuple::CreatePoint(2.0f, 3.0f, 6.0f)
            );
            //z in proportion to y
            Assert::IsTrue(
                Transform::CreateShear(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f) * point == Tuple::CreatePoint(2.0f, 3.0f, 7.0f)
            );
        }

        TEST_METHOD(ChainingTransformations) {
            Tuple point = Tuple::CreatePoint(1.0f, 0.0f, 1.0f);
            
            Transform rotation = Transform::CreateRotationX(Constants::PI / 2.0f);
            Transform scaling = Transform::CreateScale(5.0f, 5.0f, 5.0f);
            Transform translation = Transform::CreateTranslation(10.0f, 5.0f, 7.0f);

            Tuple point2 = rotation * point;
            Assert::IsTrue(point2 == Tuple::CreatePoint(1.0f, -1.0f, 0.0f));

            Tuple point3 = scaling * point2;
            Assert::IsTrue(point3 == Tuple::CreatePoint(5.0f, -5.0f, 0.0f));

            Tuple point4 = translation * point3;
            Assert::IsTrue(point4 == Tuple::CreatePoint(15.0f, 0.0f, 7.0f));

            Transform transform;
            transform.RotateX(Constants::PI / 2.0f).Scale(5.0f, 5.0f, 5.0f).Translate(10.0f, 5.0f, 7.0f);

            Assert::IsTrue(transform * point == point4);
        }
    };
}
