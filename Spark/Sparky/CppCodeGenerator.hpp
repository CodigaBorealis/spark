#pragma once

#include "Spark/Sparky/AST.hpp"
#include "Spark/Sparky/Sparky.hpp"
#include <filesystem>
namespace Spark::Sparky{

    class CppCodeGenerator{

        public:
        
        CppCodeGenerator() = delete;

        CppCodeGenerator(TranspilerOptions options)
            :
            options(std::move(options))
            {}

        void generate(const Program&, const std::filesystem::path& sourceDir, const std::filesystem::path& outputDir);

        private:

        TranspilerOptions options{};
    };

}