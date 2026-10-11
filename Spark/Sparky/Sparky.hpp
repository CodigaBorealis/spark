#pragma once
#include <filesystem>
#include <vector>

namespace Spark{

    struct TranspilerOptions{

        std::filesystem::path outputDir{"./build"};
        std::string cppStandard{"c++23"};
        bool generateHeaderSplit{true};
        bool runGxxCompiler{false};
    };

    class SparkyEngine{

        public:

        explicit SparkyEngine(TranspilerOptions options = {})
            :
            options_(std::move(options))
            {}

        bool transpileFile(const std::filesystem::path& file);
        bool transpileFiles(const std::vector<std::filesystem::path>& files);
        
        private:

        TranspilerOptions options_{};

    };

}