#include "Spark/Sparky/ASTPrinter.hpp"
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <variant>

namespace Spark::Sparky{

    void ASTPrinter::print(const Program &program){

        std::cout << "\n>>>>>>>>> AST STRUCTURE<<<<<<<<\n";

        std::cout << "[Program]\n";

        for(std::size_t x = 0; x < program.items.size(); x ++){

            bool isLast = (x == program.items.size() - 1);

            printTopLevelItem(program.items[x], "", isLast);
        }

        std::cout  << "\n>>>>>>>>>>> END <<<<<<<<<<<<\n";
    }

    void ASTPrinter::printIndent(const std::string &prefix, bool isLast){

        std::cout << prefix << (isLast ? "|" : "|----- ");
    }

    std::string ASTPrinter::nextPrefix(const std::string &prefix, bool isLast){

        return prefix + (isLast ? "    " : "|   ");
    }

    void ASTPrinter::printExpr(const Expr &expr, const std::string &prefix, bool isLast){

        printIndent(prefix, isLast);

        unwrap(expr.node, [&](const auto& node){

            using T = std::decay_t<decltype(node)>;

            if constexpr(std::is_same_v<T, IntLiteral>){
                
                std::visit([](auto value){

                    std::cout << "[Integer literal]: " << value << '\n';

                }, node.value);

            }else if constexpr(std::is_same_v<T, FloatLiteral>){

                std::visit([](auto value){

                    std::cout << "[Float literal]: " << value << '\n';

                }, node.value);

            }else if constexpr(std::is_same_v<T, StringLiteral>){

                std::cout << "[String literal]: \"" << node.value << "\"\n";

            }else if constexpr(std::is_same_v<T, CharacterLiteral>){

                std::cout << "[Character literal]: ";

                std::visit([](auto value){

                    std::cout << value;
                }, node.value);

                std::cout << '\n';

            }else if constexpr(std::is_same_v<T, BoolLiteral>){

                std::cout << "[Boolean literal]: " << std::boolalpha << node.value << '\n';

            }else if constexpr(std::is_same_v<T, NoneLiteral>){

                std::cout << "[None literal]\n";

            }else if constexpr(std::is_same_v<T, ThisExpr>){

                std::cout << "[This]\n";

            }else if constexpr(std::is_same_v<T, VariableRef>){

                std::cout << "[Variable reference]: ";

                for(std::size_t x = 0; x < node.path.size(); x ++){

                    if (x > 0) std::cout << "::";

                    std::cout << node.path[x];
            }

            std::cout << '\n';

            }else if constexpr(std::is_same_v<T, BinaryExpr>){

                std::cout << "[Binary expression]: " << node.operation << '\n';

                const auto childPrefix = nextPrefix(prefix, isLast);

                if(node.left){

                    printExpr(*node.left, childPrefix, false);
                }

                if(node.right){

                    printExpr(*node.right, childPrefix, true);
                }

            }else if constexpr(std::is_same_v<T, UnaryExpr>){

                std::cout << "[Unary expression]: " << node.operation << '\n';

            if(node.operand){

                printExpr(*node.operand, nextPrefix(prefix, isLast), true);
            }

            }else if constexpr (std::is_same_v<T, FunctionCall>){

                std::cout << "[Function call]\n";

                const auto childPrefix = nextPrefix(prefix, isLast);


                printIndent(childPrefix, node.args.empty());

                std::cout << "[Callee]: ";

                for(std::size_t x = 0; x < node.calle.path.size(); x ++){

                    if (x > 0) std::cout << "::";

                    std::cout << node.calle.path[x];
                }

                std::cout << '\n';
            

            for(std::size_t x = 0; x < node.args.size(); x ++){

                if(node.args[x]){

                    printExpr(*node.args[x], childPrefix, x == node.args.size() - 1);
                }
            }

            }else if constexpr (std::is_same_v<T, ArrayLiteral>){

                std::cout << "[Array literal]\n";

                const auto childPrefix = nextPrefix(prefix, isLast);

                for(std::size_t x = 0; x < node.elements.size(); x ++){

                    if(node.elements[x]){

                        printExpr(*node.elements[x], childPrefix, x == node.elements.size() - 1);
                    }
                }
            }
        });
    }

