#pragma once
#include <string>
#include <vector>

class LinkCreator
{
public:
	void CreateLink(const std::string& from, const std::string& to, bool dir);
	void Run(char** argv);
	void Run();

private:
	int CheckDir(const std::string& dir);
	void ClearCin();
	void ProcessInput(char** argv);
	void RunMain();

private:
	std::vector<std::string> directories;
	std::string from_, to_;
	bool isDir_ = false;
	bool isMulti_ = false;
};