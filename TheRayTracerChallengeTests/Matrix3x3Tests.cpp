#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(Matrix3x3Tests)
    {
    public:
        
        TEST_METHOD(ConstructionAndComparison)
        {
            Matrix3x3 m1 = Matrix::Create(-3.0f, 5.0f, 0.0f, 1.0f, -2.0f, -7.0f, 0.0f, 1.0f, 1.0f);
            Assert::IsTrue(Constants::FloatEqual(m1.elements[0][0], -3.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[1][1], -2.0f));
            Assert::IsTrue(Constants::FloatEqual(m1.elements[2][2], 1.0f));

            Matrix3x3 m2 = Matrix::Create(-3.0f, 5.0f, 1.0f, 1.0f, -2.0f, -7.0f, 0.0f, 1.0f, 1.0f);
            Matrix3x3 m3(m1);

            Assert::IsTrue(m1 == m3);
            Assert::IsTrue(m1 != m2);
            Assert::IsFalse(m1 != m3);
            Assert::IsFalse(m1 == m2);
        }

        TEST_METHOD(Submatrix3x3) {
            Matrix3x3 m = Matrix::Create(
                1.0f, 5.0f, 0.0f,
                -3.0f, 2.0f, 7.0f,
                0.0f, 6.0f, -3.0f
            );

            Matrix2x2 expected = Matrix::Create(
                -3.0f, 2.0f,
                0.0f, 6.0f
            );

            Assert::IsTrue(m.Submatrix(0, 2) == expected);
        }
        
    };
}
