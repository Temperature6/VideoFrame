#pragma once
#include <iostream>
#include <string>

std::string GetFileName(std::string file_path)
{
	int slash_index = file_path.length() - 1;
	int point_index = file_path.length() - 1;
	for (int i = slash_index; i >= 0; i--)
	{
		slash_index = i;
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			break;
		}
		if (i == 0)
		{
			slash_index = -1;
			break;
		}
	}
	for (int i = point_index; i >= 0; i--)
	{
		if (file_path[i] == '.')
		{
			point_index = i;
			break;
		}
		if (i == 0)
		{
			point_index = file_path.length() - 1;
		}
	}
	std::string file_name;

	for (int j = slash_index + 1; j < point_index; j++)
	{
		file_name.push_back(file_path[j]);
	}
	return file_name;
}

std::string GetFileDirectory(std::string abs_path)
{
	int slash_index = 0;
	for (int i = abs_path.length() - 1; i >= 0; i--)
	{
		slash_index = i;
		if (abs_path[i] == '/' || abs_path[i] == '\\')
		{
			break;
		}
	}
	std::string dir;
	for (int i = 0; i <= slash_index; i++)
	{
		dir.push_back(abs_path[i]);
	}
	return dir;
}
