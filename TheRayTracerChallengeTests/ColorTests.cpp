#include "stdafx.h"
#include "CppUnitTest.h"
#include <Color.h>
#include <Constants.h>
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(ColorTests)
    {
    public:

        TEST_METHOD(ColorCreation)
        {
            Color c(-0.5f, 0.4f, 1.7f);

            Assert::IsTrue(Constants::DoubleEqual(c.r, -0.5));
            Assert::IsTrue(Constants::DoubleEqual(c.g, 0.4f));
            Assert::IsTrue(Constants::DoubleEqual(c.b, 1.7f));
        }

        TEST_METHOD(ColorComparison) {
            Color c1(0.1f, 0.2f, 0.3f);
            Color c2(0.2f, 0.5f, -12.0f);
            Color c3(0.1f, 0.2f, 0.3f);

            Assert::IsTrue(c1 == c3);
            Assert::IsFalse(c1 != c3);
            Assert::IsFalse(c1 == c2);
            Assert::IsTrue(c1 != c2);


        }

        TEST_METHOD(ColorAddition) {
            Color c1(0.9f, 0.6f, 0.75f);
            Color c2(0.7f, 0.1f, 0.25f);

            Assert::IsTrue(c1 + c2 == Color(1.6f, 0.7f, 1.0f));
        }
        TEST_METHOD(ColorSubtraction) {
            Color c1(0.9f, 0.6f, 0.75f);
            Color c2(0.7f, 0.1f, 0.25f);

            Assert::IsTrue(c1 - c2 == Color(0.2f, 0.5f, 0.5f));
        }

        TEST_METHOD(ColorScalarMultiplication) {
            Color c(0.2f, 0.3f, 0.4f);

            Assert::IsTrue(c * 2.0f == Color(0.4f, 0.6f, 0.8f));
        }

        TEST_METHOD(ColorMultiplication) {
            Color c1(1.0f, 0.2f, 0.4f);
            Color c2(0.9f, 1.0f, 0.1f);

            Assert::IsTrue(c1 * c2 == Color(0.9f, 0.2f, 0.04f));
        }
    };
}