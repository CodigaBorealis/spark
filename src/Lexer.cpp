#include "Spark/Sparky/Lexer.hpp"
#include <cctype>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
namespace Spark::Sparky{

    std::vector<Token> Lexer::tokenize(){

        std::vector<Token> tokens{};

        while(true){

            Token token = nextToken();
            
            tokens.push_back(token);

            if(token.type == Token::Type::EndOfFile) break;
        }

        return tokens;
    }

    Token Lexer::nextToken(){

        skipWhiteSpaceAndComments();

        if(eof()) return {Token::Type::EndOfFile, "", {location.line, location.column}};

        unsigned char c = peek();

        switch (c){

            case '=':

                advance();

                if(peek() == '='){

                    advance();

                    return {Token::Type::Equals, "==", {location.line, location.column}};
                }

                return {Token::Type::Assign, "=", {location.line, location.column}};
            
            case '!':

                advance();

                if(peek() == '='){

                    advance();

                    return {Token::Type::NotEquals, "!=", {location.line, location.column}};
                }

                return {Token::Type::Not, "!", {location.line, location.column}};

            case '<':

                advance();

                if(peek() == '='){

                    advance();

                    return {Token::Type::LessThanEquals, "<=", {location.line, location.column}};
                }

                return {Token::Type::LessThan, "<", {location.line, location.column}};

            case '>':

                advance();

                if(peek() == '='){

                    advance();

                    return {Token::Type::GreaterThanEquals, ">=", {location.line, location.column}};
                }

                return {Token::Type::GreaterThan, ">", {location.line, location.column}};

            case '+':

                advance();

                if(peek() == '+'){

                    advance();

                    return {Token::Type::Increment, "++", {location.line, location.column}};
                }

                return {Token::Type::Plus, "+", {location.line, location.column}};

            case '-':

                advance();

                if(peek() == '-'){

                    advance();

                    return {Token::Type::Decrement, "--", {location.line, location.column}};
                }

                if(peek() == '>'){

                    advance();

                    return {Token::Type::Arrow, "->", {location.line, location.column}};
                }

                return {Token::Type::Minus, "-", {location.line, location.column}};

            case '/':

                advance();

                return {Token::Type::Slash, "/", {location.line, location.column}};
        
            case '%':

                advance();

                return {Token::Type::Mod, "%", {location.line, location.column}};

            case '(':

                advance();

                return {Token::Type::LParen, "(", {location.line, location.column}};

            case ')':

                advance();

                return {Token::Type::RParen, ")", {location.line, location.column}};

            case '{':

                advance();

                return {Token::Type::LBrace, "{", {location.line, location.column}};

             case '}':

                advance();

                return {Token::Type::RBrace, "}", {location.line, location.column}};  
                
            case '[':

                advance();

                return {Token::Type::LBracket, "[", {location.line, location.column}};   
                
            case ']':

                advance();

                return {Token::Type::RBracket, "]", {location.line, location.column}};  
                
            case ',':

                advance();

                return {Token::Type::Comma, ",", {location.line, location.column}};

            case '"':

                return lexString();

            case ':':

                advance();

                if(peek() == ':'){

                    advance();

                    return {Token::Type::DoubleColon, "::", {location.line, location.column}};
                }

                if(peek() == '='){

                    advance();

                    return {Token::Type::ColonAssign, ":=", {location.line, location.column}};
                }

                return {Token::Type::Colon, ":", {location.line, location.column}};
            

            case '&':

                advance();
                return {Token::Type::Ref, "&", {location.line, location.column}};

            

            case '*':
                
                advance();
                return {Token::Type::Star, "*", {location.line, location.column}};

            case '.':

                advance();
                return {Token::Type::Dot, ".", {location.line, location.column}};

            case ';':

                advance();
                return {Token::Type::Semi, ";", {location.line, location.column}};
        }

        if(std::isdigit(c)) return lexNumber();

        if(std::isalnum(c) || c == '_') return lexIdentifierKeyword();

        throw std::runtime_error("NLEXER: Unexpected character '" + std::string{static_cast<char>(c)} + "' at : "
         + std::to_string(location.line) + ":" + std::to_string(location.column));
    }

