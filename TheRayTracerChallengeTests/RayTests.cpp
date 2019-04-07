#include "stdafx.h"
#include "CppUnitTest.h"
#include <Matrix.h>
#include <Constants.h>
#include <Tuple.h>
#include <Ray.h>
#include <Intersection.h>
#include <Transform.h>
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(RayTests)
    {
    public:
        TEST_METHOD(RayCreation) {
            Point origin = Tuple::CreatePoint(1.0f, 2.0f, 3.0f);
            Vector direction = Tuple::CreateVector(4.0f, 5.0f, 6.0f);

            Ray ray(origin, direction);

            Assert::IsTrue(ray.direction == direction);
            Assert::IsTrue(ray.origin == origin);
        }

        TEST_METHOD(PositionAtDistance) {
            Ray ray(
                Point::CreatePoint(2.0f, 3.0f, 4.0f),
                Vector::CreateVector(1.0f, 0.0f, 0.0f)
            );

            Assert::IsTrue(
                ray.PositionAt(0.0f) == Point::CreatePoint(2.0f, 3.0f, 4.0f)
            );

            Assert::IsTrue(
                ray.PositionAt(1.0f) == Point::CreatePoint(3.0f, 3.0f, 4.0f)
            );

            Assert::IsTrue(
                ray.PositionAt(-1.0f) == Point::CreatePoint(1.0f, 3.0f, 4.0f)
            );

            Assert::IsTrue(
                ray.PositionAt(2.5f) == Point::CreatePoint(4.5f, 3.0f, 4.0f)
            );
        }

        TEST_METHOD(SphereIntersection1) {
            Ray ray(
                Point::CreatePoint(0.0f, 0.0f, -5.0f),
                Vector::CreateVector(0.0f, 0.0f, 1.0f)
            );

            auto s = std::make_shared<Sphere>();

            IntersectionBuffer xs = ray.FindIntersections(s);

            //Ray hits 2 points
            Assert::IsTrue(xs.GetCount() == 2);
            Assert::IsTrue(Constants::FloatEqual(xs[0].t, 4.0f));
            Assert::IsTrue(Constants::FloatEqual(xs[1].t, 6.0f));
        }

        TEST_METHOD(SphereIntersection2) {
            Ray ray(
                Point::CreatePoint(0.0f, 1.0f, -5.0f),
                Vector::CreateVector(0.0f, 0.0f, 1.0f)
            );

            auto s = std::make_shared<Sphere>();

            IntersectionBuffer xs = ray.FindIntersections(s);

            //Ray is tangent to the sphere
            Assert::IsTrue(xs.GetCount() == 2);
            Assert::IsTrue(Constants::FloatEqual(xs[0].t, 5.0f));
            Assert::IsTrue(Constants::FloatEqual(xs[1].t, 5.0f));
        }

        TEST_METHOD(SphereIntersection3) {
            Ray ray(
                Point::CreatePoint(0.0f, 2.0f, -5.0f),
                Vector::CreateVector(0.0f, 0.0f, 1.0f)
            );

            auto s = std::make_shared<Sphere>();

            IntersectionBuffer xs = ray.FindIntersections(s);

            //Ray doesn't intersect sphere
            Assert::IsTrue(xs.GetCount() == 0);
        }

        TEST_METHOD(SphereIntersection4) {
            Ray ray(
                Point::CreatePoint(0.0f, 0.0f, 0.0f),
                Vector::CreateVector(0.0f, 0.0f, 1.0f)
            );

            auto s = std::make_shared<Sphere>();

            IntersectionBuffer xs = ray.FindIntersections(s);

            //One intersection 'behind' the origin (negative t)
            Assert::IsTrue(xs.GetCount() == 2);
            Assert::IsTrue(Constants::FloatEqual(xs[0].t, -1.0f));
            Assert::IsTrue(Constants::FloatEqual(xs[1].t, 1.0f));
        }

        TEST_METHOD(SphereIntersection5) {
            Ray ray(
                Point::CreatePoint(0.0f, 0.0f, 5.0f),
                Vector::CreateVector(0.0f, 0.0f, 1.0f)
            );

            auto s = std::make_shared<Sphere>();

            IntersectionBuffer xs = ray.FindIntersections(s);

            //Both intersections 'behind' the origin (negative t)
            Assert::IsTrue(xs.GetCount() == 2);
            Assert::IsTrue(Constants::FloatEqual(xs[0].t, -6.0f));
            Assert::IsTrue(Constants::FloatEqual(xs[1].t, -4.0f));
        }

        TEST_METHOD(IntersectionDataStructure) {
            Intersection i;
            auto s = std::make_shared<Sphere>();
            i.object = s;
            i.t = 3.5f;

            Assert::IsTrue(Constants::FloatEqual(i.t, 3.5f));
            Assert::IsTrue(i.object == s);
        }

        TEST_METHOD(IntersectionAggregation) {
            auto s = std::make_shared<Sphere>();
            IntersectionBuffer i;

            i.Add(Intersection(1.0f, s));
            i.Add(Intersection(2.0f, s));

            Assert::IsTrue(i.GetCount() == 2);

            Assert::IsTrue(Constants::FloatEqual(i[0].t, 1.0f));
            Assert::IsTrue(Constants::FloatEqual(i[1].t, 2.0f));
        }

        TEST_METHOD(CorrectObject) {
            Ray ray(
                Point::CreatePoint(0.0f, 0.0f, -5.0f),
                Vector::CreateVector(0.0f, 0.0f, 1.0f)
            );

            auto s = std::make_shared<Sphere>();

            IntersectionBuffer xs = ray.FindIntersections(s);

            Assert::IsTrue(xs.GetCount() == 2);
            Assert::IsTrue(xs[0].object == s);
            Assert::IsTrue(xs[1].object == s);
        }

        TEST_METHOD(Hit1) {
            auto s = std::make_shared<Sphere>();
            //Positive t
            Intersection i1 = Intersection(1.0f, s);
            Intersection i2 = Intersection(2.0f, s);
            IntersectionBuffer i(i2, i1);

            Assert::IsTrue(i.GetFirstHit() == i1);
        }

        TEST_METHOD(Hit2) {
            auto s = std::make_shared<Sphere>();
            //Positive and negative t
            Intersection i1 = Intersection(-1.0f, s);
            Intersection i2 = Intersection(1.0f, s);
            IntersectionBuffer i(i2, i1);

            Assert::IsTrue(i.GetFirstHit() == i2);
        }

        TEST_METHOD(Hit3) {
            auto s = std::make_shared<Sphere>();
            //Negative t
            Intersection i1 = Intersection(-2.0f, s);
            Intersection i2 = Intersection(-1.0f, s);
            IntersectionBuffer i(i2, i1);

            Assert::IsTrue(i.GetFirstHit().object == nullptr);
        }

        TEST_METHOD(Hit4) {
            auto s = std::make_shared<Sphere>();
            //Hit must always be the lowest, non negative intersection
            Intersection i1 = Intersection(5.0f, s);
            Intersection i2 = Intersection(7.0f, s);
            Intersection i3 = Intersection(-3.0f, s);
            Intersection i4 = Intersection(2.0f, s);
            IntersectionBuffer i;

            i.Add(i1);
            i.Add(i2);
            i.Add(i3);
            i.Add(i4);

            Assert::IsTrue(i.GetFirstHit() == i4);
        }

        TEST_METHOD(RayTranslation) {
            Ray r(
                Point::CreatePoint(1.0f, 2.0f, 3.0f),
                Vector::CreateVector(0.0f, 1.0f, 0.0f)
            );

            Transform m = Transform::CreateTranslation(3.0f, 4.0f, 5.0f);

            Ray r2 = r.Transform(m);

            Assert::IsTrue(r2.origin == Point::CreatePoint(4.0f, 6.0f, 8.0f));
            Assert::IsTrue(r2.direction == Vector::CreateVector(0.0f, 1.0f, 0.0f));
        }
        
        TEST_METHOD(RayScaling) {
            Ray r(
                Point::CreatePoint(1.0f, 2.0f, 3.0f),
                Vector::CreateVector(0.0f, 1.0f, 0.0f)
            );

            Transform m = Transform::CreateScale(2.0f, 3.0f, 4.0f);

            Ray r2 = r.Transform(m);

            Assert::IsTrue(r2.origin == Point::CreatePoint(2.0f, 6.0f, 12.0f));
            Assert::IsTrue(r2.direction == Vector::CreateVector(0.0f, 3.0f, 0.0f));
        }

        TEST_METHOD(DefaultTransform) {
            Sphere s;

            Assert::IsTrue(s.GetTransform().matrix == Matrix::IndentityMatrix4x4());
        }

        TEST_METHOD(TransformSphere) {
            Sphere s;
            Transform t = Transform::CreateTranslation(2.0f, 3.0f, 4.0f);
            s.SetTransform(t);

            Assert::IsTrue(s.GetTransform().matrix == t.matrix);
        }

        TEST_METHOD(ScaledIntersection) {
            Ray r(
                Point::CreatePoint(0.0f, 0.0f, -5.0f),
                Vector::CreateVector(0.0f, 0.0f, 1.0f)
            );

            auto s = std::make_shared<Sphere>();
            
            s->SetTransform(Transform::CreateScale(2.0f, 2.0f, 2.0f));


            auto xs = r.FindIntersections(s);
            Assert::IsTrue(xs.GetCount() == 2);
            Assert::IsTrue(Constants::FloatEqual(xs[0].t, 3.0f));
            Assert::IsTrue(Constants::FloatEqual(xs[1].t, 7.0f));
        }

        TEST_METHOD(TranslatedIntersection) {
            Ray r(
                Point::CreatePoint(0.0f, 0.0f, -5.0f),
                Vector::CreateVector(0.0f, 0.0f, 1.0f)
            );

            auto s = std::make_shared<Sphere>();

            s->SetTransform(Transform::CreateTranslation(5.0f, 0.0f, 0.0f));


            auto xs = r.FindIntersections(s);
            Assert::IsTrue(xs.GetCount() == 0);
        }
    };
}