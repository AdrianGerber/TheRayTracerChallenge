#include <iostream>
#include <fstream>
#include "Canvas.h"
#include "MatrixTemplate.h"
#include <array>


int main(void) {

    /*
    Matrix<4, 4> test({ {
        1, 2, 3, 4,
        5, 6, 7, 8,
        7, 6, 5, 4,
        3, 2, 1, 19
        } });
        */
    /*
    Matrix3x3 t2 = test.Submatrix(2, 3);
    
    std::cout << test.elements[0][0] << "\n";
    std::cout << test.elements[3][3] << "\n";
    std::cout << test.elements[0][3] << "\n";
    */
    /*
    Canvas canvas(5, 3);
    canvas.WritePixel(Color(1.5f, 0.0f, 0.0f), 0, 0);
    canvas.WritePixel(Color(0.0f, 0.5f, 0.0f), 2, 1);
    canvas.WritePixel(Color(-0.5f, 0.0f, 1.0f), 4, 2);

    std::ofstream outputImage;
    outputImage.open("output.ppm", std::ios::out);

    if (outputImage.is_open()) {
        std::string ppmString = canvas.ConvertToPPM();
        outputImage.write(ppmString.c_str(), ppmString.length());
        outputImage.close();
    }
    else {
        std::cout << "Could not open file\n";
    }
    */

    std::cout << std::endl;
    char c;
    std::cin >> c;
    
    return 0;
}

