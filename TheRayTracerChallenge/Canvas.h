#pragma once
#include <iostream>
#include <fstream>
#include "Color.h"
#include <vector>
#include <string>

namespace RayTracer {
	//Canvas used to store an image
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

		std::string ConvertToPPM(unsigned int maxValue = 255, size_t maxLineLength = 70);

		//Save the image as a .ppm file
		bool SaveToFile(std::string fileName, size_t maxValue = 255, size_t maxLineLength = 70);

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
}