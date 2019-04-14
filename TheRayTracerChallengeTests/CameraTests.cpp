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
#include <HitCalculations.h>
#include <Camera.h>
#include <World.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(CameraTests)
	{
	public:
		TEST_METHOD(ViewTransform) {
			//Default transform
			Assert::IsTrue(
				Camera::CreateViewTransform(
					Point::CreatePoint(0.0f, 0.0f, 0.0f),
					Point::CreatePoint(0.0f, 0.0f, -1.0f),
					Vector::CreateVector(0.0f, 1.0f, 0.0f)
				).matrix
				==
				Matrix::IndentityMatrix4x4()
			);
			//Looking in the positive z direction
			Assert::IsTrue(
				Camera::CreateViewTransform(
					Point::CreatePoint(0.0f, 0.0f, 0.0f),
					Point::CreatePoint(0.0f, 0.0f, 1.0f),
					Vector::CreateVector(0.0f, 1.0f, 0.0f)
				)
				==
				Transform::CreateScale(-1.0f, 1.0f, -1.0f)
			);

			//Moving the camera (translation)
			Assert::IsTrue(
				Camera::CreateViewTransform(
					Point::CreatePoint(0.0f, 0.0f, 8.0f),
					Point::CreatePoint(0.0f, 0.0f, 0.0f),
					Vector::CreateVector(0.0f, 1.0f, 0.0f)
				)
				==
				Transform::CreateTranslation(0.0f, 0.0f, -8.0f)
			);

			//Arbitrary view transformation
			Assert::IsTrue(
				Camera::CreateViewTransform(
					Point::CreatePoint(1.0f, 3.0f, 2.0f),
					Point::CreatePoint(4.0f, -2.0f, 8.0f),
					Vector::CreateVector(1.0f, 1.0f, 0.0f)
				).matrix
				==
				Matrix::Create(
					-0.50709f, 0.50709f, 0.67612f, -2.36643f,
					0.76772f, 0.60609f, 0.12122f, -2.82843f,
					-0.35857f, 0.59761f, -0.71714f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				)
			);
		}

		TEST_METHOD(CameraCreation) {
			Camera c(200, 120, Constants::PI / 2.0f);

			Assert::IsTrue(c.GetXSize() == 200);
			Assert::IsTrue(c.GetYSize() == 120);
			Assert::IsTrue(Constants::FloatEqual(c.GetFieldOfView(), Constants::PI / 2.0f));
			Assert::IsTrue(c.GetTransform().matrix == Matrix::IndentityMatrix4x4());
		}

		TEST_METHOD(PixelSize) {
			Camera c(200, 125, Constants::PI / 2.0f);
			Assert::IsTrue(Constants::FloatEqual(c.GetPixelSize(), 0.01f));
			c = Camera(125, 200, Constants::PI / 2.0f);
			Assert::IsTrue(Constants::FloatEqual(c.GetPixelSize(), 0.01f));
		}

		TEST_METHOD(RayCreation) {
			Camera c(201, 101, Constants::PI / 2.0f);
			Ray r = c.CreateRayForPixel(100, 50);
			//Ray at the canvas's center
			Assert::IsTrue(r.origin == Point::CreatePoint(0.0f, 0.0f, 0.0f));
			Assert::IsTrue(r.direction == Vector::CreateVector(0.0f, 0.0f, -1.0f));

			//Ray through corner of canvas
			r = c.CreateRayForPixel(0, 0);

			Assert::IsTrue(r.origin == Point::CreatePoint(0.0f, 0.0f, 0.0f));
			Assert::IsTrue(r.direction == Vector::CreateVector(0.66519f, 0.33259f, -0.66851f));
		
			//Translated camera
			c.SetTransform(
				Transform::CreateRotationY(Constants::PI / 4.0f) * Transform::CreateTranslation(0.0f, -2.0f, 5.0f)
			);

			r = c.CreateRayForPixel(100, 50);

			Assert::IsTrue(r.origin == Point::CreatePoint(0.0f, 2.0f, -5.0f));
			Assert::IsTrue(r.direction == Vector::CreateVector(sqrtf(2.0f) / 2.0f, 0.0f, -sqrtf(2.0f) / 2.0f));
		}

		TEST_METHOD(RenderFrame) {
			World w;
			w.LoadDefaultWorld();

			Camera c(11, 11, Constants::PI / 2.0f, 
					Camera::CreateViewTransform(
						Point::CreatePoint(0.0f, 0.0f, -5.0f),
						Point::CreatePoint(0.0f, 0.0f, 0.0f),
						Vector::CreateVector(0.0f, 1.0f, 0.0f)
					)
				);

			Canvas image = c.RenderFrame(w);

			Assert::IsTrue(
				image.ReadPixel(5, 5) == Color(0.38066f, 0.47583f, 0.2855f)
			);

		}
	};
}
