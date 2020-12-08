#include <cstring>
#include <cassert>
#include "analyser.h"

/*
 * 表达式分析函数
 * 算符优先表
 * +-----------------------------------------+
 * +|     | ( | ) | + | - | * | / | > | < | >= | <= | != | == | 前置- |函数调用| as | 
 * +|------------------------------------------------------------------------------+
 * +| (   | <.| =.| <.| <.| <.| <.| <.| <.| <. | <. | <. | <. |
 * +-------------------------------------------------------------------------------+
 * +| )   |
 * +-------------------------------------------------------------------------------+
 * +| +   |
 * +-------------------------------------------------------------------------------+
 * +| -   |
 * +-------------------------------------------------------------------------------+
 * +| *   |
 * +-------------------------------------------------------------------------------+
 * +| /   |
 * +-------------------------------------------------------------------------------+
 * +| >   |
 * +-------------------------------------------------------------------------------+
 * +| <   |
 * +-------------------------------------------------------------------------------+
 * +| >=  |
 * +-------------------------------------------------------------------------------+
 * +| <=  |
 * +-------------------------------------------------------------------------------+
 * +| !=  |
 * +-------------------------------------------------------------------------------+
 * +| ==  |
 * +-------------------------------------------------------------------------------+
 * +|前置-|
 * +-------------------------------------------------------------------------------+
 * +|函数 |
 * +-------------------------------------------------------------------------------+
 * +| as  |
 * +-------------------------------------------------------------------------------+
 * 表达式执行完成后，结果一定在栈顶
 */

extern char * str[1000];
extern unsigned int nextGlobalOffset;

bool less(TokenType a, TokenType b);

Type Analyser::expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    if (!isExpressionTermination(peek().getTokenType()))
        return Type::VOID;
    // 建立两个栈，一个用于符号，另一个用于保存类型
#ifndef DEBUG
    TokenType op[100];
    Type ty[100];
#else
    TokenType op[5];
    Type ty[5];
