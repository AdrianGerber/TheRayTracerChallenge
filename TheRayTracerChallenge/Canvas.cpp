#include "Canvas.h"



RayTracer::Canvas::Canvas(size_t xSize, size_t ySize)
{
    canvasSizeX = xSize;
    canvasSizeY = ySize;

    pixelCount = xSize * ySize;

    frameBuffer.reserve(pixelCount);

    //Initialize with black
    for (size_t i{ 0 }; i < pixelCount; i++) {
        frameBuffer.push_back(Color(0.0f, 0.0f, 0.0f));
    }
}

size_t RayTracer::Canvas::GetHeight()
{
    return canvasSizeY;
}

size_t RayTracer::Canvas::GetWidth()
{
    return canvasSizeX;
}

std::string RayTracer::Canvas::ConvertToPPM(unsigned int maxValue, size_t maxLineLength)
{
    std::cout << "Converting to PPM...\n";
    if (maxValue == 0) return std::string("Invalid maxValue");
    
    //Creating the PPM header
    std::string ppmString{ "P3\n" };
    ppmString += std::to_string(canvasSizeX) + " " + std::to_string(canvasSizeY) + "\n";
    ppmString += std::to_string(maxValue) + "\n";

    //Estimate the pixel-data's length (works best for 8 bit color depth)
    size_t estimatedDataSize = (canvasSizeX * canvasSizeY * 4) + 100;

    ppmString.reserve(ppmString.length() + estimatedDataSize);

    std::string intensityString;
    intensityString.reserve(4);

    //Go through each line of pixels (each line of pixels usually corresponds to one line inside the file)
    for (size_t line = 0; line < canvasSizeY; line++) {
        bool isFirstValueOnLine = true;
        size_t currentLineLength = 0;

        //Go through each pixel that should be put on one line
        for (size_t row = 0; row < canvasSizeX; row++) {
            Color currentPixel = ReadPixel(row, line);

            //Format the color intensities as required
            for (double colorValue : {currentPixel.r, currentPixel.g, currentPixel.b}) {
                //Limit the color's intensity
                if (colorValue > 1.0) colorValue = 1.0;
                if (colorValue < 0.0) colorValue = 0.0;

                //The color's intensity as a string
                intensityString = std::to_string(static_cast<unsigned int>(std::round(colorValue * maxValue)));

                //Limit the maximum size of a line (Long lines will cause problems with some image viewers...)
                if (currentLineLength + intensityString.length() + 1 > maxLineLength) {
                    //Begin a new line
                    ppmString.append("\n");
                    currentLineLength = 0;
                }
                else {
                    if (isFirstValueOnLine) {
                        //Don't add a space for the first value on this line
                        isFirstValueOnLine = false;
                        currentLineLength += 3;
                    }
                    else {
                        //Add spaces between adjacent values
                        ppmString.append(" ");
                        currentLineLength += 1;
                    }
                }
                
                //Append the number
                ppmString.append(intensityString);
                currentLineLength += intensityString.length();
            }
        }
        
        //Terminate each line of the file with '\n'
        ppmString.append("\n");
    }

    return ppmString;
}

bool RayTracer::Canvas::SaveToFile(std::string fileName, size_t maxValue, size_t maxLineLength) {
    if (maxValue == 0) return false;
    
    std::string header = "P3\n" + std::to_string(canvasSizeX) + " " + std::to_string(canvasSizeY) + "\n" + std::to_string(maxValue) + "\n";

    //Try to write the image to a file
    std::ofstream outputFile;

    outputFile.open(fileName + ".ppm", std::ios::out);

    if (outputFile.is_open()) {
        
        std::string fileContent = ConvertToPPM();
        std::cout << "Writing image to file...";
        outputFile.write(fileContent.c_str(), fileContent.length());

        outputFile.close();
        std::cout << " Done\n";
        return true;
    }
    else {
        std::cout << "Could not open file " + fileName + ".ppm\n";
        return false;
    }
}
