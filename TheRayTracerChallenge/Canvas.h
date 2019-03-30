#pragma once

#include "Color.h"
#include <vector>
#include <string>

class Canvas
{
public:
    
    Canvas() = delete;
    Canvas(size_t xSize, size_t ySize);
    ~Canvas() = default;
    
    void WritePixel(Color color, size_t xPosition, size_t yPosition);
    Color ReadPixel(size_t xPosition, size_t yPosition);

    size_t GetHeight();
    size_t GetWidth();

    std::string ConvertToPPM(size_t maxValue = 255);

private:
    size_t canvasSizeX, canvasSizeY, pixelCount;
    std::vector<Color> frameBuffer;
};

inline void Canvas::WritePixel(Color color, size_t xPosition, size_t yPosition) {
    if (xPosition >= canvasSizeX) return;
    if (yPosition >= canvasSizeY) return;

    frameBuffer[xPosition + yPosition * canvasSizeX] = color;
}

inline Color Canvas::ReadPixel(size_t xPosition, size_t yPosition) {
    if (xPosition >= canvasSizeX) return Color();
    if (yPosition >= canvasSizeY) return Color();

    return frameBuffer[xPosition + yPosition * canvasSizeX];
}

