#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <IntersectionBuffer.h>
#include <Transform.h>
#include <memory>
#include <string>
#include <Shape.h>
#include <Triangle.h>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(TriangleTests)
	{


	public:

		TEST_METHOD(IsShape) {
			Assert::IsTrue(std::is_base_of<Shape, Triangle>());
		}

		/*Bounding Box
		
		Scenario: A triangle has a bounding box
		Given p1 ← point(-3, 7, 2)
		And p2 ← point(6, 2, -4)
		And p3 ← point(2, -1, -1)
		And shape ← triangle(p1, p2, p3)
		When box ← bounds_of(shape)
		Then box.min = point(-3, -1, -4)
		And box.max = point(6, 7, 2)
		
		
		
		*/

	};
}
