#include "OBJParser.h"

RayTracer::OBJParser::OBJParser()
{
	ignoredLines = 0;
	currentGroup = "default";
	groups.emplace(currentGroup, std::vector<std::shared_ptr<Triangle>>());

	commands.emplace(std::string("v"), std::make_shared<ParseVertexCommand>());
	commands.emplace(std::string("vn"), std::make_shared<ParseVertexNormalCommand>());
	commands.emplace(std::string("g"), std::make_shared<ParseGroupCommand>());
	commands.emplace(std::string("f"), std::make_shared<ParseFaceCommand>());
}


RayTracer::OBJParser::~OBJParser()
{
}

void RayTracer::OBJParser::ParseLine(std::string& line)
{
	if (line.length() <= 3) {
		ignoredLines++;
		return;
	}

	
	size_t endOfCommand = line.find(' ');
	size_t beginningOfData = line.find_first_not_of(' ', endOfCommand);

	std::string command = line.substr(0, endOfCommand);
	

	if (commands.find(command) != commands.end()) {
		//Find end of parameter (remove whitespace at the end)
		size_t endOfData = line.find_last_not_of(' ');
		//Extract the command's parameters
		std::string data = line.substr(beginningOfData, endOfData - beginningOfData + 1);


		if (commands[command]->operator()(data, *this)) {
			//Command successful
		}
		else {
			//Command failed
			ignoredLines++;
		}
	}
	else {
		//No command found
		ignoredLines++;
	}
}


bool RayTracer::OBJParser::ParseFile(std::string fileName)
{
	//Open the file
	std::ifstream file(fileName);
	if (!file.is_open()) return false;

	std::cout << "Parsing File: " << fileName << " ... ";

	//Parse each line
	std::string line;
	while (std::getline(file, line)) {
		ParseLine(line);
	}

	std::cout << "Done\n";
	std::cout << ignoredLines << " Line(s) ignored\n";
	
	size_t triangleCount = 0;
	for (auto& group : groups) {
		triangleCount += group.second.size();
	}

	std::cout << triangleCount << " Triangles\n";

	return true;
}

std::shared_ptr<RayTracer::ShapeGroup> RayTracer::OBJParser::MakeGroup()
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

void RayTracer::OBJParser::SetActiveGroup(std::string groupName)
{
	if (!HasGroup(groupName)) {
		groups.emplace(groupName, std::vector<std::shared_ptr<Triangle>>());
	}
	currentGroup = groupName;
}




bool RayTracer::OBJParser::ParseVertexCommand::operator()(std::string& str, OBJParser& parser)
{
	try {
		auto numbers = Helpers::ParseNumbers<double>(str, 3);

		//Store the vertex for later
		parser.AddVertex(Point::CreatePoint(
			numbers[0],
			numbers[1],
			numbers[2]
		));
		return true;
	}
	catch (std::exception & e) {
		return false;
	}
}

bool RayTracer::OBJParser::ParseFaceCommand::operator()(std::string& str, OBJParser& parser)
{
	//Each vertex nr, except the first one, is prefixed with ' '
	size_t vertexCount = std::count(str.begin(), str.end(), ' ') + 1;
	

	//????????
	if (vertexCount < 3) {
		return false;
	}


	try {
		std::vector<size_t> vertexIndices;
		std::vector<size_t> normalIndices;

		//Face with texture coordinates and/or normal vectors
		if (str.find('/') != std::string::npos) {
			std::string numberString = "";
			size_t numberStart = 0;
			size_t numberEnd = str.find('/');

			for (size_t i = 0; i < vertexCount; i++) {
				//Vertex Index
				numberString = str.substr(numberStart, numberEnd - numberStart + 1);
				vertexIndices.push_back(Helpers::StrToNr<size_t>(numberString));

				//Texture Coordinate
				numberStart = numberEnd + 1;
				numberEnd = str.find('/', numberStart);
				if (str.size() != 1) {
					numberString = str.substr(numberStart, numberEnd - numberStart + 1);
					//.push_back(StrToNr<size_t>(numberString));
				}
					
				
				//Normal Index
				numberStart = numberEnd + 1;
				numberEnd = str.find(' ', numberStart);

				//End of string was reached (Last number)
				if (numberEnd == std::string::npos) {
					numberEnd = str.size() - 1;
				}

				numberString = str.substr(numberStart, numberEnd - numberStart + 1);
				normalIndices.push_back(Helpers::StrToNr<size_t>(numberString));
				

				//Prepare next vertex index
				numberStart = numberEnd + 1;
				numberEnd = str.find('/', numberStart);
			}
		}
		//Face command only contains vertex indices
		else {
			vertexIndices = Helpers::ParseNumbers<size_t>(str, vertexCount);
		}

		//Only use normal vectors if they were parsed correctly
		bool useNormalVectors = !normalIndices.empty() && normalIndices.size() == vertexIndices.size();

		//Build the polygon from triangles
		for (size_t index = 2; index < vertexIndices.size(); index++) {
			if (useNormalVectors) {
				parser.AddTriangle(
					Shape::MakeShared<Triangle>(
						parser.GetVertex(vertexIndices[0]),
						parser.GetVertex(vertexIndices[index - 1]),
						parser.GetVertex(vertexIndices[index]),
						parser.GetNormal(normalIndices[0]),
						parser.GetNormal(normalIndices[index - 1]),
						parser.GetNormal(normalIndices[index])
					)
				);
			}
			else {
				parser.AddTriangle(
					Shape::MakeShared<Triangle>(
						parser.GetVertex(vertexIndices[0]),
						parser.GetVertex(vertexIndices[index - 1]),
						parser.GetVertex(vertexIndices[index])
					)
				);
			}
		}

		return true;
	}

	catch (std::exception & e) {
		return false;
	}

}

bool RayTracer::OBJParser::ParseGroupCommand::operator()(std::string & str, OBJParser & parser)
{
	parser.SetActiveGroup(str);

	return true;
}

bool RayTracer::OBJParser::ParseVertexNormalCommand::operator()(std::string & str, OBJParser & parser)
{
	try {
		auto numbers = Helpers::ParseNumbers<double>(str, 3);

		parser.AddNormal(Vector::CreateVector(
			numbers[0],
			numbers[1],
			numbers[2]
		));

		return true;
	}
	catch (std::exception & e) {
		return false;
	}
}
