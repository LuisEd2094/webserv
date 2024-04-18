#include "../includes/dirList.hpp"

std::string dirList(std::string dir_str)
{
	DIR				*dir;
	struct dirent	*file;
	std::string		html("");
	//chdir(directory.c_str()); //TODO try catch
	dir = opendir(dir_str.c_str()); //TODO try catch
	file = readdir(dir);

	html = html + "<html><head></head><body>";	
	html = html + "<h1>Directory listing for " + dir_str + "</h1><hr>";	
	html = html + "<ul>";	
	while (file)
	{
		if (std::string(file->d_name) != "." && std::string(file->d_name) != "..")
			html = html + "<li><a href=" + dir_str + file->d_name + ">" + file->d_name + "</a></li>";
		file = readdir(dir);
	}
	html += "</ul><hr></body></html>";

	closedir(dir);
	return html;
}
