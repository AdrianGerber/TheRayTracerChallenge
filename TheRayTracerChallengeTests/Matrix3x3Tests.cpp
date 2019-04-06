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
        
        TEST_METHOD(Minor) {
            Matrix3x3 m = Matrix::Create(
                3.0f, 5.0f, 0.0f,
                2.0f, -1.0f, -7.0f,
                6.0f, -1.0f, 5.0f
            );

            Assert::IsTrue(Constants::FloatEqual(m.Minor(1, 0), 25.0f));
        }

        TEST_METHOD(Cofactor) {
            Matrix3x3 m = Matrix::Create(
                3.0f, 5.0f, 0.0f,
                2.0f, -1.0f, -7.0f,
                6.0f, -1.0f, 5.0f
            );

            Assert::IsTrue(Constants::FloatEqual(m.Minor(0, 0), -12.0f));
            Assert::IsTrue(Constants::FloatEqual(m.Cofactor(0, 0), -12.0f));

            Assert::IsTrue(Constants::FloatEqual(m.Minor(1, 0), 25.0f));
            Assert::IsTrue(Constants::FloatEqual(m.Cofactor(1, 0), -25.0f));
        }

        TEST_METHOD(Determinant) {
            Matrix3x3 m = Matrix::Create(
                1.0f, 2.0f, 6.0f,
                -5.0f, 8.0f, -4.0f,
                2.0f, 6.0f, 4.0f
            );

            Assert::IsTrue(Constants::FloatEqual(m.Cofactor(0, 0),56.0f));
            Assert::IsTrue(Constants::FloatEqual(m.Cofactor(0, 1),12.0f));
            Assert::IsTrue(Constants::FloatEqual(m.Cofactor(0, 2),-46.0f));
            Assert::IsTrue(Constants::FloatEqual(m.Determinant(),-196.0f));
        }
        
    };
}
