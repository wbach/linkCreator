#include "LinkCreator.h"

#include <windows.h>
#include <string>
#include <iostream>
#include <vector>

#include <strsafe.h>
#pragma comment(lib, "User32.lib")

std::vector<std::string> directories;

int CheckDir(const std::string& dir)
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

void ClearCin()
{
	char input[256];
	std::cin.getline(input, sizeof(input));
}

int main(int argc, char* argv[])
{
	LinkCreator linkCreator;

	if (argc <= 1)
		linkCreator.Run();
	else
		linkCreator.Run(argv);

	system("pause");
	return 0;

	int chooseFileDir = 0;
	int choose = 0;

	if (argc <= 1)
	{
		std::cout <<
			"Help : \n"
			"1 - create single link, 'from' - folder to link, 'To' - link, default take the same name as from.\n"
			"2 - create multiple links for all directories in direcotry, 'from' - directory where are directories with links,"
			"'to' -  direcotry where will be new links, links will be the same name as orgin files.\n\n\n";

		std::cout << "Choose : [1-file], [2-directory] : ";

		std::cin >> chooseFileDir;

		std::cout << "Choose : [1-single directory], [2-all-direcotries] : ";
		std::cin >> choose;
	}
	else
	{
		std::string inputPath = argv[1];
		std::string outputPath = argv[2];


		return 0;
	}

	if (choose == 1)
	{
		char input[256], output[256];
		std::cout << "FROM : ";
		ClearCin();
		std::cin.getline(input, sizeof(input));

		std::string src(input);

		if (!src.empty())
		{
			std::cout << "TO : ";
			std::cin.getline(output, sizeof(output));

			std::string src2(output);
			std::string o("mklink /D ");

			if (chooseFileDir == 1)
				o = std::string("mklink ");

			if (src2.empty())
			{
				src2 = src.substr(src.find_last_of('\\') + 1);
			}

			o += '"' + src2 + "\" \"" + src + '"';

			std::cout << "Create cmd : " + o << std::endl;
			system(o.c_str());
		}
	}
	else if (choose == 2)
	{
		char input[256], output[256];
		std::cout << "FROM : ";
		ClearCin();
		std::cin.getline(input, sizeof(input));

		std::cout << "TO : ";
		std::cin.getline(output, sizeof(output));

		std::string src2(output);
		std::string src(input);

		if (!src.empty())
			src += '\\';
		if (!src2.empty())
			src2 += '\\';

		CheckDir(input);

		for (const auto& dir : directories)
		{
			if (dir == "." || dir == "..")
				continue;

			std::string o("mklink /D ");
			if (chooseFileDir == 1)
				o = std::string("mklink ");

			o += '"' + src2 + dir + "\" \"" + src + dir + '"';

			std::cout << "Create cmd : " + o << std::endl;
			system(o.c_str());
		}

	}
	system("pause");
}