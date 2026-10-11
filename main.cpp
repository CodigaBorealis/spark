#include "Spark/Sparky/FileReader.hpp"
#include "Spark/Sparky/Parser.hpp"
#include "Spark/Sparky/Lexer.hpp"
#include <iostream>
int main(){
        
    
    std::filesystem::path file{"helloWorld.spk"};

    Spark::Sparky::FileReader reader{};

    std::string contents = reader.getData(file);

    Spark::Sparky::SourceFile source{file, contents};

    Spark::Sparky::Lexer lexer{source};

    auto tokens = lexer.tokenize();


    for(const auto& t : tokens){

        std::cout << t.text;
    }

    //Spark::Sparky::Parser parser{tokens};

    //auto ast = parser.buildAST();

    //parser.dumpTree();

    return 0;
}