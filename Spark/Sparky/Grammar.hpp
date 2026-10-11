#include <cstdint>
#include <string>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

namespace Spark::Sparky{

    struct Expr;
    struct Stmt;

    using ExprPtr = std::unique_ptr<Expr>;
    using StmtPtr = std::unique_ptr<Stmt>;

    using IntegerValue = std::variant<std::int64_t, std::uint64_t>;

    using FloatValue = std::variant<float, double>;
    using CharValue = std::variant<char, unsigned char>;

    struct TypeSpecifier{

        std::string name{};
    };

    struct IntLiteral{

        IntegerValue value{};
    };

    struct FloatLiteral{

        FloatValue value{};
    };

    struct CharacterLiteral{
        
        CharValue value{};
    };

    struct StringLiteral{

        std::string value{};
    };

    struct BoolLiteral{

        bool value{};
    };

    struct NoneLiteral{

    };

    struct ThisExpr{


    };

    struct VariableRef{

        std::vector<std::string> path{};//QualifiedName(i.e, Math::convert()); &something
    };

    struct BinaryExpr{

        ExprPtr left{};
        std::string operation{};
        ExprPtr right{};
    };

    struct UnaryExpr{

        std::string operation{};
        ExprPtr operand{};
    };

    struct FunctionCall{

        VariableRef calle{};
        std::vector<ExprPtr> args{};
    };

    struct ArrayLiteral{

        std::vector<ExprPtr> elements{};
    };

    struct Expr{

    Expr() = default;
    Expr(Expr&&) = default;
    Expr& operator=(Expr&&) = default;
    Expr(const Expr&) = delete;
    Expr& operator=(const Expr&) = delete;

        std::variant<IntLiteral,
        FloatLiteral, 
        StringLiteral, 
        BoolLiteral,
        NoneLiteral, 
        ThisExpr, 
        VariableRef, 
        BinaryExpr, 
        UnaryExpr, 
        FunctionCall,
        ArrayLiteral> node{};
    };

    struct VariableDecl{

        std::string name{};
        TypeSpecifier type{};
        std::optional<ExprPtr> initializer{};
        bool isConst{false};
    };

    struct Assignment{

        bool isThisAccess{false};
        std::string target{};
        ExprPtr value{};
    };

    struct Block{

        Block();
        ~Block();
        Block(Block&&) noexcept;
        Block& operator=(Block&&) noexcept;
        Block(const Block&) = delete;
        Block& operator=(const Block&) = delete;

        std::vector<StmtPtr> statements{};
    };

    struct IfStmt{

        ExprPtr condition{};
        Block thenBlock{};
        std::optional<Block> elseBlock{};
    };

    struct ForStmt{

        std::string iteratorName{};
        ExprPtr iterable{};
        Block body{};
    };

    struct WhileStmt{

        ExprPtr condition{};
        Block body{};
    };

    struct ReturnStmt{

        std::optional<ExprPtr> value{};
    };

    struct ExprStmt{

        ExprPtr expression{};
    };

    struct ImportStmt{

        std::string moduleName{};
        std::optional<std::string> alias{};
        std::vector<std::string> selectiveImports{};//import {a,b} from Math
    };

    struct Stmt{

        Stmt() = default;
        Stmt(Stmt&&) = default;
        Stmt& operator=(Stmt&&) = default;
        Stmt(const Stmt&) = delete;
        Stmt& operator=(const Stmt&) = delete;

        std::variant<
        VariableDecl,
        Assignment,
        IfStmt,
        ForStmt,
        WhileStmt,
        ReturnStmt,
        ExprStmt,
        ImportStmt
        > node{};
    };
    
    struct Parameter{

        std::string name{};
        TypeSpecifier type{};
    };

    struct FunctionDecl{

        std::string name{};
        std::vector<Parameter> params{};
        TypeSpecifier returnType{};
        Block body{};
    };

    struct ConstructorDecl{

        std::string className{};
        std::vector<Parameter> params{};
        Block body{};
    };

    enum class AccessModifier{

        Public,
        Private,
        Protected
    };

    struct ClassMember{

        AccessModifier access{AccessModifier::Public};
        std::variant<VariableDecl, ConstructorDecl, FunctionDecl> member{};
    };

    struct ClassDecl{

        std::string name{};
        std::optional<std::string> baseClass{};
        std::vector<ClassMember> members{};
    };

    using TopLevelItem = std::variant<ImportStmt, ClassDecl, FunctionDecl, StmtPtr>;//root

    struct Program{

        Program() = default;
        Program(Program&&) = default;
        Program& operator=(Program&&) = default;
        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;

        std::vector<TopLevelItem> items{};
    };
    
}