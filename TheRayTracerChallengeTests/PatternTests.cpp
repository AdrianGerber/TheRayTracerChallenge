#include "stdafx.h"
#include "CppUnitTest.h"
#include <Color.h>
#include <tuple>
#include <Matrix.h>
#include <Pattern.h>
#include <StripePattern.h>
#include <GradientPattern.h>
#include <CheckerPattern.h>
#include <RingPattern.h>
#include <Constants.h>
#include <cmath>
#include <Material.h>
#include <LightSource.h>
#include <Shape.h>
#include <Sphere.h>
#include <memory>
#include <type_traits>
#include <BlendedPattern.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(PatternTests)
	{
		class TestPattern : public Pattern {
		public:
			Point lastPoint;
			TestPattern() {
				lastPoint = Point::CreatePoint(0.0, 0.0, 0.0);
			}

			Color ReadPattern(Point point) {
				lastPoint = point;
				//Pass through the point coordinates
				return Color(point.x, point.y, point.z);
			}

			std::shared_ptr<Pattern> PatternSpecificCopy() override {
				return std::make_shared<TestPattern>();
			}
		};

	public:
		
		TEST_METHOD(BaseClass) {
			Assert::IsTrue(std::is_base_of<Pattern, GradientPattern>());
			Assert::IsTrue(std::is_base_of<Pattern, StripePattern>());
			Assert::IsTrue(std::is_base_of<Pattern, RingPattern>());
			Assert::IsTrue(std::is_base_of<Pattern, CheckerPattern>());
		}
		TEST_METHOD(Transformation) {
			TestPattern testPattern;

			//Default transform is the identity matrix
			Assert::IsTrue(testPattern.GetTransform().GetMatrix()
				== Matrix::IndentityMatrix4x4());

			Transform t;
			t.RotateX(Constants::PI);

			testPattern.SetTransform(t);
			Assert::IsTrue(testPattern.GetTransform() == t);
		}

		TEST_METHOD(CorrectPoint) {
			TestPattern testPattern;

			Point in = Point::CreatePoint(1.0, 2.0, 3.0);
			Color out(1.0, 2.0, 3.0);

			Assert::IsTrue(testPattern.ColorAtShapePoint(in) == out);
		}

		TEST_METHOD(IsAppliedToLighting) {
			//Pattern should be applied inside the lighting() function
			Material m;
			m.pattern = std::make_shared<StripePattern>(Color(1.0, 1.0, 1.0), Color(0.0, 0.0, 0.0));
			m.ambient = 1.0;
			m.diffuse = 0.0;
			m.specular = 0.0;
			std::shared_ptr<Shape> shape = std::make_shared<Sphere>();

			Vector eye = Vector::CreateVector(0.0, 0.0, -1.0);
			Vector normal = Vector::CreateVector(0.0, 0.0, -1.0);
			LightSource l(Point::CreatePoint(0.0, 0.0, -10.0), Color(1.0, 1.0, 1.0));

			shape->SetMaterial(m);

			Assert::IsTrue(
				l.Lighting(shape, Point::CreatePoint(0.9, 0.0, 0.0), eye, normal, false)
				==
				Color(1.0, 1.0, 1.0)
			);

			Assert::IsTrue(
				l.Lighting(shape, Point::CreatePoint(1.1, 0.0, 0.0), eye, normal, false)
				==
				Color(0.0, 0.0, 0.0)
			);
		}

		TEST_METHOD(GradientPatternTest) {
			Color black(0.0, 0.0, 0.0);
			Color white(1.0, 1.0, 1.0);
			GradientPattern pattern(
				white,
				black
			);

			//Color is interpolated linearly between white and black
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.25, 0.0, 0.0)) == Color(0.75, 0.75, 0.75));
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.5, 0.0, 0.0)) == Color(0.5, 0.5, 0.5));
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.75, 0.0, 0.0)) == Color(0.25, 0.25, 0.25));
		}

		TEST_METHOD(StripePatternTest) {
			Color black(0.0, 0.0, 0.0);
			Color white(1.0, 1.0, 1.0);
			StripePattern pattern(
				white,
				black
			);

			//Stripe pattern is constant in y direction
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 1.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 2.0, 0.0)) == white);
			//Also constant in z direction
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 1.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 2.0)) == white);
			//Color alternates in x direction
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.9, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(1.0, 0.0, 0.0)) == black);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(-0.1, 0.0, 0.0)) == black);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(-1.0, 0.0, 0.0)) == black);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(-1.1, 0.0, 0.0)) == white);
		}

		TEST_METHOD(RingPatternTest) {
			Color black(0.0, 0.0, 0.0);
			Color white(1.0, 1.0, 1.0);
			RingPattern pattern(white, black);

			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(1.0, 0.0, 0.0)) == black);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 1.0)) == black);
			//Slightly more that sqrt(2.0)
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.708, 0.0, 0.708)) == black);
		}

		TEST_METHOD(CheckerPatterTest) {
			Color black(0.0, 0.0, 0.0);
			Color white(1.0, 1.0, 1.0);
			CheckerPattern pattern(white, black);

			
			//Checkers repeat in x direction
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.99, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(1.01, 0.0, 0.0)) == black);

			//Checkers repeat in y direction
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.99, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 1.01, 0.0)) == black);
			
			//Checkers repeat in z direction
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.99)) == white);
			Assert::IsTrue(pattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 1.01)) == black);

		}
		
		TEST_METHOD(BlendedPatternTest) {
			//Colors should be blended together
			BlendedPattern blendedPattern(
				std::make_shared<ColorPattern>(Color(0.2, 0.3, 0.4)),
				std::make_shared<ColorPattern>(Color(0.4, 0.3, 0.2))
			);
			Assert::IsTrue(blendedPattern.ColorAtShapePoint(Point::CreatePoint(0.0, 0.0, 0.0))
				== Color(0.6, 0.6, 0.6)
			);


			//Make sure that the right point is passed through to the subpatterns
			auto testPattern1 = std::make_shared<TestPattern>();
			auto testPattern2 = std::make_shared<TestPattern>();

			blendedPattern = BlendedPattern(testPattern1, testPattern2);

			blendedPattern.ColorAtShapePoint(Point::CreatePoint(1.0, 2.0, 3.0));

			Assert::IsTrue(testPattern1->lastPoint == Point::CreatePoint(1.0, 2.0, 3.0));
			Assert::IsTrue(testPattern2->lastPoint == Point::CreatePoint(1.0, 2.0, 3.0));
		}

	};
}