    unsigned char Lexer::peek() const{

        if(eof()) return '\0';

        return source.content[cursor];
    }

    unsigned char Lexer::advance(){

        if(eof()) return '\0';

        unsigned char current = source.content[cursor];

        if(current == '\n'){

            location.line ++;

            location.column = 1;

        }else{

            location.column ++;
        }

        cursor ++;

        return current;

    }

    bool Lexer::eof() const{

        return cursor >= source.content.size();
    }

    void Lexer::skipWhiteSpaceAndComments(){

        while(!eof()){

            unsigned char c = peek();

            if(c == ' ' || c == '\t' || c == '\r' || c == '\n'){
                
                advance();

            }else if(c == '/' && cursor + 1 < source.content.size() && source.content[cursor + 1] == '/'){
                
                while(!eof() && peek() != '\n'){

                    advance();
                }

            }else if(c == '/' && cursor + 1 < source.content.size() && source.content[cursor + 1] == '*'){

                advance();
                advance();

                while(!eof()){

                    if(peek() == '*' && cursor + 1 < source.content.size() && source.content[cursor + 1] == '/'){

                        advance();
                        advance();
                        break;
                    }

                    advance();
                }
                    
            }else{

                break;
            }
        }
    }
    

    Token Lexer::lexIdentifierKeyword(){

        std::string value{};

        while(!eof()){

            unsigned char c = peek();

            if(std::isalnum(c) || c == '_'){

                value += advance();

            }else{

                break;
            }
        }

        auto it = keywords.find(value);

        if(it != keywords.end()){
            
            if(it->second == Token::Type::Main){
                
                throw std::runtime_error("'main' cannot be used to avoid conflicts during code generation, 'main' found at: " +
                        std::to_string(location.line) + ":" + std::to_string(location.column));
            }

            return {it->second, value, {location.line, location.column}};
    }
        return {Token::Type::Identifier, value, {location.line, location.column}};
    }

    Token Lexer::lexNumber(){

        short dotCount = 0;

        std::string value{};

        while(!eof()){

            unsigned char c = peek();

            if(std::isdigit(c) || c == '.'){

                if(c == '.'){

                    dotCount ++;

                    if(dotCount > 1)throw std::runtime_error("Invalid number literal with multiple decimal points at: " +
                        std::to_string(location.line) + ":" + std::to_string(location.column));
                }

                value += advance();
            }else{

                break;
            }
        }

        if(!value.empty() && value.back() == '.') throw std::runtime_error("Invalid number literal ending with a dot at: " +
             std::to_string(location.line) + ":" + std::to_string(location.column));

        if(value.contains('.')){

            return {Token::Type::FloatLiteral, value, {location.line, location.column}};
        }

        return {Token::Type::IntLiteral, value, {location.line, location.column}};
    }

    Token Lexer::lexString(){

        advance();

        std::string value{};

        while(!eof() && peek() != '"'){

            value += advance();
        }

        if(eof()) throw std::runtime_error("Unterminated string literal at: " + std::to_string(location.line) + ":" + std::to_string(location.column));


        advance();

        return {Token::Type::StringLiteral, value, {location.line, location.column}};

    }

    Token Lexer::lexChar(){

        advance();

        std::string value{};
        std::size_t count{};

        while(!eof() && peek() != '\''){

            value += advance();
            count ++;

            if(count > 1) throw std::runtime_error("Expected single character literal at: "+ std::to_string(location.line) + ":" + std::to_string(location.column));
        }

        if(eof()) throw std::runtime_error("Unterminated character literal at: " + std::to_string(location.line) + ":" + std::to_string(location.column));

        advance();
        
        return {Token::Type::CharacterLiteral, value, {location.line, location.column}};
    }
}