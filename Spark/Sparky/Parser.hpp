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

        std::vector<Token> tokens{};
        std::size_t cursor{0};

        SourceLocation location{};

        Token& current();
        Token& peek(std::size_t offset);
        
        void consume();
        void expect();
        
        TypeSpecifier parseSpecifier();
        IntLiteral parseInt();
        FloatLiteral parseFloat();
        StringLiteral parseString();
        BoolLiteral paseBool();
        NoneLiteral parseNone();
        ThisExpr parseThis();
        VariableRef parseRef();
        BinaryExpr parseBinary();
        UnaryExpr parseUnary();
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