#pragma once
#include "Grammar.hpp"
#include "Spark/Sparky/Lexer.hpp"
#include <cstddef>
#include <vector>
#include <utility>

namespace Spark::Sparky{

    class Parser{

        public:

        Parser() = delete;

        Parser(std::vector<Token> tokens)
            :
            tokens(std::move(tokens))
            {}

        Program buildAST();

        private:

        struct ParseScope{

            ParseScope(Parser& parser, std::string ruleName)
                :
                parser(parser){

                    parser.ruleTrace.push_back(std::move(ruleName));
                }

            ~ParseScope(){

                parser.ruleTrace.pop_back();
            }


            Parser& parser;
                
        };

        std::vector<Token> tokens{};
        std::vector<std::string> ruleTrace{};
        std::string parsed{};

        std::size_t cursor{0};

        SourceLocation location{};

        Token& current();
        Token& peek(std::size_t offset);
        
        void consume();
        void expect(Token::Type type);

        std::string dumpTree();

        TypeSpecifier parseSpecifier();
        IntLiteral parseInt();
        FloatLiteral parseFloat();
        StringLiteral parseString();
        CharacterLiteral parseChar();
        BoolLiteral parseBool();
        NoneLiteral parseNone();
        ThisExpr parseThis();
        VariableRef parseRef();
        Expr parseBinary();
        UnaryExpr parseUnary();
        Expr parsePrimaryOrUnary();
        FunctionCall parseCall();
        ArrayLiteral parseArray();
        Expr parseExpression();
        VariableDecl parseVariable();
        Assignment parseAssign();
        Block parseBlock();
        IfStmt parseIfStmt();
        ForStmt parseForStmt();
        WhileStmt parseWhileStmt();
        ReturnStmt parseReturnStmt();
        ExprStmt parseExpressionStmt();
        ImportStmt parseImport();
        Stmt parseStmt();
        Parameter parseParam();
        FunctionDecl parseFunc();
        ConstructorDecl parseConstructor();
        AccessModifier parseAccess();
        ClassMember parseMember();
        ClassDecl parseClass();
        Program parseProgram();

    };

}