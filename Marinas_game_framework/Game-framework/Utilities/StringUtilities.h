#pragma once

// Retrieves the filename in a filepath (file extension included) and returns the result as a new std::string
inline std::string GetFileName(const std::string& path)
{
	if (path.empty())
		return path;

	size_t filenameStart = path.find_last_of("/\\");

	if (filenameStart >= path.length())
		return path;

	return path.substr(filenameStart + 1);
}

// Removes the file extension from a filename or a filepath string and returns the result as a new std::string
inline std::string RemoveFileExtension(const std::string& string)
{
	if (string.empty())
		return string;

	size_t fileExtensionStart = string.find_last_of('.');

	if (fileExtensionStart >= string.length())
		return string;

	return string.substr(0, fileExtensionStart);
}