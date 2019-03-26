#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TheRayTracerChallenge/Tuple.h"
#include "../TheRayTracerChallenge/Constants.h"
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(TupleTests)
    {
    public:

        TEST_METHOD(Point)
        {
            Tuple t(4.3f, -4.2f, 3.1f, 1.0f);

            Assert::IsTrue(Constants::FloatEqual(t.x, 4.3f));
            Assert::IsTrue(Constants::FloatEqual(t.y, -4.2f));
            Assert::IsTrue(Constants::FloatEqual(t.z, 3.1f));
            Assert::IsTrue(Constants::FloatEqual(t.w, 1.0f));
            Assert::IsTrue(t.IsPoint());
            Assert::IsFalse(t.IsVector());

        }
        TEST_METHOD(Vector)
        {
            Tuple t(4.3f, -4.2f, 3.1f, 0.0f);

            Assert::IsTrue(Constants::FloatEqual(t.x, 4.3f));
            Assert::IsTrue(Constants::FloatEqual(t.y, -4.2f));
            Assert::IsTrue(Constants::FloatEqual(t.z, 3.1f));
            Assert::IsTrue(Constants::FloatEqual(t.w, 0.0f));
            Assert::IsFalse(t.IsPoint());
            Assert::IsTrue(t.IsVector());
        }

        TEST_METHOD(Equal) {
            Tuple t1(4.3f, -4.2f, 3.1f, 0.0f);
            Tuple t2(4.3f, -4.2f, 3.1f, 1.0f);

            Assert::IsFalse(t1 == t2);
            Assert::IsTrue(t1 == t1);
            Assert::IsFalse(t1 != t1);
            Assert::IsTrue(t1 != t2);

        }

        TEST_METHOD(PointFactory)
        {
            Tuple t1 = Tuple::CreatePoint(4.0f, -4.0f, 3.0f);
            Tuple t2(4.0f, -4.0f, 3.0f, 1.0f);

            Assert::IsTrue(t1 == t2);
        }

        TEST_METHOD(VectorFactory)
        {
            Tuple t1 = Tuple::CreateVector(4.0f, -4.0f, 3.0f);
            Tuple t2(4.0f, -4.0f, 3.0f, 0.0f);

            Assert::IsTrue(t1 == t2);
        }

        TEST_METHOD(Addition) {
            Tuple t1 = Tuple::CreatePoint(3.0f, -2.0f, 5.0f);
            Tuple t2 = Tuple::CreateVector(-2.0f, 3.0f, 1.0f);

            Assert::IsTrue(t1 + t2 == Tuple(1.0f, 1.0f, 6.0f, 1.0f));
        }

        TEST_METHOD(Subtraction) {
            Tuple p1 = Tuple::CreatePoint(3.0f, 2.0f, 1.0f);
            Tuple p2 = Tuple::CreatePoint(5.0f, 6.0f, 7.0f);
            Tuple v1 = Tuple::CreateVector(3.0f, 2.0f, 1.0f);
            Tuple v2 = Tuple::CreateVector(5.0f, 6.0f, 7.0f);

            Assert::IsTrue(p1 - p2 == Tuple::CreateVector(-2.0f, -4.0f, -6.0));
            Assert::IsTrue(p1 - v2 == Tuple::CreatePoint(-2.0f, -4.0f, -6.0f));
            Assert::IsTrue(v1 - v2 == Tuple::CreateVector(-2.0f, -4.0f, -6.0f));
        }
        TEST_METHOD(Negation) {
            Tuple zero = Tuple::CreateVector(0.0f, 0.0f, 0.0f);
            Tuple v = Tuple::CreateVector(1.0f, -2.0f, 3.0f);

            Assert::IsTrue(zero - v == Tuple::CreateVector(-1.0f, 2.0f, -3.0f));
            Assert::IsTrue(Tuple(1.0f, -2.0f, 3.0f, -4.0f) == -Tuple(-1.0f, 2.0f, -3.0f, 4.0f));
        }

        TEST_METHOD(ScalarMultiplication) {
            Assert::IsTrue(Tuple(1.0f, -2.0f, 3.0f, -4.0f) * 3.5f == Tuple(3.5f, -7.0f, 10.5f, -14.0f));
            Assert::IsTrue(Tuple(1.0f, -2.0f, 3.0f, -4.0f) * 0.5f== Tuple(0.5f, -1.0f, 1.5f, -2.0f));
        }

        TEST_METHOD(ScalarDivision) {
            Assert::IsTrue(Tuple(1.0f, -2.0f, 3.0f, -4.0f) / 2.0f == Tuple(0.5f, -1.0f, 1.5f, -2.0f));
        }

        TEST_METHOD(Magnitude) {
            Assert::IsTrue(Constants::FloatEqual(Tuple::CreateVector(1.0f, 0.0f, 0.0f).Magnitude(), 1.0f));
            Assert::IsTrue(Constants::FloatEqual(Tuple::CreateVector(0.0f, 1.0f, 0.0f).Magnitude(), 1.0f));
            Assert::IsTrue(Constants::FloatEqual(Tuple::CreateVector(0.0f, 0.0f, 1.0f).Magnitude(), 1.0f));
            Assert::IsTrue(Constants::FloatEqual(Tuple::CreateVector(1.0f, 2.0f, 3.0f).Magnitude(), sqrtf(14.0f)));
            Assert::IsTrue(Constants::FloatEqual(Tuple::CreateVector(-1.0f, -2.0f, -3.0f).Magnitude(), sqrtf(14.0f)));
        }

        TEST_METHOD(Normalisation) {
            Assert::IsTrue(Tuple::CreateVector(4.0f, 0.0f, 0.0f).Normalize() == Tuple::CreateVector(1.0f, 0.0f, 0.0f));
            Assert::IsTrue(Tuple::CreateVector(1.0f, 2.0f, 3.0f).Normalize() == Tuple::CreateVector(0.26726f, 0.53452f, 0.80178f));
        }

        TEST_METHOD(DotProduct) {
            Assert::IsTrue(Constants::FloatEqual(Tuple::DotProduct(Tuple::CreateVector(1.0f, 2.0f, 3.0f), Tuple::CreateVector(2.0f, 3.0f, 4.0f)), 20.0f));
        }

        TEST_METHOD(CrossProduct) {
            Tuple v1 = Tuple::CreateVector(1.0f, 2.0f, 3.0f);
            Tuple v2 = Tuple::CreateVector(2.0f, 3.0f, 4.0f);
            Assert::IsTrue(Tuple::CrossProduct(v1, v2) == Tuple::CreateVector(-1.0f, 2.0f, -1.0f));
            Assert::IsTrue(Tuple::CrossProduct(v2, v1) == Tuple::CreateVector(1.0f, -2.0f, 1.0f));
        }
    };
}