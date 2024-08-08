
#include "dependencies.hpp"
#include "core.hpp"
#include "utils.hpp"

std::string createFileListHtml(const std::string& dirPath)
{
	std::vector<std::string> files = listFilesInDirectory(dirPath);
	std::string html = "<html><body><h2>Files in Directory:</h2><ul>";

	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
		html += "<li><a href=\"/delete?file=" + *it + "\">" + *it + "</a></li>";
	html += "</ul></body></html>";
	
	return html;
}