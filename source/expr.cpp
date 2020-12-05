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

/////////////////////////////
/// \brief 将运算符编号
/////////////////////////////
int token_to_int(TokenType ty);
TokenType int_to_token(int id);
int matrix[12][12] = {
    //          $   +   -   *   /   >   <  >=  <=  ==  !=  as
    /* $ */   { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    /* + */   { 1,  1,  1, -1, -1,  1,  1,  1,  1,  1,  1, -1 },
    /* - */   { 1,  1,  1, -1, -1,  1,  1,  1,  1,  1,  1, -1 },
    /* * */   { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
    /* / */   { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1  },
    /* > */   { 1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1, -1 },
    /* < */   { 1, -1 ,-1, -1, -1,  1,  1,  1,  1,  1,  1, -1 },
    /*>= */   { 1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1, -1 },
    /*<= */   { 1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1, -1 },
    /*== */   { 1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1, -1 },
    /*!= */   { 1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1, -1 },
    /*as */   { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 }
};
Type Analyser::expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    int top = 0;
    int op[100];
    Type ty[100];
    op[top++] = 0; // 0 表示 '$'
    if (isExpressionTermination(peek().getTokenType()))
        return Type::VOID;
    while (has_next() && isExpressionTermination(peek().getTokenType()))
    {
        if (peek().getTokenType() == TokenType::L_PAREN)
        {
            // 两个表达式不能相邻
            if (top > 0 && op[top - 1] == -1)
                throw Error(ErrorCode::BadExpr, "bad expr!", current().getStart());
            ty[top] = group_expr(vt, ft, fn);
            op[top] = -1;
            top++;
        }
        else if (peek().getTokenType() == TokenType::IDENT)
        {
            if (top > 0 && op[top - 1] == -1)
                throw Error(ErrorCode::BadExpr, "bad expr!", current().getStart());
            next();
            if (has_next() && peek().getTokenType() == TokenType::L_PAREN)
            {
                // 函数调用
                previous();
                ty[top] = call_expr(vt, ft, fn);
                op[top] = -1;
                top++;
            }
            else
            {
                // 普通标识符
                previous();
                ty[top] = ident_expr(vt, ft, fn);
                op[top] = -1;
                top++;
            }
        }
        else if (peek().getTokenType() == TokenType::MINUS && op[top - 1] != -1)
        {
            // 这个减号是前置的
            ty[top] = negate_expr(vt, ft, fn);
            op[top] = -1;
            top++;
        }
        else {
            // 这里开始使用算符优先文法
            // 不会出现 (、ident、和前置-
            if(token_to_int(peek().getTokenType()) <= 0) throw Error(ErrorCode::BadExpr, "bad expr!", current().getStart());
            int u = op[top-1] == -1 ? op[top-2] : op[top-1]; // u 栈顶符号
            int v = token_to_int(peek().getTokenType());
            if(matrix[u][v] <= 0) op[top++] = v;
            else {
                auto t = int_to_token(u);
            }
        }
    }
}
Type Analyser::operator_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
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
    return type;
}
Type Analyser::as_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
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
            ident_expr(vt, ft, fn);
        else if (expr(vt, ft, fn) != Type::INT)
            throw Error(ErrorCode::ParamNotMatch, "ParamNotMatch", current().getStart());
        expect(TokenType::R_PAREN);
        fn.addInstruction(Instruction(Operation::PRINT_S));
        return Type::VOID;
    }

    Function &function = ft.get(name, current().getStart());

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
    return fn.getReturnType();
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
        var.setName(peek().getValue());
        var.setType(Type::STRING);
        var.setSize(peek().getValue().size());
        var.setKind(Kind::GLOBAL);
        auto &globals = vt.getGlobal();
        globals.insert(var, current().getStart());
        fn.addInstruction(Instruction(Operation::PUSH, var.getAddress()));
        next();
        return Type::INT;
    }
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

int token_to_int(TokenType ty)
{
    switch (ty)
    {
    case TokenType::PLUS:
        return 1;
    case TokenType::MINUS:
        return 2;
    case TokenType::MUL:
        return 3;
    case TokenType::DIV:
        return 4;
    case TokenType::GT:
        return 5;
    case TokenType::LT:
        return 6;
    case TokenType::GE:
        return 7;
    case TokenType::LE:
        return 8;
    case TokenType::EQ:
        return 9;
    case TokenType::NEQ:
        return 10;
    case TokenType::AS_KW:
        return 11;
    default:
        return 0;
    }
}

TokenType int_to_token(int id)
{
    switch (id)
    {
    case 1:
        return TokenType::PLUS;
    case 2:
        return TokenType::MINUS;
    case 3:
        return TokenType::MUL;
    case 4:
        return TokenType::DIV;
    case 5:
        return TokenType::GT;
    case 6:
        return TokenType::LT;
    case 7:
        return TokenType::GE;
    case 8:
        return TokenType::LE;
    case 9:
        return TokenType::EQ;
    case 10:
        return TokenType::NEQ;
    case 11:
        return TokenType::AS_KW;
    default:
        return TokenType::NONE;
    }
}