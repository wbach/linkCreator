#include "LinkCreator.h"
#include <Windows.h>
#include <strsafe.h>
#include <iostream>
#pragma comment(lib, "User32.lib")

void LinkCreator::CreateLink(const std::string & from, const std::string & to, bool dir)
{
	std::string o("mklink /D ");

	if (!dir)
		o = std::string("mklink ");

	o += '"' + to + '"' + " " + '"' + from + '"';

	std::cout << "Create cmd : " + o << std::endl;
	system(o.c_str());
}

void LinkCreator::Run(char ** argv)
{
	ProcessInput(argv);
	RunMain();
}

void LinkCreator::Run()
{
	std::cout <<
		"Help : \n"
		"1 - create single link, 'from' - folder to link, 'To' - link, default take the same name as from.\n"
		"2 - create multiple links for all directories in direcotry, 'from' - directory where are directories with links,"
		"'to' -  direcotry where will be new links, links will be the same name as orgin files.\n\n\n";

	std::cout << "Choose : [1-file], [2-directory] : ";

	int chooseFileDir = 0;
	std::cin >> chooseFileDir;
	isDir_ = chooseFileDir == 2;

	int choose = 0;
	std::cout << "Choose : [1-single directory], [2-all-direcotries] : ";
	std::cin >> choose;
	isMulti_ = choose == 2;

	char input[256], output[256];
	std::cout << "FROM : ";
	ClearCin();
	std::cin.getline(input, sizeof(input));
	from_ = std::string(input);

	std::cout << "TO : ";
	std::cin.getline(output, sizeof(output));
	to_ = std::string(output);

	RunMain();
}

int LinkCreator::CheckDir(const std::string & dir)
{
	directories.clear();

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	StringCchCopy(szDir, MAX_PATH, dir.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));


	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return dwError;
	}

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (std::string(ffd.cFileName) == "." || std::string(ffd.cFileName) == "..")
				continue;

			//std::cout << "<DIR> " << ffd.cFileName << "\n";
			directories.push_back(ffd.cFileName);
		}
		else
		{

			/*filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;

			std::cout << ffd.cFileName << " " << filesize.QuadPart << "<bytes>\n";*/
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
	}

	FindClose(hFind);
	return dwError;
}

void LinkCreator::ClearCin()
{
	char input[256];
	std::cin.getline(input, sizeof(input));
}

void LinkCreator::ProcessInput(char ** argv)
{
	std::string fileDir = argv[1];
	std::string single = argv[2];
	to_ = argv[4];
	from_ = argv[3];

	isDir_ = std::stoi(fileDir) == 2;
	isMulti_ = std::stoi(single) == 2;
}

void LinkCreator::RunMain()
{
	if (!to_.empty() && isDir_)
		to_ += '\\';
	if (!from_.empty() && isDir_)
		from_ += '\\';

	if (to_.empty())
	{
		to_ = from_.substr(from_.find_last_of('\\') + 1);
	}

	if (isMulti_ && isDir_)
	{
		CheckDir(from_);
		for (const auto& dir : directories)
			CreateLink(from_ + dir, to_ + dir, isDir_);

	}
	else if (isMulti_ && !isDir_)
	{
		std::cout << "Not implemented." << std::endl;
	}
	else
	{
		CreateLink(from_, to_, isDir_);
	}

}