#endif // DEBUG
    int t_op = 0;
    int t_ty = 0;
    op[t_op++] = TokenType::NONE; // NONE for '$'

    while (has_next() && isExpressionTermination(peek().getTokenType()))
    {
        if (peek().getTokenType() == TokenType::L_PAREN)
        {
            // 两个表达式不能相邻
            if (t_op <= t_ty)
                throw Error(ErrorCode::BadExpr, "bad expr!", current().getStart());
            ty[t_ty++] = group_expr(vt, ft, fn);
        }
        else if (peek().getTokenType() == TokenType::IDENT)
        {
            if (t_op <= t_ty)
                throw Error(ErrorCode::BadExpr, "bad expr!", current().getStart());
            next();
            if (has_next() && peek().getTokenType() == TokenType::L_PAREN)
            {
                // 函数调用
                previous();
                ty[t_ty++] = call_expr(vt, ft, fn);
            }
            else if(has_next() && peek().getTokenType() == TokenType::ASSIGN)
            {
                previous();
                ty[t_ty++] = assign_expr(vt,ft,fn);
            }
            else
            {
                // 普通标识符
                previous();
                ty[t_ty++] = ident_expr(vt, ft, fn);
            }
        }
        else if (peek().getTokenType() == TokenType::UINT_LITERAL || peek().getTokenType() == TokenType::CHAR_LITERAL || peek().getTokenType() == TokenType::STRING_LITERAL || peek().getTokenType() == TokenType::DOUBLE_LITERAL)
        {
            if (t_op <= t_ty)
                throw Error(ErrorCode::BadExpr, "bad expr!", current().getStart());
            ty[t_ty++] = literal_expr(vt, ft, fn);
        }
        else if (peek().getTokenType() == TokenType::MINUS && t_op > t_ty)
        {
            // 这个减号是前置的
            ty[t_ty++] = negate_expr(vt, ft, fn);
        }
        else if (peek().getTokenType() == TokenType::AS_KW)
        {
            next();
            auto type = expect(TokenType::TY).getValue();
            if (t_op > t_ty || type != "int" && type != "double" || t_ty <= 0)
                throw Error(ErrorCode::BadExpr, "bad expr!", current().getStart());
            if (ty[t_ty - 1] == Type::INT && type == "double")
            {
                ty[t_ty - 1] = Type::DOUBLE;
                fn.addInstruction(Instruction(Operation::ITOF));
            }
            else if (ty[t_ty - 1] == Type::DOUBLE && type == "int")
            {
                ty[t_ty - 1] = Type::INT;
                fn.addInstruction(Instruction(Operation::FTOI));
            }
            else
                throw Error(ErrorCode::BadExpr, "type change fail!", current().getStart());
        }
        else
        {
            auto type = peek().getTokenType();
            if (t_op != t_ty || type == TokenType::ASSIGN || type == TokenType::TY)
                throw Error(ErrorCode::BadExpr, "bad expr", current().getStart());
            else if(t_ty == 1 && (type == TokenType::R_PAREN || type == TokenType::COMMA))
                return ty[0];
            // 这里开始使用算符优先文法
            // 不会出现 (、ident、和前置-，这时一定是符号

            if (less(op[t_op - 1], type))
            {
                // 移入
                op[t_op++] = next().getTokenType();
            }
            else
            {
                // 归约
                if (ty[t_ty - 1] != ty[t_ty - 2])
                    throw Error(ErrorCode::TypeNotMatch, "type does not match!", current().getStart());
                auto tmp = operator_expr(ty[t_ty-1],op[t_op-1],fn);
                t_ty -= 2;
                t_op--;
                ty[t_ty++] = tmp;
            }
        }
    }
    while (op[t_op-1]!=TokenType::NONE)
    {
        if (ty[t_ty - 1] != ty[t_ty - 2])
            throw Error(ErrorCode::TypeNotMatch, "type does not match!", current().getStart());
        ty[t_ty++] = operator_expr(ty[t_ty - 1], op[t_op - 1], fn);
        t_ty -= 2;
        t_op--;
        
    }
    return ty[0];
}
Type Analyser::operator_expr(Type t, TokenType op, Function &fn)
{
    switch (op)
    {
    case TokenType::PLUS:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::ADD_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::ADD_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t add!", current().getStart());
        return t;
    }
    case TokenType::MINUS:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::SUB_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::SUB_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t sub!", current().getStart());
        return t;
    }
    case TokenType::MUL:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::MUL_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::MUL_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t mul!", current().getStart());
        return t;
    }
    case TokenType::DIV:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::DIV_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::DIV_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t div!", current().getStart());
        return t;
    }
    case TokenType::LE:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::CMP_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::CMP_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t cmp!", current().getStart());
        fn.addInstruction(Instruction(Operation::SET_GT));
        fn.addInstruction(Instruction(Operation::NOT));
        return Type::BOOL;
    }
    case TokenType::GE:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::CMP_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::CMP_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t cmp!", current().getStart());
        fn.addInstruction(Instruction(Operation::SET_LT));
        fn.addInstruction(Instruction(Operation::NOT));
        return Type::BOOL;
    }
    case TokenType::LT:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::CMP_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::CMP_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t cmp!", current().getStart());
        fn.addInstruction(Instruction(Operation::SET_LT));
        return Type::BOOL;
    }
    case TokenType::GT:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::CMP_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::CMP_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t cmp!", current().getStart());
        fn.addInstruction(Instruction(Operation::SET_GT));
        return Type::BOOL;
    }
    case TokenType::NEQ:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::CMP_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::CMP_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t cmp!", current().getStart());
        return Type::BOOL;
    }
    case TokenType::EQ:
    {
        if (t == Type::INT)
            fn.addInstruction(Instruction(Operation::CMP_I));
        else if (t == Type::DOUBLE)
            fn.addInstruction(Instruction(Operation::CMP_F));
        else
            throw Error(ErrorCode::BadExpr, "can\'t cmp!", current().getStart());
        fn.addInstruction(Instruction(Operation::NOT));
        return Type::BOOL;
    }
    default:
        throw Error(ErrorCode::BadExpr, "no operation!", current().getStart());
    }
    return Type::VOID;
}
Type Analyser::negate_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::MINUS);
    auto type = expr(vt, ft, fn);
    if (type == Type::INT)
        fn.addInstruction(Instruction(Operation::NEG_I));
    else if (type == Type::DOUBLE)
        fn.addInstruction(Instruction(Operation::NEG_F));
    return type;
}
Type Analyser::assign_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    auto &name = expect(TokenType::IDENT).getValue();
    Varible l_expr = vt.get(name, current().getStart());
    if (l_expr.isConst())
        throw Error(ErrorCode::ConstValueChanged, "you can\'t assign to a const varible!", current().getStart());
    expect(TokenType::ASSIGN);
    if (l_expr.getKind() == Kind::GLOBAL)
        fn.addInstruction(Instruction(Operation::GLOBA, l_expr.getAddress()));
    else if (l_expr.getKind() == Kind::PARAM)
        fn.addInstruction(Instruction(Operation::ARGA, l_expr.getAddress()));
    else if (l_expr.getKind() == Kind::VAR)
        fn.addInstruction(Instruction(Operation::LOCA, l_expr.getAddress()));
    else
        throw Error(ErrorCode::InvalidAssign, "invalid assign", current().getStart());
    auto type = expr(vt, ft, fn);
    if (type != l_expr.getType())
        throw Error(ErrorCode::InvalidAssign, "invalid assign", current().getStart());
    fn.addInstruction(Instruction(Operation::STORE_64));
    return Type::VOID;
}
Type Analyser::as_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    return Type::VOID;
}

