#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(MatrixTests)
    {
    public:

        TEST_METHOD(ConstructionAndComparison2x2)
        {
            Matrix2x2 m1(-3.0f, 5.0f, 1.0f, -2.0f);
            Assert::IsTrue(Constants::FloatEqual(m1.elements[0][0], -3.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[0][1], 5.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[1][0], 1.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[1][1], -2.0f));

            Matrix2x2 m2(-2.0f, 7.0f, 19.0f, -5.0f);
            Matrix2x2 m3(m1);

            Assert::IsTrue(m1 == m3);
            Assert::IsTrue(m1 != m2);
            Assert::IsFalse(m1 != m3);
            Assert::IsFalse(m1 == m2);
        }

        TEST_METHOD(ConstructionAndComparison3x3)
        {
            Matrix3x3 m1(-3.0f, 5.0f, 0.0f, 1.0f, -2.0f, -7.0f, 0.0f, 1.0f, 1.0f);
            Assert::IsTrue(Constants::FloatEqual(m1.elements[0][0], -3.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[1][1], -2.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[2][2], 1.0f));

            Matrix3x3 m2(-3.0f, 5.0f, 1.0f, 1.0f, -2.0f, -7.0f, 0.0f, 1.0f, 1.0f);
            Matrix3x3 m3(m1);

            Assert::IsTrue(m1 == m3);
            Assert::IsTrue(m1 != m2);
            Assert::IsFalse(m1 != m3);
            Assert::IsFalse(m1 == m2);
        }

        TEST_METHOD(ConstructionAndComparison4x4)
        {
            Matrix4x4 m1(1.0f, 2.0f, 3.0f, 4.0f,
                5.5f, 6.5f, 7.5f, 8.5f,
                9.0f, 10.0f, 11.0f, 12.0f,
                13.5f, 14.5f, 15.5f, 16.5f);

            Assert::IsTrue(Constants::FloatEqual(m1.elements[0][0], 1.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[0][3], 4.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[1][0], 5.5f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[1][2], 7.5f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[2][2], 11.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[3][0], 13.5f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[3][2], 15.5f));


            Matrix4x4 m2(1.0f, 2.0f, 3.0f, 4.0f,
                5.5f, 6.5f, 7.5f, 8.5f,
                9.0f, 10.0f, 13.0f, 12.0f,
                13.5f, 14.5f, 15.5f, 16.5f);
            Matrix4x4 m3(m1);

            Assert::IsTrue(m1 == m3);
            Assert::IsTrue(m1 != m2);
            Assert::IsFalse(m1 != m3);
            Assert::IsFalse(m1 == m2);
        }

        TEST_METHOD(Multiplication4x4) {
            Matrix4x4 m1(1.0f, 2.0f, 3.0f, 4.0f,
                5.0f, 6.0f, 7.0f, 8.0f,
                9.0f, 8.0f, 7.0f, 6.0f,
                5.0f, 4.0f, 3.0f, 2.0f);
            Matrix4x4 m2(-2.0f, 1.0f, 2.0f, 3.0f,
                3.0f, 2.0f, 1.0f, -1.0f,
                4.0f, 3.0f, 6.0f, 5.0f,
                1.0f, 2.0f, 7.0f, 8.0f);
            Matrix4x4 result(20.0f, 22.0f, 50.0f, 48.0f,
                44.0f, 54.0f, 114.0f, 108.0f,
                40.0f, 58.0f, 110.0f, 102.0f,
                16.0f, 26.0f, 46.0f, 42.0f);

            Assert::IsTrue(m1 * m2 == result);
        }

        TEST_METHOD(Multiplication4x4Tuple) {
            Matrix4x4 m(1.0f, 2.0f, 3.0f, 4.0f,
                2.0f, 4.0f, 4.0f, 2.0f,
                8.0f, 6.0f, 4.0f, 1.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            Tuple t(1.0f, 2.0f, 3.0f, 1.0f);
            Tuple resultExpected(18.0f, 24.0f, 33.0f, 1.0f);
            Tuple result = m * t;

            Logger::WriteMessage((std::to_string(result.x) +
                std::string(" ") + std::to_string(result.y) +
                std::string(" ") + std::to_string(result.z) +
                std::string(" ") + std::to_string(result.w)).c_str());

            Assert::IsTrue(result == resultExpected);
        }

        TEST_METHOD(IdentityMatrix) {
            Matrix4x4 m(0.0f, 1.0f, 2.0f, 4.0f,
                1.0f, 2.0f, 4.0f, 8.0f,
                2.0f, 4.0f, 8.0f, 16.0f,
                4.0f, 8.0f, 16.0f, 32.0f);

            Assert::IsTrue(m * Matrix4x4::IdentityMatrix() == m);
        }
    };
}
