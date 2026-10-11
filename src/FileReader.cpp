#include "Spark/Sparky/FileReader.hpp"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

namespace Spark::Sparky{

    std::string FileReader::getData(const std::filesystem::path &path){

        if(!std::filesystem::exists(path)) throw std::runtime_error("File not found: " + path.string());

        if(path.extension().string() != ".spk") throw std::runtime_error("File is not a spark script, expected .spk, got: " + path.extension().string());
        
        std::ifstream file{path, std::ios::binary};

        if(!file) throw std::runtime_error("Failed to open file: " + path.string());

        return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }

}