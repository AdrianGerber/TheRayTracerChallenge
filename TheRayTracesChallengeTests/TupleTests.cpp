#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TheRayTracerChallenge/Tuple.h"
#include "../TheRayTracerChallenge/Constants.h"

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
	};
}