    void ASTPrinter::printTopLevelItem(const TopLevelItem &item, const std::string &prefix, bool isLast){

        unwrap(item, [&](const auto& node){

            using T = std::decay_t<decltype(node)>;

            if constexpr (std::is_same_v<T, ImportStmt>){

                printIndent(prefix, isLast);

                std::cout << "[Import statement]: "<< node.moduleName;

                if(node.alias){

                    std::cout << " as " << *node.alias;
                }

                std::cout << '\n';

            }else if constexpr(std::is_same_v<T, FunctionDecl>){

                printIndent(prefix, isLast);

                std::cout << "[Function declaration]: " << node.name << '\n';

                const auto childPrefix = nextPrefix(prefix, isLast);

                
                printBlock(node.body, childPrefix, true);
                

            }else if constexpr (std::is_same_v<T, ClassDecl>){

                printIndent(prefix, isLast);

                std::cout << "[Class declaration]: " << node.name;

                if(node.baseClass){

                    std::cout << " extends " << *node.baseClass;
                }

                std::cout << '\n';

            }else if constexpr (std::is_same_v<T, StmtPtr>){

                if(node){
                    
                    printStmt(*node, prefix, isLast);
                }
            }
        });
    }

    void ASTPrinter::printStmt(const Stmt &stmt, const std::string &prefix, bool isLast){

        unwrap(stmt.node, [&](const auto& node){

            using T = std::decay_t<decltype(node)>;

            printIndent(prefix, isLast);

            if constexpr(std::is_same_v<T, VariableDecl>){

                std::cout << "[Variable declaration]: " << node.name << '\n';

                const auto childPrefix = nextPrefix(prefix, isLast);

                printIndent(childPrefix, !node.initializer);
                std::cout << "[Type]: " << node.type.name << '\n';
                

                if(node.initializer && *node.initializer){

                    printExpr(**node.initializer, childPrefix, true);
                }

            }else if constexpr(std::is_same_v<T, Assignment>){

                std::cout << "[Assignment]: " << (node.isThisAccess ? "this." : "") << node.target << '\n';

                if(node.value){

                    printExpr(*node.value, nextPrefix(prefix, isLast), true);
                }

            }else if constexpr (std::is_same_v<T, IfStmt>){

                std::cout << "[If statement]\n";

                const auto childPrefix = nextPrefix(prefix, isLast);

                if(node.condition){

                    printExpr(*node.condition, childPrefix, false);
                }

                printBlock(node.thenBlock, childPrefix, !node.elseBlock.has_value());
                
                if(node.elseBlock){

                    printIndent(childPrefix, true);
                    std::cout << "[Else block]\n";

                    printBlock(*node.elseBlock, nextPrefix(childPrefix, true), true);

                }

            }else if constexpr (std::is_same_v<T, ForStmt>){

                std::cout << "[For statement]: " << node.iteratorName << '\n';

                const auto childPrefix = nextPrefix(prefix, isLast);

                if(node.iterable){

                    printIndent(childPrefix,  false);
                    std::cout << "[Iterable]\n";

                    printExpr(*node.iterable, nextPrefix(childPrefix, false), true);
                }

                
                printBlock(node.body, childPrefix, true);
                

            }else if constexpr(std::is_same_v<T, WhileStmt>){

                std::cout << "[While statement]\n";

                const auto childPrefix = nextPrefix(prefix, isLast);

                if(node.condition){

                    printExpr(*node.condition,  childPrefix, false);
                }


                printBlock(node.body,  childPrefix,  true);
                

            }else if constexpr (std::is_same_v<T, ReturnStmt>){

                std::cout << "[Return statement]\n";

                if(node.value && *node.value){

                    printExpr(**node.value, nextPrefix(prefix, isLast), true);
                }

            }else if constexpr(std::is_same_v<T, ExprStmt>){

                std::cout << "[Expression statement]\n";

                if(node.expression){

                    printExpr(*node.expression, nextPrefix(prefix, isLast), true);
                }

            }else if constexpr(std::is_same_v<T, ImportStmt>){

                std::cout << "[Import statement]: " << node.moduleName;

                if(node.alias){

                    std::cout << " as " << *node.alias;
                }

                std::cout << '\n';
            }
        });
    }

    void ASTPrinter::printBlock(const Block &block, const std::string &prefix, bool isLast){

        printIndent(prefix, isLast);

        std::cout << "[Block]\n";

        const auto childPrefix = nextPrefix(prefix, isLast);

        for(std::size_t x = 0; x < block.statements.size(); x ++){

            if(block.statements[x]){

                printStmt(*block.statements[x], childPrefix, x == block.statements.size() - 1);
            }
        }
    }

}
    
