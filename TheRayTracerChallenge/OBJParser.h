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
#include <functional>
#include <algorithm>
#include "Helpers.h"

namespace RayTracer {
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
		void AddVertex(Point p) { vertices.push_back(p); }
		size_t GetVertexCount() { return vertices.size(); }

		//Read a triangle (index starts at 1!)
		std::shared_ptr<Triangle> GetTriangle(size_t id, std::string groupName) { return groups[groupName][id - 1]; }
		void AddTriangle(std::shared_ptr<Triangle> triangle) { groups[currentGroup].push_back(triangle); }

		//Read a normal vector (Starts at 1!)
		Vector GetNormal(size_t id) { return normalVectors[id - 1]; }
		void AddNormal(Vector normal) { normalVectors.push_back(normal); }

		//Create a group of triangles from the parsed file
		std::shared_ptr<ShapeGroup> MakeGroup();

		//Get a named group from within a .obj file
		const std::vector<std::shared_ptr<Triangle>>& GetGroup(std::string name) { return groups[name]; }
		void SetActiveGroup(std::string groupName);

		//Find out if the file contained a group with the specified name
		bool HasGroup(std::string name) { return groups.find(name) != groups.end(); }

	private:
		//How many lines were ignored while parsing the file
		size_t ignoredLines;

		//Group that the next triangles will be written to
		std::string currentGroup;

		//Defined points
		std::vector<Point> vertices;

		std::vector<Vector> normalVectors;

		//Named groups of triangles
		std::map<std::string, std::vector<std::shared_ptr<Triangle>>> groups;



		struct ParserFunction {
			ParserFunction() = default;
			virtual bool operator()(std::string& str, OBJParser& parser) = 0;
		};

		//Supported .obj commands
		std::map<std::string, std::shared_ptr<ParserFunction>> commands;

		struct ParseVertexCommand : ParserFunction {
			ParseVertexCommand() = default;
			bool operator()(std::string& str, OBJParser& parser) override;
		};

		struct ParseFaceCommand : ParserFunction {
			ParseFaceCommand() = default;
			bool operator()(std::string& str, OBJParser& parser) override;
		};

		struct ParseGroupCommand : ParserFunction {
			ParseGroupCommand() = default;
			bool operator()(std::string& str, OBJParser& parser) override;
		};

		struct ParseVertexNormalCommand : ParserFunction {
			ParseVertexNormalCommand() = default;
			bool operator()(std::string& str, OBJParser& parser) override;
		};
	};
}
