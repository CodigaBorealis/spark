#pragma once
#include <cstddef>
#include <string>
#include <utility>

namespace Spark::Sparky{

    struct SourceLocation{

        std::size_t line{1};
        std::size_t column{1};
    };

    struct Token{

        enum class Type : int{

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
            IntType,
            UIntType,
            IntAlias,
            FloatType,
            CharType,
            StringType,
            BoolType,
            VoidType,
            PtrType,
            Assign,
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
            Minus,
            Star,
            Slash,
            Mod,
            GreaterThan,
            LessThan,
            GreaterThanEquals,
            LessThanEquals,
            Equals,
            NotEquals,
            Identifier,
            IntLiteral,
            FloatLiteral,
            StringLiteral,
            Comment,
            BlockComment,
            Ws,
            EndOfFile
        };

        explicit Token(Type type, std::string text)
            :
            type(type),
            text(std::move(text))
            {}

        Type type{Type::EndOfFile};
        std::string text{};
        SourceLocation location{};
    };
}