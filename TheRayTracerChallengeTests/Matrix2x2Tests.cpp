#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(Matrix2x2Tests)
    {
    public:
        
        TEST_METHOD(ConstructionAndComparison)
        {
            Matrix2x2 m1 = Matrix::Create(-3.0f, 5.0f, 1.0f, -2.0f);
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[0][0], -3.0f));
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[0][1], 5.0f));
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[1][0], 1.0f));
            Assert::IsTrue(Constants::DoubleEqual(m1.elements[1][1], -2.0f));
            Matrix2x2 m2 = Matrix::Create(-2.0f, 7.0f, 19.0f, -5.0f);
            Matrix2x2 m3(m1);

            Assert::IsTrue(m1 == m3);
            Assert::IsTrue(m1 != m2);
            Assert::IsFalse(m1 != m3);
            Assert::IsFalse(m1 == m2);
        }
        
        TEST_METHOD(Determinant2x2) {
            Matrix2x2 m = Matrix::Create(1.0f, 5.0f, -3.0f, 2.0f);

            Assert::IsTrue(Constants::DoubleEqual(m.Determinant(), 17.0f));
        }
        
    };
}
