#include "OBJParser.h"



OBJParser::OBJParser()
{
	ignoredLines = 0;
	currentGroup = "default";
	groups.emplace(currentGroup, std::vector<std::shared_ptr<Triangle>>());
}


OBJParser::~OBJParser()
{
}

void OBJParser::ParseLine(std::string& line)
{
	if (line.length() == 0) {
		ignoredLines++;
		return;
	}

	switch (line[0]) {
		//Vertex command
	case 'v': {
		try {
			auto numbers = Parse3Numbers<double>(line);

			//Store the vertex for later
			vertices.push_back(Point::CreatePoint(
				std::get<0>(numbers),
				std::get<1>(numbers),
				std::get<2>(numbers)
			));

		}
		catch (std::exception & e) {
			ignoredLines++;
			return;
		}
		break;
	}

	//Face command (create a triangle or polygon)
	case 'f': {
		try {
			//Each vertex nr is prefixed with ' '
			size_t vertexCount = std::count(line.begin(), line.end(), ' ');

			//????????
			if (vertexCount < 3) {
				ignoredLines++;
				return;
			}

			//Triangle
			if (vertexCount == 3) {
				auto vertexNumbers = Parse3Numbers<size_t>(line);

				groups[currentGroup].push_back(
					Shape::MakeShared<Triangle>(
						GetVertex(std::get<0>(vertexNumbers)),
						GetVertex(std::get<1>(vertexNumbers)),
						GetVertex(std::get<2>(vertexNumbers))
						)
				);
			}
			//Convex Polygon
			else {
				try {
					std::vector<size_t> vertexIndices;

					//Positions of the spaces before and after each number
					size_t numberStart = 0;
					size_t numberEnd = 1;

					//Read all numbers on the line
					bool endReached = false;
					while (!endReached) {

						numberStart = numberEnd;

						//Numbers are delimited by spaces (or \n for the last number)
						numberEnd = line.find_first_of(" \n", numberStart + 1);

						//Last number when line is not terminated with \n
						if (numberEnd == std::string::npos) {
							numberEnd = line.size() - 1;
							endReached = true;
						}

						if (line[numberEnd] == '\n') {
							endReached = true;
						}

						//String that contains the number
						std::string nr = line.substr(numberStart + 1, numberEnd - numberStart - 1);

						//Attempt to convert the number
						vertexIndices.push_back(StrToNr<size_t>(nr));
					}

					//Build the polygon from triangles
					for (size_t index = 2; index < vertexIndices.size(); index++) {
						groups[currentGroup].push_back(
							Shape::MakeShared<Triangle>(
								GetVertex(vertexIndices[0]),
								GetVertex(vertexIndices[index - 1]),
								GetVertex(vertexIndices[index])
								)
						);
					}
				}

				catch (std::exception & e) {
					ignoredLines++;
					return;
				}
			}
		}
		catch (std::exception & e) {
			ignoredLines++;
			return;
		}
		break;
	}

	//Group command
	case 'g': {
		//g + ' ' + characters + '\n'
		if (line.size() < 4) {
			ignoredLines++;
			return;
		}

		//Create the new group
		currentGroup = line.substr(2, line.size() - 3);
		if (!HasGroup(currentGroup)) {
			groups.emplace(currentGroup, std::vector<std::shared_ptr<Triangle>>());
		}
		break;
	}

	//Unidentified command
	default: ignoredLines++; break;
	}
}


bool OBJParser::ParseFile(std::string fileName)
{
	//Open the file
	std::ifstream file(fileName);
	if (!file.is_open()) return false;

	std::cout << "Parsing File: " << fileName << " ... ";

	//Parse each line
	std::string line;
	while (std::getline(file, line)) {
		line += "\n";
		ParseLine(line);
	}

	std::cout << "Done\n";
	std::cout << ignoredLines << " Line(s) ignored\n";

	return true;
}

std::shared_ptr<ShapeGroup> OBJParser::MakeGroup()
{
	//Create a new group of shapes
	auto shapeGroup = Shape::MakeShared<ShapeGroup>();

	//Go through every group of shapes from within the .obj file
	for (auto& group : groups) {
		//Combine every shape of every group
		for (auto& triangle : group.second) {
			//Create copies, so that adding the same group
			//multiple times won't cause problems
			shapeGroup->AddShape(triangle->Copy());
		}
	}

	return shapeGroup;
}
