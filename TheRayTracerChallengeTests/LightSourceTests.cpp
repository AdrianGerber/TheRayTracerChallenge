#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <IntersectionBuffer.h>
#include <Transform.h>
#include <memory>
#include <cmath>
#include <Shape.h>
#include <Sphere.h>
#include <LightSource.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(LightSourceTests)
    {
    public:
        TEST_METHOD(Initialisation) {
            LightSource l(Point::CreatePoint(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f));

            Assert::IsTrue(l.GetPosition() == Point::CreatePoint(0.0f, 0.0f, 0.0f));
            Assert::IsTrue(l.GetIntensity() == Color(1.0f, 1.0f, 1.0f));
        }
        TEST_METHOD(Lighting) {
            LightSource l;
            Vector eye;
            Vector normal;
            Material m;
            Point position = Point::CreatePoint(0.0f, 0.0f, 0.0f);

            //Eye between wall light source and point
            l = LightSource(Point::CreatePoint(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
            eye = Vector::CreateVector(0.0f, 0.0f, -1.0f);
            normal = Vector::CreateVector(0.0f, 0.0f, -1.0f);

            Assert::IsTrue(
                l.Lighting(m, position, eye, normal, false)
                == Color(1.9f, 1.9f, 1.9f)
            );

            //45 degree view to normal vector and light source
            l = LightSource(Point::CreatePoint(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
            eye = Vector::CreateVector(0.0f, sqrtf(2.0f) / 2.0f, -sqrtf(2.0f)/2.0f);
            normal = Vector::CreateVector(0.0f, 0.0f, -1.0f);
            Assert::IsTrue(
                l.Lighting(m, position, eye, normal, false)
                == Color(1.0f, 1.0f, 1.0f)
            );

            //eye and normal aligned, light source at 45 degree
            l = LightSource(Point::CreatePoint(0.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
            eye = Vector::CreateVector(0.0f, 0.0f, -1.0f);
            normal = Vector::CreateVector(0.0f, 0.0f, -1.0f);
            Assert::IsTrue(
                l.Lighting(m, position, eye, normal, false)
                == Color(0.7364f, 0.7364f, 0.7364f)
            );

            //Reflection of light source aligned with eye
            l = LightSource(Point::CreatePoint(0.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
            eye = Vector::CreateVector(0.0f, -sqrtf(2.0f)/2.0f, -sqrtf(2.0f)/2.0f);
            normal = Vector::CreateVector(0.0f, 0.0f, -1.0f);

            Assert::IsTrue(
                l.Lighting(m, position, eye, normal, false)
                == Color(1.6364f, 1.6364f, 1.6364f)
            );


            //Light behind surface, only ambient light visible
            l = LightSource(Point::CreatePoint(0.0f, 0.0f, 10.0f), Color(1.0f, 1.0f, 1.0f));
            eye = Vector::CreateVector(0.0f, 0.0f, -1.0f);
            normal = Vector::CreateVector(0.0f, 0.0f, -1.0f);
            Assert::IsTrue(
                l.Lighting(m, position, eye, normal, false)
                == Color(0.1f, 0.1f, 0.1f)
            );
        }

		TEST_METHOD(InShadow) {
			LightSource l(
				Point::CreatePoint(0.0f, 0.0f, -10.0f),
				Color(1.0f, 1.0f, 1.0f)
			);


			Assert::IsTrue(
				l.Lighting(
					Material(),
					Point::CreatePoint(0.0f, 0.0f, 0.0f),
					Vector::CreateVector(0.0f, 0.0f, -1.0f),
					Vector::CreateVector(0.0f, 0.0f, -1.0f),
					true
				)
				==
				Color(0.1f, 0.1f, 0.1f)
			);
		}
    };
}