Type Analyser::call_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    auto &name = expect(TokenType::IDENT).getValue();

    // 首先检查是否是标准库中的参数
    if (name == "getint")
    {
        expect(TokenType::L_PAREN);
        expect(TokenType::R_PAREN);
        fn.addInstruction(Instruction(Operation::SCAN_I));
        return Type::INT;
    }
    else if (name == "getdouble")
    {
        expect(TokenType::L_PAREN);
        expect(TokenType::R_PAREN);
        fn.addInstruction(Instruction(Operation::SCAN_F));
        return Type::DOUBLE;
    }
    else if (name == "getchar")
    {
        expect(TokenType::L_PAREN);
        expect(TokenType::R_PAREN);
        fn.addInstruction(Instruction(Operation::SCAN_C));
        return Type::INT;
    }
    else if (name == "putln")
    {
        expect(TokenType::L_PAREN);
        expect(TokenType::R_PAREN);
        fn.addInstruction(Instruction(Operation::PRINTLN));
        return Type::VOID;
    }
    else if (name == "putint")
    {
        expect(TokenType::L_PAREN);
        if (expr(vt, ft, fn) != Type::INT)
            throw Error(ErrorCode::ParamNotMatch, "ParamNotMatch", current().getStart());
        expect(TokenType::R_PAREN);
        fn.addInstruction(Instruction(Operation::PRINT_I));
        return Type::VOID;
    }
    else if (name == "putdouble")
    {
        expect(TokenType::L_PAREN);
        if (expr(vt, ft, fn) != Type::DOUBLE)
            throw Error(ErrorCode::ParamNotMatch, "ParamNotMatch", current().getStart());
        expect(TokenType::R_PAREN);
        fn.addInstruction(Instruction(Operation::PRINT_F));
        return Type::VOID;
    }
    else if (name == "putchar")
    {
        expect(TokenType::L_PAREN);
        if (expr(vt, ft, fn) != Type::INT)
            throw Error(ErrorCode::ParamNotMatch, "ParamNotMatch", current().getStart());
        expect(TokenType::R_PAREN);
        fn.addInstruction(Instruction(Operation::PRINT_C));
        return Type::VOID;
    }
    else if (name == "putstr")
    {
        expect(TokenType::L_PAREN);
        if (peek().getTokenType() == TokenType::STRING_LITERAL)
            literal_expr(vt, ft, fn);
        else if (expr(vt, ft, fn) != Type::INT)
            throw Error(ErrorCode::ParamNotMatch, "ParamNotMatch", current().getStart());
        expect(TokenType::R_PAREN);
        fn.addInstruction(Instruction(Operation::PRINT_S));
        return Type::VOID;
    }

    Function &function = ft.get(name, current().getStart());
    if(function.getReturnType() != Type::VOID) 
        fn.addInstruction(Instruction(Operation::STACKALLOC,(unsigned int)1));

    // 准备参数，并检查参数列表和函数的声明是否匹配，包括参数个数和类型
    expect(TokenType::L_PAREN);

    for (const auto &type : function.getParamTypes())
    {
        if (peek().getTokenType() == TokenType::R_PAREN)
            throw Error(ErrorCode::ParamNotEnough, "param is not enough", current().getStart());
        else if (type != expr(vt, ft, fn))
            throw Error(ErrorCode::ParamNotMatch, "param not match", current().getStart());
        if (peek().getTokenType() != TokenType::COMMA && peek().getTokenType() != TokenType::R_PAREN)
            throw Error(ErrorCode::InvalidCallList, "invalid call list!", current().getStart());
        if (peek().getTokenType() == TokenType::COMMA)
            next();
    }

    expect(TokenType::R_PAREN);

    fn.addInstruction(Instruction(Operation::CALL, function.getFid()));
    return function.getReturnType();
}
Type Analyser::literal_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    auto t = peek().getTokenType();
    if (t == TokenType::UINT_LITERAL)
    {
        unsigned long long value = std::stoull(peek().getValue());
        fn.addInstruction(Instruction(Operation::PUSH, value));
        next();
        return Type::INT;
    }
    else if (t == TokenType::DOUBLE_LITERAL)
    {
        double value = std::stod(peek().getValue());
        fn.addInstruction(Instruction(Operation::PUSH, value));
        next();
        return Type::DOUBLE;
    }
    else if (t == TokenType::CHAR_LITERAL)
    {
        unsigned long long value = peek().getValue()[0];
        fn.addInstruction(Instruction(Operation::PUSH, value));
        next();
        return Type::INT;
    }
    else if (t == TokenType::STRING_LITERAL)
    {
        Varible var;
        var.setName("");
        var.setType(Type::STRING);
        var.setSize(peek().getValue().size());
        var.setKind(Kind::GLOBAL);
        var.setConst(false);
        var.setAddress(nextGlobalOffset++);
        unsigned int addr = var.getAddress();

        int len = peek().getValue().size() + 1;
        assert(str[addr] == nullptr);
        str[addr] = new char[len];
        strcpy(str[addr],peek().getValue().c_str());
        auto &globals = vt.getGlobal();
        globals.insert(var, current().getStart());
        fn.addInstruction(Instruction(Operation::PUSH, (unsigned long long)addr));
        next();
        return Type::INT;
    }
    return Type::VOID;
}
Type Analyser::ident_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    auto &name = expect(TokenType::IDENT).getValue();
    auto &var = vt.get(name, current().getStart());
    if (var.getKind() == Kind::GLOBAL)
        fn.addInstruction(Instruction(Operation::GLOBA, var.getAddress()));
    else if (var.getKind() == Kind::PARAM)
        fn.addInstruction(Instruction(Operation::ARGA, var.getAddress()));
    else if (var.getKind() == Kind::VAR)
        fn.addInstruction(Instruction(Operation::LOCA, var.getAddress()));
    fn.addInstruction(Instruction(Operation::LOAD_64));
    return var.getType();
}
Type Analyser::group_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::L_PAREN);
    auto ret = expr(vt, ft, fn);
    expect(TokenType::R_PAREN);
    return ret;
}
bool less(TokenType a,TokenType b)
{
    // 只用考虑 + -  * / 和比较
    if(a == b) return false;
    else if(a == TokenType::NONE) return b != TokenType::NONE;
    else if(b == TokenType::NONE) return false;
    else if(a == TokenType::LT || a == TokenType::GT || a == TokenType::LE || a == TokenType::GE){
        if(b == TokenType::LT || b == TokenType::GT || b == TokenType::LE || b == TokenType::GE || b == TokenType::NONE)
            return false;
        return true;
    }
    else if(a == TokenType::PLUS || a == TokenType::MINUS)
    {
        if(b == TokenType::MUL || b == TokenType::DIV) return true;
        return false;
    }
    else return false;
}