#include "file_utils.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

std::string futil::readFile(std::string file_path) {
    const std::string emtpy = "";

    std::ifstream     file_stream;
    std::stringstream string_stream;

    file_stream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        file_stream.open(file_path);
        string_stream << file_stream.rdbuf();
        file_stream.close();

        return string_stream.str();
    } catch(std::ifstream::failure e) {
        std::cout << "Failed to read File at " << file_path << std::endl;

        return emtpy;
    }
}

std::string futil::readRelativeFile(std::string rel_file_path, std::string root_path) {
    std::string full_path = root_path + rel_file_path;
    return readFile(full_path);
}