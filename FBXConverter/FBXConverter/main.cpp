#define _CRT_SECURE_NO_WARNINGS

//#include <iostream>
#include <string>
//#include <fstream>
#include "Converter.h"
//#include <fbxsdk.h>
using namespace std;

void ConvertFile(string FileName) {
	FBXLoader fbx;
	std::vector<Vertex> outVertices;
	std::vector<std::uint32_t> outIndices;
	std::vector<Material> outMaterial;

	std::string filename = "../models/" + FileName;
	filename = "../models/ice_cube_2";
	std::string filename2 = filename + ".fbx";
	ifstream fin(filename2);

	if (!fin)
	{
		cout << "the path not found." << endl;
		return;
	}

	fbx.LoadFBX(outVertices, outIndices, outMaterial, filename);


}

int main() {

	ConvertFile("ice_cube_2");

}