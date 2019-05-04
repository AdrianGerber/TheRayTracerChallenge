#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace RayTracer {
	namespace Helpers {
		//Templates to parse numbers
		template<typename T> static T StrToNr(std::string& str);

		template<> static double StrToNr<double>(std::string& str) {
			return std::stod(str);
		}

		template<> static size_t StrToNr<size_t>(std::string& str) {
			return std::stoul(str);
		}

		template<typename T>
		static std::vector<T> ParseNumbers(const std::string& str, size_t count) {
			std::vector<T> values;
			values.reserve(count);

			size_t startOfNumber = 0;
			size_t endOfNumber = str.find_first_of(" ");
			std::string numberString = "";
			T currentNumber;

			for (size_t numberIndex = 0; numberIndex < count; numberIndex++) {

				//Read the number
				numberString = str.substr(startOfNumber, endOfNumber - startOfNumber + 1);
				currentNumber = StrToNr<T>(numberString);

				values.push_back(currentNumber);

				//Find the position of the next number
				//Was the last digit reached?
				if (endOfNumber == str.length() - 1) {
					if (numberIndex == count - 1) {
						//Success
						break;
					}
					else {
						//End of string reached, but not enough numbers read...
						throw std::invalid_argument("String is not formatted correctly");
					}
				}

				startOfNumber = endOfNumber;
				endOfNumber = str.find_first_of(" ", startOfNumber + 1);

				//Last digit of the string is the end of the next number (no trailing whitespace)
				if (endOfNumber == std::string::npos) {
					endOfNumber = str.length() - 1;
				}
			}

			return values;
		}
	}
}
