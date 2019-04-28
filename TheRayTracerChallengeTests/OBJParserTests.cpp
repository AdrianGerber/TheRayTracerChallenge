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
#include <BoundingBox.h>
#include <OBJParser.h>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
	TEST_CLASS(OBJPareserTests)
	{
	public:
		TEST_METHOD(InvalidInput) {
			OBJParser p;
			p.ParseLine(std::string("szudtihgouhuhip"));
			p.ParseLine(std::string("vhdkgliipuphhoiüühio"));
			p.ParseLine(std::string("qwertzuiopll"));
			p.ParseLine(std::string("vgogouzgouzgozg"));
			p.ParseLine(std::string("zeszesurdifl"));

			Assert::IsTrue(p.GetIgnoredLineCount() == 5);
		}
		TEST_METHOD(Vertices) {
			OBJParser p;
			p.ParseLine(std::string("v -1 1 0\n"));
			p.ParseLine(std::string("v -1.0000 0.5000 0.0000\n"));
			p.ParseLine(std::string("v 1 0 0\n"));
			p.ParseLine(std::string("v 1 1 0\n"));

			Assert::IsTrue(p.GetIgnoredLineCount() == 0);
			Assert::IsTrue(p.GetVertex(1) == Point::CreatePoint(-1.0, 1.0, 0.0));
			Assert::IsTrue(p.GetVertex(2) == Point::CreatePoint(-1.0, 0.5, 0.0));
			Assert::IsTrue(p.GetVertex(3) == Point::CreatePoint(1.0, 0.0, 0.0));
			Assert::IsTrue(p.GetVertex(4) == Point::CreatePoint(1.0, 1.0, 0.0));
		}

		TEST_METHOD(Triangles) {
			OBJParser p;
			p.ParseLine(std::string("v -1 1 0\n"));
			p.ParseLine(std::string("v -1 0 0\n"));
			p.ParseLine(std::string("v 1 0 0\n"));
			p.ParseLine(std::string("v 1 1 0\n"));

			p.ParseLine(std::string("f 1 2 3\n"));
			p.ParseLine(std::string("f 1 3 4\n"));


			
			auto t1 = p.GetTriangle(1, "default");
			auto t2 = p.GetTriangle(2, "default");

			Assert::IsTrue(t1->GetPoint(0) == p.GetVertex(1));
			Assert::IsTrue(t1->GetPoint(1) == p.GetVertex(2));
			Assert::IsTrue(t1->GetPoint(2) == p.GetVertex(3));
			Assert::IsTrue(t2->GetPoint(0) == p.GetVertex(1));
			Assert::IsTrue(t2->GetPoint(1) == p.GetVertex(3));
			Assert::IsTrue(t2->GetPoint(2) == p.GetVertex(4));
		}

		TEST_METHOD(Polygon) {
			OBJParser p;
			p.ParseLine(std::string("v -1 1 0\n"));
			p.ParseLine(std::string("v -1 0 0\n"));
			p.ParseLine(std::string("v 1 0 0\n"));
			p.ParseLine(std::string("v 1 1 0\n"));
			p.ParseLine(std::string("v 0 2 0\n"));

			p.ParseLine(std::string("f 1 2 3 4 5\n"));

			Assert::IsTrue(p.GetTriangle(1, "default")->GetPoint(0) == p.GetVertex(1));
			Assert::IsTrue(p.GetTriangle(1, "default")->GetPoint(1) == p.GetVertex(2));
			Assert::IsTrue(p.GetTriangle(1, "default")->GetPoint(2) == p.GetVertex(3));
			Assert::IsTrue(p.GetTriangle(2, "default")->GetPoint(0) == p.GetVertex(1));
			Assert::IsTrue(p.GetTriangle(2, "default")->GetPoint(1) == p.GetVertex(3));
			Assert::IsTrue(p.GetTriangle(2, "default")->GetPoint(2) == p.GetVertex(4));
			Assert::IsTrue(p.GetTriangle(3, "default")->GetPoint(0) == p.GetVertex(1));
			Assert::IsTrue(p.GetTriangle(3, "default")->GetPoint(1) == p.GetVertex(4));
			Assert::IsTrue(p.GetTriangle(3, "default")->GetPoint(2) == p.GetVertex(5));
		}

		TEST_METHOD(Groups) {
			OBJParser p;
			p.ParseLine(std::string("v -1 1 0\n"));
			p.ParseLine(std::string("v -1 0 0\n"));
			p.ParseLine(std::string("v 1 0 0\n"));
			p.ParseLine(std::string("v 1 1 0\n"));

			p.ParseLine(std::string("g FirstGroup\n"));
			p.ParseLine(std::string("f 1 2 3\n"));

			p.ParseLine(std::string("g SecondGroup\n"));
			p.ParseLine(std::string("f 1 3 4\n"));

			Assert::IsTrue(p.HasGroup("FirstGroup"));
			auto firstGroupTriangle = p.GetTriangle(1, "FirstGroup");
			Assert::IsTrue(p.HasGroup("SecondGroup"));
			auto secondGroupTriangle = p.GetTriangle(1, "SecondGroup");

			Assert::IsTrue(firstGroupTriangle->GetPoint(0) == p.GetVertex(1));
			Assert::IsTrue(firstGroupTriangle->GetPoint(1) == p.GetVertex(2));
			Assert::IsTrue(firstGroupTriangle->GetPoint(2) == p.GetVertex(3));

			Assert::IsTrue(secondGroupTriangle->GetPoint(0) == p.GetVertex(1));
			Assert::IsTrue(secondGroupTriangle->GetPoint(1) == p.GetVertex(3));
			Assert::IsTrue(secondGroupTriangle->GetPoint(2) == p.GetVertex(4));
		
			Assert::IsTrue(p.MakeGroup()->GetShapeCount() == 2);
		}
	};
}
