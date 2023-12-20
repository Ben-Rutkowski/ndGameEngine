#ifndef FILE_UNTILS_HPP
#define FILE_UNTILS_HPP

#include <string>

/* NAMESPACE futil

Contains funtions to read files

- readFile : Takes a string of a path and outputs the string of 
    the files contents.
    ==Parameter==
        - string file_path
    ==Return==
        - string

- readRelativeFile : Takes a string of a relative path and a 
    root diretory and outputs a string of the files contents.
    ==Parameters==
        - string rel_file_path
        - string root_path
    ==Return==
        - string

*/

namespace futil {

std::string readFile(std::string file_path);
std::string readRelativeFile(std::string rel_file_path, std::string root_path);

}

#endif