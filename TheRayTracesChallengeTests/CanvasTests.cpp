#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <Canvas.h>
#include <Color.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheRayTracesChallengeTests
{
    TEST_CLASS(CanvasTests)
    {
    public:

        TEST_METHOD(CanvasCreation)
        {
            Canvas c(10, 20);
        
            size_t width = c.GetWidth();
            size_t height = c.GetHeight();

            Assert::AreEqual(width, static_cast<size_t>(10));
            Assert::AreEqual(height, static_cast<size_t>(20));

            for (size_t x{ 0 }; x < width; x++) {
                for (size_t y{ 0 }; y < height; y++) {
                    Assert::IsTrue(c.ReadPixel(x, y) == Color(0.0f, 0.0f, 0.0f));
                }
            }
        }
        TEST_METHOD(HeaderCreation) {
            /*
            Canvas c(5, 3);

            //PPM File must start with the correct header
            std::string ppmString = c.ConvertToPPM();
            Assert::IsTrue(ppmString.find("P3\n5 3\n255\n") == 0);
            */
        }

        TEST_METHOD(PixelDataCreation) {
            /*
            Canvas c(5, 3);
            c.WritePixel(Color(1.5f, 0.0f, 0.0f), 0, 0);
            c.WritePixel(Color(0.0f, 0.5f, 0.0f), 2, 1);
            c.WritePixel(Color(-0.5f, 0.0f, 1.0f), 4, 2);

            std::string ppmString = c.ConvertToPPM();

            //Find the beginning of the pixel data
            size_t pixelDataStart = 0;
            for (size_t newlineCount = 0; newlineCount < 3; newlineCount++) {
                pixelDataStart = ppmString.find("\n", 0, 3) + 1;
            }
                
            Assert::IsTrue(
                ppmString.find(
                    "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                    "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255"
                    , pixelDataStart
            ) == pixelDataStart);
            */
        }
    };
}