#pragma once
#include <string>
#include <filesystem>
#include <cstddef>
#include <utility>
#include <unordered_map>

namespace Spark::Sparky{

        struct SourceFile{
        
        std::filesystem::path path{};
        std::string content{};
    };

    struct SourceLocation{

        std::size_t line{1};
        std::size_t column{1};
    };

    struct Token{

        enum class Type : int{
            //keywords
            Import,
            From,
            As,
            Fn,
            Main,
            Class,
            Extends,
            Public,
            Private,
            Protected,
            Const,
            This,
            Cast,
            None,
            If,
            Else,
            For,
            In,
            While,
            Return,
            Break,
            Continue,
            True,
            False,
            //types
            IntType,
            UIntType,
            FloatType,
            CharType,
            StringType,
            BoolType,
            VoidType,
            PtrType,
            //operators and punctuation
            Assign,
            Not,
            ColonAssign,
            Colon,
            DoubleColon,
            Arrow,
            Ref,
            Dot,
            Comma,
            Semi,
            LParen,
            RParen,
            LBracket,
            RBracket,
            LBrace,
            RBrace,
            Plus,
            Increment,
            Minus,
            Decrement,
            Star,
            Slash,
            Mod,
            GreaterThan,
            LessThan,
            GreaterThanEquals,
            LessThanEquals,
            Equals,
            NotEquals,
            And,
            Or,
            //literals and identifiers
            Identifier,
            IntLiteral,
            FloatLiteral,
            StringLiteral,
            Comment,
            BlockComment,
            Ws,

            EndOfFile
        };

        Type type{Type::EndOfFile};
        std::string text{};
        SourceLocation location{};
    };
    
    class Lexer{

        public:

        Lexer() = delete;
        
        explicit Lexer(SourceFile source)
            :
            source(std::move(source)),
            cursor(0)
            {}

        Token nextToken();

        private:

        unsigned char peek() const;
        unsigned char advance();
        bool eof() const;
        void skipWhiteSpaceAndComments();

        Token lexIdentifierKeyword();
        Token lexNumber();
        Token lexString();
        
        SourceFile source{};
        std::size_t cursor{0};
        SourceLocation location{1, 1};

        inline static const std::unordered_map<std::string, Token::Type> keywords = {

        {"import", Token::Type::Import},
        {"from", Token::Type::From},
        {"as", Token::Type::As},
        {"fn", Token::Type::Fn},
        {"main", Token::Type::Main},
        {"class", Token::Type::Class},
        {"extends", Token::Type::Extends},
        {"public", Token::Type::Public},
        {"private",Token::Type::Private},
        {"protected", Token::Type::Protected},
        {"const", Token::Type::Const},
        {"this", Token::Type::This},
        {"cast", Token::Type::Cast},
        {"none", Token::Type::None},
        {"if", Token::Type::If},
        {"else", Token::Type::Else},
        {"for", Token::Type::For},
        {"in", Token::Type::In},
        {"while", Token::Type::While},
        {"return", Token::Type::Return},
        {"break", Token::Type::Break},
        {"continue", Token::Type::Continue},
        {"true", Token::Type::True},
        {"false", Token::Type::False},
        {"string", Token::Type::StringType},
        {"str", Token::Type::StringType},
        {"bool", Token::Type::BoolType},
        {"void", Token::Type::VoidType},
        {"ptr", Token::Type::PtrType},
        {"and", Token::Type::And},
        {"or", Token::Type::Or},
        {"i", Token::Type::IntType},
        {"i8", Token::Type::IntType},
        {"i16", Token::Type::IntType},
        {"i32", Token::Type::IntType},
        {"i64", Token::Type::IntType},
        {"ui", Token::Type::UIntType},
        {"ui8", Token::Type::UIntType},
        {"ui16", Token::Type::UIntType},
        {"ui32", Token::Type::UIntType},
        {"ui64", Token::Type::UIntType},
        {"f", Token::Type::FloatType},
        {"f32", Token::Type::FloatType},
        {"f64", Token::Type::FloatType},
        {"fl", Token::Type::FloatType},
        {"d", Token::Type::FloatType},
        {"dl", Token::Type::FloatType},
        {"uc", Token::Type::CharType},
        {"u", Token::Type::CharType},
        {"c", Token::Type::CharType},
        {"uc16", Token::Type::CharType},
        {"uc32", Token::Type::CharType},
        {"c32", Token::Type::CharType},
        {"c64", Token::Type::CharType},
    };

    };

}