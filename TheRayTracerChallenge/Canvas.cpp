#include "Canvas.h"



Canvas::Canvas(size_t xSize, size_t ySize)
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

size_t Canvas::GetHeight()
{
    return canvasSizeY;
}

size_t Canvas::GetWidth()
{
    return canvasSizeX;
}

std::string Canvas::ConvertToPPM(unsigned int maxValue, size_t maxLineLength)
{
    
    if (maxValue == 0) return std::string("Invalid maxValue");
    
    //Creating the PPM header
    std::string ppmString{ "P3\n" };
    ppmString += std::to_string(canvasSizeX) + " " + std::to_string(canvasSizeY) + "\n";
    ppmString += std::to_string(maxValue) + "\n";

    //Go through each line of pixels (each line of pixels usually corresponds to one line inside the file)
    for (size_t line = 0; line < canvasSizeY; line++) {
        std::string currentFileLine = "";
        bool isFirstValueOnLine = true;

        //Go through each pixel that should be put on one line
        for (size_t row = 0; row < canvasSizeX; row++) {
            Color currentPixel = ReadPixel(row, line);

            //Format the color intensities as required
            for (float colorValue : {currentPixel.r, currentPixel.g, currentPixel.b}) {
                //Limit the color's intensity
                if (colorValue > 1.0f) colorValue = 1.0f;
                if (colorValue < 0.0f) colorValue = 0.0f;

                //The color's intensity as a string
                std::string intensityString = std::to_string(static_cast<unsigned int>(std::roundf(colorValue * maxValue)));

                //Limit the maximum size of a line (Long lines will cause problems with some image viewers...)
                if (currentFileLine.length() + intensityString.length() + std::string(" ").length() > maxLineLength) {
                    //Save the full line and begin a new one
                    ppmString += currentFileLine + "\n";
                    currentFileLine = "";
                }
                else {
                    if (isFirstValueOnLine) {
                        //Don't add a space for the first value on this line
                        isFirstValueOnLine = false;
                    }
                    else {
                        //Add spaces between adjacent values
                        currentFileLine += " ";
                    }
                }
                
                //Append the number
                currentFileLine += intensityString;
            }
        }
        
        //Terminate each line of the file with '\n'
        ppmString += currentFileLine + "\n";
    }

    return ppmString;
}

bool Canvas::SaveToFile(std::string fileName, size_t maxValue, size_t maxLineLength) {
    if (maxValue == 0) return false;
    
    std::string header = "P3\n" + std::to_string(canvasSizeX) + " " + std::to_string(canvasSizeY) + "\n" + std::to_string(maxValue) + "\n";

    //Try to write the image to a file
    std::ofstream outputFile;

    outputFile.open(fileName + ".ppm", std::ios::out);

    if (outputFile.is_open()) {
       
        outputFile.write(header.c_str(), header.length());

        //Go through each line of pixels (each line of pixels usually corresponds to one line inside the file)
        for (size_t line = 0; line < canvasSizeY; line++) {
            std::string currentFileLine = "";
            bool isFirstValueOnLine = true;

            //Go through each pixel that should be put on one line
            for (size_t row = 0; row < canvasSizeX; row++) {
                Color currentPixel = ReadPixel(row, line);

                //Format the color intensities as required
                for (float colorValue : {currentPixel.r, currentPixel.g, currentPixel.b}) {
                    //Limit the color's intensity
                    if (colorValue > 1.0f) colorValue = 1.0f;
                    if (colorValue < 0.0f) colorValue = 0.0f;

                    //The color's intensity as a string
                    std::string intensityString = std::to_string(static_cast<unsigned int>(std::roundf(colorValue * maxValue)));

                    //Limit the maximum size of a line (Long lines will cause problems with some image viewers...)
                    if (currentFileLine.length() + intensityString.length() + std::string(" ").length() > maxLineLength) {
                        //Save the full line and begin a new one
                        currentFileLine += "\n";
                        outputFile.write(currentFileLine.c_str(), currentFileLine.length());
                        currentFileLine = "";
                    }
                    else {
                        if (isFirstValueOnLine) {
                            //Don't add a space for the first value on this line
                            isFirstValueOnLine = false;
                        }
                        else {
                            //Add spaces between adjacent values
                            currentFileLine += " ";
                        }
                    }

                    //Append the number
                    currentFileLine += intensityString;
                }
            }

            //Terminate each line of the file with '\n'
            currentFileLine += "\n";
            outputFile.write(currentFileLine.c_str(), currentFileLine.length());
        }

        outputFile.close();
        return true;
    }
    else {
        return false;
    }
}
