#pragma once
#include "AST.hpp"

namespace Spark::Sparky{

    class ASTPrinter{

        public:

        void print(const Program& program);
        
        private:

        void printIndent(const std::string& prefix, bool isLast);
        std::string nextPrefix(const std::string& prefix, bool isLast);
        void printTopLevelItem(const TopLevelItem& item, const std::string& prefix, bool isLast);
        void printExpr(const Expr& expr, const std::string& prefix, bool isLast);
        void printStmt(const Stmt& stmt, const std::string& prefix, bool isLast);
        void printBlock(const Block& block, const std::string& prefix, bool isLast);
        template<typename Variant, typename Visitor> void unwrap(const Variant& variant, Visitor&& visitor){
            
            std::visit([&visitor](const auto& node){visitor(node);},variant);
        }
    };
}