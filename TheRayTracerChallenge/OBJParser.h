#pragma once
#include <string>

#include "Tuple.h"
#include <vector>
#include <tuple>
#include "Shape.h"
#include "Triangle.h"
#include "ShapeGroup.h"
#include <memory>
#include <map>
#include <iostream>
#include <fstream>

class OBJParser
{
public:
	OBJParser();
	~OBJParser();

	size_t GetIgnoredLineCount() { return ignoredLines; }

	//Parsing .obj files / commands
	void ParseLine(std::string& line);
	bool ParseFile(std::string fileName);

	//Read a vertex (indexing starts with 1!)
	Point GetVertex(size_t id) { return vertices[id - 1]; }
	size_t GetVertexCount() { return vertices.size(); }

	//Read a triangle (index starts at 1!)
	std::shared_ptr<Triangle> GetTriangle(size_t id, std::string groupName) { return groups[groupName][id - 1]; }

	//Create a group of triangles from the parsed file
	std::shared_ptr<ShapeGroup> MakeGroup();

	//Get a named group from within a .obj file
	const std::vector<std::shared_ptr<Triangle>>& GetGroup(std::string name) { return groups[name]; }

	//Find out if the file contained a group with the specified name
	bool HasGroup(std::string name) { return groups.find(name) != groups.end(); }

private:
	//How many lines were ignored while parsing the file
	size_t ignoredLines;

	//Group that the next triangles will be written to
	std::string currentGroup;

	//Defined points
	std::vector<Point> vertices;

	//Named groups of triangles
	std::map<std::string, std::vector<std::shared_ptr<Triangle>>> groups;


	//Templates to parse numbers
	template<typename T> T StrToNr(std::string& str);
	
	template<> double StrToNr<double>(std::string& str) {
		return std::stod(str);
	}

	template<> size_t StrToNr<size_t>(std::string& str) {
		return std::stoul(str);
	}

	template<typename T>
	std::tuple<T, T, T> Parse3Numbers(const std::string& str) {
		T numbers[3];

		//Positions of the spaces before and after each number
		size_t numberStart = 0;
		size_t numberEnd = 1;

		//Read all 3 numbers
		for (size_t i = 0; i < 3; i++) {
			
			numberStart = numberEnd;

			//Numbers are delimited by spaces (or \n for the last number)
			numberEnd = str.find_first_of(" \n", numberStart + 1);

			if (numberEnd == std::string::npos) {
				throw(std::exception());
			}

			//String that contains the number
			std::string nr = str.substr(numberStart + 1, numberEnd - numberStart - 1);

			//Attempt to convert the number
			T currentNumber = StrToNr<T>(nr);

			//Store the number
			numbers[i] = currentNumber;
		}

		return std::make_tuple(numbers[0], numbers[1], numbers[2]);
	}
};

