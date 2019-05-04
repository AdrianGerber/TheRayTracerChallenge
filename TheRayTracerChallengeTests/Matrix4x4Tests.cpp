#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace RayTracer;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(Matrix4x4Tests)
    {
    public:
        
        TEST_METHOD(ConstructionAndComparison)
        {
            Matrix4x4 m1 = Matrix::Create(
                1.0f, 2.0f, 3.0f, 4.0f,
                5.5f, 6.5f, 7.5f, 8.5f,
                9.0f, 10.0f, 11.0f, 12.0f,
                13.5f, 14.5f, 15.5f, 16.5f);

            Assert::IsTrue(Constants::DoubleEqual(m1.elements[0][0], 1.0f));
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[0][3], 4.0f));
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[1][0], 5.5f));
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[1][2], 7.5f));
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[2][2], 11.0f));
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[3][0], 13.5f));
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[3][2], 15.5f));


            Matrix4x4 m2 = Matrix::Create(1.0f, 2.0f, 3.0f, 4.0f,
                5.5f, 6.5f, 7.5f, 8.5f,
                9.0f, 10.0f, 13.0f, 12.0f,
                13.5f, 14.5f, 15.5f, 16.5f);
            Matrix4x4 m3(m1);

            Assert::IsTrue(m1 == m3);
            Assert::IsTrue(m1 != m2);
            Assert::IsFalse(m1 != m3);
            Assert::IsFalse(m1 == m2);
        }
        
        TEST_METHOD(Multiplication) {
            Matrix4x4 m1 = Matrix::Create(1.0f, 2.0f, 3.0f, 4.0f,
                5.0f, 6.0f, 7.0f, 8.0f,
                9.0f, 8.0f, 7.0f, 6.0f,
                5.0f, 4.0f, 3.0f, 2.0f);
            Matrix4x4 m2 = Matrix::Create(-2.0f, 1.0f, 2.0f, 3.0f,
                3.0f, 2.0f, 1.0f, -1.0f,
                4.0f, 3.0f, 6.0f, 5.0f,
                1.0f, 2.0f, 7.0f, 8.0f);
            Matrix4x4 result = Matrix::Create(20.0f, 22.0f, 50.0f, 48.0f,
                44.0f, 54.0f, 114.0f, 108.0f,
                40.0f, 58.0f, 110.0f, 102.0f,
                16.0f, 26.0f, 46.0f, 42.0f);

            Assert::IsTrue(m1 * m2 == result);
        }

        TEST_METHOD(MultiplicationTuple) {
            Matrix4x4 m = Matrix::Create(1.0f, 2.0f, 3.0f, 4.0f,
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
            Matrix4x4 m = Matrix::Create(0.0f, 1.0f, 2.0f, 4.0f,
                1.0f, 2.0f, 4.0f, 8.0f,
                2.0f, 4.0f, 8.0f, 16.0f,
                4.0f, 8.0f, 16.0f, 32.0f);

            Assert::IsTrue(m * Matrix::IndentityMatrix4x4() == m);
        }
        
        TEST_METHOD(Transpose) {
            Matrix4x4 m = Matrix::Create(
                0.0f, 9.0f, 3.0f, 0.0f,
                9.0f, 8.0f, 0.0f, 8.0f,
                1.0f, 8.0f, 5.0f, 3.0f,
                0.0f, 0.0f, 5.0f, 8.0f);
            Matrix4x4 mTransposed = Matrix::Create(
                0.0f, 9.0f, 1.0f, 0.0f,
                9.0f, 8.0f, 8.0f, 0.0f,
                3.0f, 0.0f, 5.0f, 5.0f,
                0.0f, 8.0f, 3.0f, 8.0f
            );

            Assert::IsTrue(m.Transpose() == mTransposed);

            //Indentity matrix shouldn't change when transposed
            Assert::IsTrue(Matrix::IndentityMatrix4x4().Transpose() == Matrix::IndentityMatrix4x4());
        }
        
        TEST_METHOD(Submatrix4x4) {
            Matrix4x4 m = Matrix::Create(
                -6.0f, 1.0f, 1.0f, 6.0f,
                -8.0f, 5.0f, 8.0f, 6.0f,
                -1.0f, 0.0f, 8.0f, 2.0f,
                -7.0f, 1.0f, -1.0f, 1.0f
            );

            Matrix3x3 expected = Matrix::Create(
                -6.0f, 1.0f, 6.0f,
                -8.0f, 8.0f, 6.0f,
                -7.0f, -1.0f, 1.0f
            );

            Assert::IsTrue(m.Submatrix(2, 1) == expected);
        }
        
        TEST_METHOD(Determinant) {
            Matrix4x4 m = Matrix::Create(
                -2.0f, -8.0f, 3.0f, 5.0f,
                -3.0f, 1.0f, 7.0f, 3.0f,
                1.0f, 2.0f, -9.0f, 6.0f,
                -6.0f, 7.0f, 7.0f, -9.0f
            );

            Assert::IsTrue(Constants::DoubleEqual(m.Cofactor(0, 0), 690.0f));
            Assert::IsTrue(Constants::DoubleEqual(m.Cofactor(0, 1), 447.0f));
            Assert::IsTrue(Constants::DoubleEqual(m.Cofactor(0, 2), 210.0f));
            Assert::IsTrue(Constants::DoubleEqual(m.Cofactor(0, 3), 51.0f));
            Assert::IsTrue(Constants::DoubleEqual(m.Determinant(), -4071.0f));
        }

        TEST_METHOD(InvertibilityTest) {
            Matrix4x4 m1 = Matrix::Create(
                6.0f, 4.0f, 4.0f, 4.0f,
                5.0f, 5.0f, 7.0f, 6.0f,
                4.0f, -9.0f, 3.0f, -7.0f,
                9.0f, 1.0f, 7.0f, -6.0f
            );

            Matrix4x4 m2 = Matrix::Create(
                -4.0f, 2.0f, -2.0f, -3.0f,
                9.0f, 6.0f, 2.0f, 6.0f,
                0.0f, -5.0f, 1.0f, -5.0f,
                0.0f, 0.0f, 0.0f, 0.0f
            );

            Assert::IsTrue(m1.IsInvertible());
            Assert::IsFalse(m2.IsInvertible());

            Assert::IsTrue(Constants::DoubleEqual(m1.Determinant(), -2120.0f));
            Assert::IsTrue(Constants::DoubleEqual(m2.Determinant(), 0.0f));

        }

        TEST_METHOD(InversionBasic) {
            Matrix4x4 m1 = Matrix::Create(
                -5.0f, 2.0f, 6.0f, -8.0f,
                1.0f, -5.0f, 1.0f, 8.0f,
                7.0f, 7.0f, -6.0f, -7.0f,
                1.0f, -3.0f, 7.0f, 4.0f
            );

            Matrix4x4 m2 = m1.Inversion();
            Matrix4x4 expected = Matrix::Create(
                0.21805f, 0.45113f, 0.24060f, -0.04511f,
                -0.80827f, -1.45677f, -0.44361f, 0.52068f,
                -0.07895f, -0.22368f, -0.05263f, 0.19737f,
                -0.52256f, -0.81391f, -0.30075f, 0.30639f
            );


            Assert::IsTrue(Constants::DoubleEqual(m1.Determinant(), 532.0f));
            Assert::IsTrue(Constants::DoubleEqual(m1.Cofactor(2, 3), -160.0f));
            Assert::IsTrue(Constants::DoubleEqual(m2.elements[3][2], -160.0f/532.0f));
            Assert::IsTrue(Constants::DoubleEqual(m1.Cofactor(3, 2), 105.0f));
            Assert::IsTrue(Constants::DoubleEqual(m2.elements[2][3], 105.0f/532.0f));

            for (size_t row = 0; row < expected.rows; row++) {
                for (size_t column = 0; column < expected.columns; column++) {
                    if (!Constants::DoubleEqual(m2.elements[row][column], expected.elements[row][column])) {
                        std::string log = std::to_string(m2.elements[row][column]) + " != " + std::to_string(expected.elements[row][column]);
                        Logger::WriteMessage(log.c_str());

                        log = std::to_string(m2.elements[row][column] - expected.elements[row][column]);
                        Logger::WriteMessage(log.c_str());
                    }
                }
            }

            Assert::IsTrue(m2 == expected);
        }

        TEST_METHOD(Inversion) {
            Matrix4x4 m1 = Matrix::Create(
                8.0f, -5.0f, 9.0f, 2.0f,
                7.0f, 5.0f, 6.0f, 1.0f,
                -6.0f, 0.0f, 9.0f, 6.0f,
                -3.0f, 0.0f, -9.0f, -4.0f
            );

            Matrix4x4 expected1 = Matrix::Create(
                -0.15385f, -0.15385f, -0.28205f, -0.53846f,
                -0.07692f, 0.12308f, 0.02564f, 0.03077f,
                0.35897f, 0.35897f, 0.43590f, 0.92308f,
                -0.69231f, -0.69231f, -0.76923f, -1.92308f
            );

            Matrix4x4 m2 = Matrix::Create(
                9.0f, 3.0f, 0.0f, 9.0f,
                -5.0f, -2.0f, -6.0f, -3.0f,
                -4.0f, 9.0f, 6.0f, 4.0f,
                -7.0f, 6.0f, 6.0f, 2.0f
            );

            Matrix4x4 expected2 = Matrix::Create(
                -0.04074f, -0.07778f, 0.14444f, -0.22222f,
                -0.07778f, 0.03333f, 0.36667f, -0.33333f,
                -0.02901f, -0.14630f, -0.10926f, 0.12963f,
                0.17778f, 0.06667f, -0.26667f, 0.33333f
            );

            Assert::IsTrue(m1.Inversion() == expected1);
            Assert::IsTrue(m2.Inversion() == expected2);
        }

        TEST_METHOD(InversionMultiplication) {
            Matrix4x4 m1 = Matrix::Create(
                3.0f, -9.0f, 7.0f, 3.0f,
                3.0f, -8.0f, 2.0f, -9.0f,
                -4.0f, 4.0f, 4.0f, 1.0f,
                -6.0f, 5.0f, -1.0f, 1.0f
            );

            Matrix4x4 m2 = Matrix::Create(
                8.0f, 2.0f, 2.0f, 2.0f,
                3.0f, -1.0f, 7.0f, 0.0f,
                7.0f, 0.0f, 5.0f, 4.0f,
                6.0f, -2.0f, 0.0f, 5.0f
            );

            Matrix4x4 m3 = m1 * m2;

            Assert::IsTrue(m3 * m2.Inversion() == m1);
        }
        
    };
}
