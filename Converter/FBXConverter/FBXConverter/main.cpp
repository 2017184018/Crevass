
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <io.h>
#include "FbxLoader.h"
using namespace std;

void LoadFileFbx(std::string filaName);
void LoadFolderFbx(std::string findPath, std::string dir);


void LoadFileAnimationFbx(std::string clipName, ::string filaName);
void LoadFolderAnimationFbx(std::string findPath, std::string dir);


int main(void)
{


	// �ϳ��� �޽��� �ٲٰ� ���� ��

	//LoadFileFbx(".//Ani//snow_cube");
	 //�����ȿ� ��� �޽� fbx�� �ٲٰ� ���� ��
	 //LoadFolderFbx("..//..//..//Resource//fbx//*.fbx", "..//..//..//Resource//convert");

	// ���Ͼȿ� ��� �ִϸ��̼� FBX ���� ��ȯ�� ���� ��
	LoadFolderAnimationFbx(".//Ani//husky_run.fbx", "./Ani/");
	// Ư�� �ִϸ��̼� ���� �ϳ��� ��ȯ�� ���� ��
	// .FBX�� ������ �̸��� �־��־����
	// LoadFileAnimationFbx("Idle", "./Ani/");
	
}

void LoadFileFbx(std::string filaName)
{
	FbxLoader fbx;
	std::vector<Vertex> outVertexVector;
	std::vector<std::uint32_t> outIndexVector;
	std::vector<Material> outMaterial;

	fbx.LoadFBX(outVertexVector, outIndexVector, outMaterial, filaName);

}

void LoadFolderFbx(std::string findPath, std::string dir)
{
	struct _finddata_t fd;
	intptr_t handle;
	vector<string> paths;

	if ((handle = _findfirst(findPath.c_str(), &fd)) == -1L)
	{
		cout << "No file in directory!" << endl;
		return;
	}
	do
	{
		cout << fd.name << endl;
		paths.push_back(dir + fd.name);
	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);

	cout << "Fbx Count is " << paths.size() << endl;
	for (int i = 0; i < paths.size(); ++i)
	{
		for (int j = 0; j < 4; ++j)
			paths[i].pop_back();
		LoadFileFbx(paths[i]);
	}
}

void LoadFileAnimationFbx(std::string clipName, ::string filaName)
{
	FbxLoader fbx;
	std::vector<CharacterVertex> outSkinnedVertices;
	std::vector<std::uint32_t> outIndices;
	std::vector<Material> outMaterial;
	SkinnedData outSkinnedInfo;

	fbx.LoadFBX(outSkinnedVertices, outIndices, outSkinnedInfo, clipName, outMaterial, filaName);
}

void LoadFolderAnimationFbx(std::string findPath, std::string dir)
{
	struct _finddata_t fd;
	intptr_t handle;
	vector<string> paths;

	if ((handle = _findfirst(findPath.c_str(), &fd)) == -1L)
	{
		cout << "No file in directory!" << endl;
		return;
	}
	do
	{
		cout << fd.name << endl;
		paths.push_back(fd.name);
	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);

	cout << "Fbx Count is " << paths.size() << endl;
	for (int i = 0; i < paths.size(); ++i)
	{
	for (int j = 0; j < 4; ++j)
			paths[i].pop_back();
		LoadFileAnimationFbx(paths[i].c_str(), dir);
	}

}
