#include "Spark/Sparky/Parser.hpp"
#include "Spark/Sparky/Lexer.hpp"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace Spark::Sparky{

    std::string Parser::dumpTree(){

        std::string report{"\n=== PARSER ERROR / AST TRACE ==\n"};

        report += "Grammar path:\n";

        if(ruleTrace.empty()){

            report += " (Root)\n";

        }else{

            for(std::size_t x = 0; x < ruleTrace.size(); x ++){

                report += std::string{static_cast<char>(x * 2), ' '} + "-> " + ruleTrace[x] + "\n";
            }
        }

        const auto& curr = current();

        report += "\nFailed at Token: '" + curr.text + "' (Type: " + std::to_string(static_cast<int>(curr.type)) + ")\n";
        report += "Location: Line " + std::to_string(curr.location.line) + ", Col " + std::to_string(curr.location.column);

        report += "\nToken Stream Context:\n";

        int start = static_cast<int>(cursor) - 5;
        int end = static_cast<int>(cursor) + 5;

        for(int x = start; x <= end; x ++){

            if(x >= 0 && x < static_cast<int>(tokens.size())){

                if(x == static_cast<int>(cursor)){

                    report += "[ >>> " + tokens[x].text + " <<< ]";

                }else{

                    report += " " + tokens[x].text + " ";
                }
            }        
        }

        report += "\n=================================\n";

        return report;
    }

    Program Parser::buildAST(){
        
        return parseProgram();
    }

    Token& Parser::current(){

        return tokens[cursor];
    }

    Token& Parser::peek(std::size_t offset){

        if(cursor + offset >= tokens.size()) return tokens.back();

        return tokens[cursor + offset];
    }

    void Parser::consume(){

        parsed += current().text + "\n";

        if(cursor < tokens.size() -1) cursor ++;


    }

    void Parser::expect(Token::Type type){

        if(current().type != type) throw std::runtime_error(dumpTree());

        consume();
    }

    TypeSpecifier Parser::parseSpecifier(){

        ParseScope scope{*this, "TypeSpecifier: " + current().text};
        
        std::string typeText = current().text;

        switch(current().type){

            case Token::Type::IntType:

                consume();

                return TypeSpecifier{typeText};

            case Token::Type::UIntType:
            
                consume();

                return TypeSpecifier{typeText};

            case Token::Type::FloatType:

                consume();

                return TypeSpecifier{typeText};

            case Token::Type::StringType:

                consume();

                return TypeSpecifier{typeText};

            case Token::Type::BoolType:

                consume();

                return TypeSpecifier{typeText};

            case Token::Type::CharType:

                consume();

                return TypeSpecifier{typeText};

            case Token::Type::VoidType:

                consume();

                return TypeSpecifier{typeText};

            case Token::Type::RefType:

                consume();

                return TypeSpecifier{typeText};

            case Token::Type::Identifier:

                consume();

                return TypeSpecifier{typeText};

            default:

            throw std::runtime_error("Invalid type specifier: " + current().text + "\n" + dumpTree());
            
        }
    }

    IntLiteral Parser::parseInt(){

        ParseScope scope{*this, "IntegerLiteral: " + current().text};

        if(current().type != Token::Type::IntLiteral) throw std::runtime_error("Expected integer literal:\n" + dumpTree());

        IntegerValue val = std::stoi(current().text);

        consume();

        return IntLiteral{val};
    }

    FloatLiteral Parser::parseFloat(){

        ParseScope scope{*this, "FloatLiteral: " + current().text};

        if(current().type != Token::Type::FloatLiteral) throw std::runtime_error("Expected float literal:\n" + dumpTree());

        FloatValue val = std::stod(current().text);

        consume();
        
        return FloatLiteral{val};
    }

    StringLiteral Parser::parseString(){

        ParseScope scope{*this, "StringLiteral: " + current().text};

        if(current().type != Token::Type::StringLiteral) throw std::runtime_error("Expected string literal:\n" + dumpTree());

        std::string value{current().text};

        consume();

        return StringLiteral{value};
    }

    BoolLiteral Parser::parseBool(){

        ParseScope scope{*this, "BoolLiteral: " + current().text};

        if(current().type != Token::Type::True && current().type != Token::Type::False) throw std::runtime_error("Expected bool literal:\n" + dumpTree());

        bool val = current().text == "true";

        consume();

        return BoolLiteral{val};

    }

    NoneLiteral Parser::parseNone(){

        ParseScope scope{*this, "NoneLiteral: " + current().text};

        if(current().type != Token::Type::None) throw std::runtime_error("Expected None literal" + dumpTree());

        consume();

        return NoneLiteral{};

    }

    ThisExpr Parser::parseThis(){

        ParseScope scope{*this, "ThisExpression: " + current().text};

        if(current().type != Token::Type::This) throw std::runtime_error("Expected 'this' literal" + dumpTree());

        consume();

        return ThisExpr{};
    }

    VariableRef Parser::parseRef(){

        ParseScope scope{*this, "VariableReference: " + current().text};

        if(current().type != Token::Type::Identifier) throw std::runtime_error("Expected namespace or variable identifier" + dumpTree());

        std::vector<std::string> path{};

        path.push_back(current().text);

        consume();

        while(current().text == "::"){

            consume();

            if(current().type != Token::Type::Identifier) throw std::runtime_error("Expected identifier after ::" + dumpTree());

            path.push_back(current().text);

            consume();
        }

        return VariableRef{std::move(path)};
    }

    Expr Parser::parseBinary(){

        ParseScope scope{*this, "BinaryExpression: " + current().text};
        
        Expr left = parsePrimaryOrUnary();

        while(true){

            Token::Type type = current().type;

            bool isBinaryOperation = (type == Token::Type::Plus || type == Token::Type::Minus ||
                        type == Token::Type::Star || type == Token::Type::Slash ||
                        type == Token::Type::Mod  || type == Token::Type::Equals ||
                        type == Token::Type::NotEquals || type == Token::Type::LessThan ||
                        type == Token::Type::GreaterThan || type == Token::Type::LessThanEquals ||
                        type == Token::Type::GreaterThanEquals || type == Token::Type::And ||
                        type == Token::Type::Or);
            
            if(!isBinaryOperation) break;

            std::string op = current().text;

            consume();

            Expr right = parsePrimaryOrUnary();

            BinaryExpr binExpr{};

            binExpr.left = std::make_unique<Expr>(std::move(left));
            binExpr.operation = op;
            binExpr.right = std::make_unique<Expr>(std::move(right));

            Expr newExpr{};

            newExpr.node = std::move(binExpr);
            left = std::move(newExpr);
        }

        return left;
    }

   UnaryExpr Parser::parseUnary(){

        ParseScope scope{*this, "Unary solving: " + current().text};

            Token::Type type{current().type};

            consume();

            UnaryExpr unary{};

            switch(type){

                case Token::Type::Increment:

                    unary.operation = "++";
                    break;

                case Token::Type::Decrement:

                    unary.operation = "--";
                    break;

                case Token::Type::Not:

                    unary.operation = "!";
                    break;

                case Token::Type::Minus:

                    unary.operation = "-";
                    break;

                default:

                    throw std::runtime_error("Unexpected Unary Operand:\n" + dumpTree());
        }

        Expr operand = parseExpression();

        unary.operand = std::make_unique<Expr>(std::move(operand));

        return unary;
    }

    CharacterLiteral Parser::parseChar(){

        ParseScope scope{*this, "CharacterLiteral: " + current().text};

        if(current().type != Token::Type::CharacterLiteral) throw std::runtime_error("Expected character literal:\n" + dumpTree());

        CharacterLiteral literal{};

        std::string value{current().text};

        consume();

        return CharacterLiteral{value[1]};

    }

    Expr Parser::parsePrimaryOrUnary(){

        ParseScope scope{*this, "Unary solving: " + current().text};

        Token::Type type = current().type;

        Expr expression{};

        switch(type){

            case Token::Type::IntLiteral:

                expression.node = parseInt();
                return expression;
            
            case Token::Type::FloatLiteral:

                expression.node = parseFloat();
                return expression;
            
            case Token::Type::StringLiteral:

                expression.node = parseString();
                return expression;

            case Token::Type::CharacterLiteral:

                expression.node = parseChar();
                return expression;

            case Token::Type::True:

                expression.node = parseBool();
                return expression;

            case Token::Type::False:

                expression.node = parseBool();
                return expression;

            case Token::Type::None:

                expression.node = parseNone();
                return expression;

            case Token::Type::This:

                expression.node = parseThis();
                return expression;

            case Token::Type::LBracket:

                expression.node = parseArray();
                return expression;

            case Token::Type::Increment:

                expression.node = parseUnary();
                return expression;

            case Token::Type::Decrement:

                expression.node = parseUnary();
                return expression;

            case Token::Type::Not:

                expression.node = parseUnary();
                return expression;

            case Token::Type::Minus:

                expression.node = parseUnary();
                return expression;

            case Token::Type::LParen:

                consume();
                expression = parseExpression();
                expect(Token::Type::RParen);
                return expression;
            
            case Token::Type::Identifier:

                if(peek(1).type == Token::Type::LParen || peek(1).type == Token::Type::DoubleColon){

                    std::size_t lookahead{1};

                    while(peek(lookahead).type == Token::Type::LParen){

                        lookahead += 2;
                    }

                    if(peek(lookahead).type == Token::Type::LParen){

                        expression.node = parseCall();
                        return expression;
                    }
                }

                expression.node = parseRef();

                return expression;

            default:

                throw std::runtime_error("Unexpected token in expression: " + current().text + "\n" + dumpTree());
        }
    }

    FunctionCall Parser::parseCall(){

        ParseScope scope{*this, "FunctionCall: " + current().text};
        
        FunctionCall call{};

        call.calle = parseRef();

        expect(Token::Type::LParen);

        while(current().type != Token::Type::RParen){

            call.args.push_back(std::make_unique<Expr>(parseExpression()));

            if(current().type == Token::Type::Comma) consume();
        }

        expect(Token::Type::RParen);

        return call;
    }

    ArrayLiteral Parser::parseArray(){

        ParseScope scope{*this, "ArrayLiteral: " + current().text};

        ArrayLiteral array{};

        expect(Token::Type::LBracket);

        while(current().type != Token::Type::RBracket){

            array.elements.push_back(std::make_unique<Expr>(parseExpression()));

            if(current().type == Token::Type::Comma) consume();
        }

        expect(Token::Type::RBracket);

        return array;

    }

    Expr Parser::parseExpression(){

        ParseScope scope{*this, "EXpression: " + current().text};

        return parseBinary();

    }

    VariableDecl Parser::parseVariable(){

        ParseScope scope{*this, "VariableDeclaration: " + current().text};

        if(current().type != Token::Type::Identifier) throw std::runtime_error("Expected variable name:\n" + dumpTree());

        VariableDecl decl{};

        decl.name = current().text;

        consume();

        if(current().type == Token::Type::ColonAssign){

            consume();

            decl.initializer = std::make_unique<Expr>(parseExpression());

            return decl;
        }

        expect(Token::Type::Colon);

        decl.type = parseSpecifier();

        if(current().type == Token::Type::Assign){

            consume();

            decl.initializer = std::make_unique<Expr>(parseExpression());
        }

        return decl;
    }

    Assignment Parser::parseAssign(){

        ParseScope scope{*this, "Assignment: " + current().text};

        Assignment assign{};

        if(current().type == Token::Type::This){

            assign.isThisAccess = true;

            consume();

            expect(Token::Type::Dot);
        }

        if(current().type != Token::Type::Identifier) throw std::runtime_error("Expected variable name for assignment:\n");

        assign.target = current().text;

        consume();

        if(current().type == Token::Type::Colon){

            expect(Token::Type::Equals);

            if(current().type == Token::Type::This){

                assign.isThisAccess = true;

                consume();

                expect(Token::Type::Dot);
            }

            assign.value = std::make_unique<Expr>(parseExpression());

            return assign;
        }

        expect(Token::Type::Equals);

        assign.value = std::make_unique<Expr>(parseExpression());

        if(current().type == Token::Type::Semi) consume();

        return assign;

    }

    Block Parser::parseBlock(){

        ParseScope scope{*this, "Block: " + current().text};

        Block block{};

        if(current().type == Token::Type::LBrace){

            consume();
        }

        while(current().type != Token::Type::RBrace && current().type != Token::Type::EndOfFile){

            block.statements.push_back(std::make_unique<Stmt>(parseStmt()));
        }

        expect(Token::Type::RBrace);

        return block;

    }

    IfStmt Parser::parseIfStmt(){

        ParseScope scope{*this, "IfStatement: " + current().text};

        IfStmt stmt{};

        expect(Token::Type::If);
        expect(Token::Type::LParen);

        if(peek(1).type == Token::Type::RParen) throw std::runtime_error("Empty condition in if statement:\n" + dumpTree());

        stmt.condition = std::make_unique<Expr>(parseExpression());

        expect(Token::Type::RParen);

        stmt.thenBlock = parseBlock();

        if(current().type == Token::Type::Else){

            
            stmt.elseBlock = parseBlock();

        }

        return stmt;
    }

    ForStmt Parser::parseForStmt(){

        ParseScope scope{*this, "ForStatement: " + current().text};

        ForStmt stmt{};

        expect(Token::Type::For);
        expect(Token::Type::LParen);

        if(peek(1).type == Token::Type::RParen) throw std::runtime_error("Empty condition in for statement:\n" + dumpTree());

        if(peek(1).type == Token::Type::In){

            stmt.iteratorName = current().text;

            consume();
            consume();

            stmt.iterable = std::make_unique<Expr>(parseExpression());

            expect(Token::Type::RParen);

            stmt.body = parseBlock();

            return stmt;
        }

        stmt.iterable = std::make_unique<Expr>(parseExpression());

        expect(Token::Type::RParen);

        stmt.body = parseBlock();

        return stmt;
    }

    WhileStmt Parser::parseWhileStmt(){

        ParseScope scope{*this, "WhileStatement: " + current().text};

        WhileStmt stmt{};

        expect(Token::Type::While);
        expect(Token::Type::LParen);

        if(peek(1).type == Token::Type::RParen) throw std::runtime_error("Empty condition in while statement:\n" + dumpTree());

        stmt.condition = std::make_unique<Expr>(parseExpression());

        expect(Token::Type::RParen);

        stmt.body = parseBlock();

        return stmt;

    }

    ReturnStmt Parser::parseReturnStmt(){

        ParseScope scope{*this, "ReturnStatement: " + current().text};

        expect(Token::Type::Return);

        ReturnStmt stmt{};

        stmt.value = std::make_unique<Expr>(parseExpression());

        return stmt;
    }

    ExprStmt Parser::parseExpressionStmt(){

        ParseScope scope{*this, "ExpressionStatement: " + current().text};

        ExprStmt stmt{};

        stmt.expression = std::make_unique<Expr>(parseExpression());

        if(current().type == Token::Type::Semi) consume();
        
        return stmt;

    }

    ImportStmt Parser::parseImport(){

        ParseScope scope{*this, "Import: " + current().text};

        expect(Token::Type::Import);

        ImportStmt stmt{};

        if(current().type == Token::Type::Identifier){
            
            stmt.moduleName = current().text;

            consume();

            if(current().type == Token::Type::As){

                consume();

                stmt.alias = current().text;

                consume();

                return stmt;
            }

            return stmt;
        }

        if(current().type == Token::Type::LBrace){

            consume();

            while(current().type != Token::Type::RBrace){

                stmt.selectiveImports.push_back(current().text);

                consume();
            }

            expect(Token::Type::RBrace);

            expect(Token::Type::From);

            if(current().type != Token::Type::Identifier) throw std::runtime_error("Expected module name:\n" + dumpTree());

            stmt.moduleName = current().text;

            consume();
            
        }

        throw std::runtime_error("Empty import instruction:\n" + dumpTree());
    }

    Stmt Parser::parseStmt(){

        ParseScope scope{*this, "Statement: " + current().text};

        Stmt stmt{};
        
        switch(current().type){

            case Token::Type::If:

                stmt.node = parseIfStmt();
                break;

            case Token::Type::For:

                stmt.node = parseForStmt();
                break;

            case Token::Type::While:

                stmt.node = parseWhileStmt();
                break;

            case Token::Type::Return:

                stmt.node = parseReturnStmt();
                break;

            case Token::Type::Semi:

                consume();
                break;

            default:

                if(current().type == Token::Type::Identifier && (peek(1).type == Token::Type::Colon ||
                 peek(1).type == Token::Type::ColonAssign)){

                    stmt.node = parseVariable();

                }else if(current().type == Token::Type::Identifier || current().type == Token::Type::This){

                    stmt.node = parseAssign();

                }else{

                    stmt.node = parseExpressionStmt();
                }

                 if(current().type == Token::Type::Semi) consume();
        }

        return stmt;

    }

    Parameter Parser::parseParam(){

        ParseScope scope{*this, "Parameter: " + current().text};

        Parameter param{};

        if(current().type != Token::Type::Identifier) throw std::runtime_error("Expected parameter identifier\n" + dumpTree());

        param.name = current().text;

        consume();

        expect(Token::Type::Colon);

        param.type = parseSpecifier();

        return param;
    }

    FunctionDecl Parser::parseFunc(){

        ParseScope scope{*this, "FunctionDeclaration: " + current().text};

        FunctionDecl function{};

        expect(Token::Type::Fn);

        if(current().type != Token::Type::Identifier) throw std::runtime_error("Expected function name:\n" + dumpTree());

        function.name = current().text;

        consume();

        expect(Token::Type::LParen);

        if(current().type != Token::Type::RParen){

            do{

                function.params.push_back(parseParam());

                if(current().type == Token::Type::Comma){

                    consume();

                }else{

                    break;
                }
            }while(current().type != Token::Type::RParen);
        }

        expect(Token::Type::RParen);

        expect(Token::Type::Arrow);
        
        function.returnType = parseSpecifier();

        expect(Token::Type::LBrace);

        function.body = parseBlock();

        expect(Token::Type::RBrace);

        return function;

    }

    ConstructorDecl Parser::parseConstructor(){

        ParseScope scope{*this, "ConstructorDeclaration: " + current().text};

        ConstructorDecl constructor{};

        if(current().type != Token::Type::Identifier) throw std::runtime_error("Expected constructor identifier:\n" + dumpTree());

        constructor.className = current().text;

        consume();

        expect(Token::Type::LParen);

        if(current().type != Token::Type::RParen){

            do{

                constructor.params.push_back(parseParam());

                if(current().type == Token::Type::Comma){

                    consume();
                    
                }else{

                    break;
                }

            }while(current().type != Token::Type::RParen);
        }

        expect(Token::Type::RParen);

        expect(Token::Type::LBrace);

        constructor.body = parseBlock();

        expect(Token::Type::RBrace);

        return constructor;
    }

    AccessModifier Parser::parseAccess(){

        ParseScope scope{*this, "AccessModifier: " + current().text};

        switch(current().type){

            case Token::Type::Public:

                consume();

                return AccessModifier::Public;
            
            case Token::Type::Protected:

                consume();

                return AccessModifier::Protected;

            case Token::Type::Private:

                consume();

                return AccessModifier::Private;
            
            default:

                throw std::runtime_error("Invalid access modifier: " + current().text + "\n" + dumpTree());

        }
    }

    ClassMember Parser::parseMember(){

        ParseScope scope{*this, "ClassMember: " + current().text};

        ClassMember member{};

        member.access = parseAccess();
        
        switch (current().type){

            case Token::Type::Fn:

                member.member = parseFunc();
                
                break;

            case Token::Type::Identifier:

                if(peek(1).type == Token::Type::Colon){

                    member.member = parseVariable();

                    break;

                }else{

                    member.member = parseConstructor();

                    break;
                }

            default:

            throw std::runtime_error("Unexpected token in class body: \n" + dumpTree());
        }

        return member;
    }

    ClassDecl Parser::parseClass(){

        ParseScope scope{*this, "Class: " + current().text};

        ClassDecl classDecl{};

        expect(Token::Type::Class);

        if(current().type != Token::Type::Identifier) throw std::runtime_error("Expected class name:\n" + dumpTree());        

        classDecl.name = current().text;

        consume();

        if(current().type == Token::Type::Extends){

            consume();

            classDecl.baseClass = current().text;

            consume();
        }

        expect(Token::Type::LBrace);

        while(current().type != Token::Type::RBrace){

            classDecl.members.push_back(parseMember());
        }

        expect(Token::Type::RBrace);

        return classDecl;
    }

    Program Parser::parseProgram(){
        
        Block rootBlock{};

        ParseScope scope{*this, "Program: " + current().text};

        Program program{};

        while(current().type != Token::Type::EndOfFile){

            if(current().type == Token::Type::Import){

                program.items.push_back(parseImport());

            }else if(current().type == Token::Type::Fn){

                program.items.push_back(parseFunc());

            }else if(current().type == Token::Type::Class){

                program.items.push_back(parseClass());

            }else{
                
                rootBlock.statements.push_back(std::make_unique<Stmt>(parseStmt()));
            }
        }

        for(auto& statement : rootBlock.statements){

            program.items.push_back(std::move(statement));
        }
        
        return program;
    }

}