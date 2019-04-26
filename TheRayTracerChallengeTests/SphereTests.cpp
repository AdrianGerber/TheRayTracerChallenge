#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <IntersectionBuffer.h>
#include <Transform.h>
#include <memory>
#include <Shape.h>
#include <Sphere.h>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(SphereTests)
    {
    public:

		TEST_METHOD(IsShape) {
			Assert::IsTrue(std::is_base_of<Shape, Sphere>());
		}
        TEST_METHOD(SurfaceNormals) {
            Sphere s;

            //Normal vectors at different points
            Assert::IsTrue(s.SurfaceNormal(Point::CreatePoint(1.0f, 0.0f, 0.0f)) == Vector::CreateVector(1.0f, 0.0f, 0.0f));
            Assert::IsTrue(s.SurfaceNormal(Point::CreatePoint(0.0f, 1.0f, 0.0f)) == Vector::CreateVector(0.0f, 1.0f, 0.0f));
            Assert::IsTrue(s.SurfaceNormal(Point::CreatePoint(0.0f, 0.0f, 1.0f)) == Vector::CreateVector(0.0f, 0.0f, 1.0f));
            float result = sqrtf(3.0f) / 3.0f;
            Assert::IsTrue(
                s.SurfaceNormal(Point::CreatePoint(result, result, result))
                == Vector::CreateVector(result, result, result)
            );

            //Normal vectors should be normalized
            Vector v = s.SurfaceNormal(Point::CreatePoint(result, result, result));
            Assert::IsTrue(v == v.Normalize());
        }

        TEST_METHOD(TransformedNormals) {
            Sphere s;
            //Translated Sphere
            s.SetTransform(Transform::CreateTranslation(0.0f, 1.0f, 0.0f));

            Assert::IsTrue(
                s.SurfaceNormal(Point::CreatePoint(0.0f, 1.70711f, -0.70711f))
                == Vector::CreateVector(0.0f, 0.70711f, -0.70711f)
            );

            //Transformed Sphere
            s.SetTransform(
                Transform::CreateScale(1.0f, 0.5f, 1.0f) * Transform::CreateRotationZ(Constants::PI / 5.0f)
            );

            Assert::IsTrue(
                s.SurfaceNormal(Point::CreatePoint(0.0f, sqrtf(2.0f) / 2.0f, -sqrtf(2.0f) / 2.0f))
                == Vector::CreateVector(0.0f, 0.97014f, -0.24254f)
            );
        }

		TEST_METHOD(SphereIntersection1) {
			Ray ray(
				Point::CreatePoint(0.0f, 0.0f, -5.0f),
				Vector::CreateVector(0.0f, 0.0f, 1.0f)
			);

			auto s = Shape::MakeShared<Sphere>();
			
			IntersectionBuffer xs = s->FindIntersections(ray);

			//Ray hits 2 points
			Assert::IsTrue(xs.GetCount() == 2);
			Assert::IsTrue(Constants::DoubleEqual(xs[0].t, 4.0f));
			Assert::IsTrue(Constants::DoubleEqual(xs[1].t, 6.0f));
		}

		TEST_METHOD(SphereIntersection2) {
			Ray ray(
				Point::CreatePoint(0.0f, 1.0f, -5.0f),
				Vector::CreateVector(0.0f, 0.0f, 1.0f)
			);

			auto s = Shape::MakeShared<Sphere>();

			IntersectionBuffer xs = s->FindIntersections(ray);


			//Ray is tangent to the sphere
			Assert::IsTrue(xs.GetCount() == 2);
			Assert::IsTrue(Constants::DoubleEqual(xs[0].t, 5.0f));
			Assert::IsTrue(Constants::DoubleEqual(xs[1].t, 5.0f));
		}

		TEST_METHOD(SphereIntersection3) {
			Ray ray(
				Point::CreatePoint(0.0f, 2.0f, -5.0f),
				Vector::CreateVector(0.0f, 0.0f, 1.0f)
			);

			auto s = Shape::MakeShared<Sphere>();

			IntersectionBuffer xs = s->FindIntersections(ray);


			//Ray doesn't intersect sphere
			Assert::IsTrue(xs.GetCount() == 0);
		}

		TEST_METHOD(SphereIntersection4) {
			Ray ray(
				Point::CreatePoint(0.0f, 0.0f, 0.0f),
				Vector::CreateVector(0.0f, 0.0f, 1.0f)
			);

			auto s = Shape::MakeShared<Sphere>();

			IntersectionBuffer xs = s->FindIntersections(ray);

			//One intersection 'behind' the origin (negative t)
			Assert::IsTrue(xs.GetCount() == 2);
			Assert::IsTrue(Constants::DoubleEqual(xs[0].t, -1.0f));
			Assert::IsTrue(Constants::DoubleEqual(xs[1].t, 1.0f));
		}

		TEST_METHOD(SphereIntersection5) {
			Ray ray(
				Point::CreatePoint(0.0f, 0.0f, 5.0f),
				Vector::CreateVector(0.0f, 0.0f, 1.0f)
			);

			auto s = Shape::MakeShared<Sphere>();

			IntersectionBuffer xs = s->FindIntersections(ray);

			//Both intersections 'behind' the origin (negative t)
			Assert::IsTrue(xs.GetCount() == 2);
			Assert::IsTrue(Constants::DoubleEqual(xs[0].t, -6.0f));
			Assert::IsTrue(Constants::DoubleEqual(xs[1].t, -4.0f));
		}
    };
}
