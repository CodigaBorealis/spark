#include "Spark/Sparky/Sparky.hpp"
#include "Spark/Sparky/AST.hpp"
#include "Spark/Sparky/CppCodeGenerator.hpp"
#include "Spark/Sparky/FileReader.hpp"
#include "Spark/Sparky/Lexer.hpp"
#include "Spark/Sparky/Parser.hpp"
#include "Spark/Sparky/SemanticAnalizer.hpp"
#include <future>
#include <iostream>

namespace Spark{

    bool SparkyEngine::transpileFile(const std::filesystem::path& file){

        try{

        Sparky::FileReader reader{};

        std::string contents{reader.getData(file)};
        
        Sparky::SourceFile source{file, contents};

        Sparky::Lexer lexer{source};

        auto tokens = lexer.tokenize();

        Sparky::Parser parser{tokens};

        Sparky::Program ast = parser.buildAST();

        Sparky::SemanticAnalizer analizer{};

        analizer.analize(ast);

        Sparky::CppCodeGenerator generator{options_};

        generator.generate(ast, file, options_.outputDir);

        return true;

        }catch (const std::exception& e){

            std::cerr << "Error transpiling " << file.filename() << ": " << e.what() << "\n";
            return false;
        }
    }   

    bool SparkyEngine::transpileFiles(const std::vector<std::filesystem::path>& files){

        std::vector<std::future<bool>> futures{};

        futures.reserve(files.size());

        for(const auto& file : files){

            futures.push_back(std::async(std::launch::async, [this, file](){

                return transpileFile(file);
            }));
        }

        bool sucess{true};

        for(auto& f : futures){

            if(!f.get()) sucess = false;
        }

        return sucess;
    }

}