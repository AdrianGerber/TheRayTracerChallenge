#include "Canvas.h"



Canvas::Canvas(size_t xSize, size_t ySize)
{
    canvasSizeX = xSize;
    canvasSizeY = ySize;

    pixelCount = xSize * ySize;

    frameBuffer.reserve(pixelCount);

    for (size_t i{ 0 }; i < pixelCount; i++) {
        frameBuffer.push_back(Color(0.0f, 0.0f, 0.0f));
    }
}

size_t Canvas::GetHeight()
{
    return canvasSizeY;
}

size_t Canvas::GetWidth()
{
    return canvasSizeX;
}

std::string Canvas::ConvertToPPM(size_t maxValue)
{
    /*
    if (maxValue == 0) return std::string("Invalid maxValue");
    
    //Creating the PPM header
    std::string ppmString{ "P3\n" };
    ppmString += std::to_string(canvasSizeX) + " " + std::to_string(canvasSizeY) + "\n";
    ppmString += std::to_string(maxValue) + "\n";

    for (size_t row = 0; row < canvasSizeX; row++) {
        for (size_t line = 0; line < canvasSizeY; line++) {
            Color pixeColor = ReadPixel(row, line);
            ppmString += std::to_string(pixeColor.r * maxValue);
            ppmString += " ";
            ppmString += std::to_string(pixeColor.g * maxValue);
            ppmString += " ";
            ppmString += std::to_string(pixeColor.b * maxValue);

            if(line != canvasSizeY - 1) ppmString += " ";
        }
        ppmString += "\n";
    }

    return ppmString;
    */
    return std::string();
}
