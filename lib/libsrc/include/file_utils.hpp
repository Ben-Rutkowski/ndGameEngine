#ifndef FILE_UNTILS_HPP
#define FILE_UNTILS_HPP

#include <string>

namespace futil {

std::string readFile(std::string file_path);
std::string readRelativeFile(std::string rel_file_path, std::string root_path);

}

#endif