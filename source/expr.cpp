#include "analyser.h"

/*
 * 表达式分析函数
 * 算符优先表
 * +-----------------------------------------+
 * +|     | ( | ) | + | - | * | / | > | < | >= | <= | != | == | 前置- |函数调用| as | 
 * +|------------------------------------------------------------------------------+
 * +| (   | <.| =.| <.| <.|
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

Type Analyser::expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    
}
Type Analyser::operator_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{

}
Type Analyser::negate_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::MINUS);
    auto type = expr(vt,ft,fn);
    if(type == Type::INT)
    fn.addInstruction(Instruction(Operation::NEG_I));
    else if(type == Type::DOUBLE)
        fn.addInstruction(Instruction(Operation::NEG_F));
    return type;
}
Type Analyser::assign_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    auto & name = expect(TokenType::IDENT).getValue();
    Varible l_expr = vt.get(name,current().getStart());
    if(l_expr.isConst()) throw Error(ErrorCode::ConstValueChanged,"you can\'t assign to a const varible!",current().getStart());
    expect(TokenType::ASSIGN);
    if(l_expr.getKind() == Kind::GLOBAL) fn.addInstruction(Instruction(Operation::GLOBA,l_expr.getAddress()));
    else if(l_expr.getKind() == Kind::PARAM) fn.addInstruction(Instruction(Operation::ARGA,l_expr.getAddress()));
    else if(l_expr.getKind() == Kind::VAR) fn.addInstruction(Instruction(Operation::LOCA,l_expr.getAddress()));
    else throw Error(ErrorCode::InvalidAssign,"invalid assign",current().getStart());
    auto type = expr(vt,ft,fn);
    if(type != l_expr.getType()) throw Error(ErrorCode::InvalidAssign,"invalid assign",current().getStart());
    fn.addInstruction(Instruction(Operation::STORE_64));
    return type;
}
Type Analyser::as_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
}
Type Analyser::call_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    auto & name = expect(TokenType::IDENT).getValue();
    Function & function = ft.get(name,current().getStart());
    // 准备参数，并检查参数列表和函数的声明是否匹配，包括参数个数和类型
    fn.addInstruction(Instruction(Operation::CALL,function.getFid()));
    fn.addInstruction(Instruction(Operation::RET));
    return fn.getReturnType();
}
Type Analyser::literal_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    auto t = peek().getTokenType();
    if(t == TokenType::UINT_LITERAL)
    {
        unsigned long long value = std::stoull(peek().getValue());
        fn.addInstruction(Instruction(Operation::PUSH,value));
        return Type::INT;
    }
    else if(t == TokenType::DOUBLE_LITERAL)
    {
        return Type::DOUBLE;
    }
    else if(t == TokenType::CHAR_LITERAL)
    {
        return Type::INT;
    }
    else if(t == TokenType::STRING_LITERAL)
    {
        Varible var;
        var.setName(peek().getValue());
        var.setType(Type::STRING);
        var.setSize(peek().getValue().size());
        var.setKind(Kind::GLOBAL);
        auto & globals = vt.getGlobal();
        globals.insert(var,current().getStart());
        fn.addInstruction(Instruction(Operation::PUSH,var.getAddress()));
        return Type::INT;
    }
}
Type Analyser::ident_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    auto & name = expect(TokenType::IDENT).getValue();
    auto & var = vt.get(name,current().getStart());
    if(var.getKind() == Kind::GLOBAL) fn.addInstruction(Instruction(Operation::GLOBA,var.getAddress()));
    else if(var.getKind() == Kind::PARAM) fn.addInstruction(Instruction(Operation::ARGA,var.getAddress()));
    else if(var.getKind() == Kind::VAR) fn.addInstruction(Instruction(Operation::LOCA,var.getAddress()));
    fn.addInstruction(Instruction(Operation::LOAD_64));
    return var.getType();
}
Type Analyser::group_expr(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::L_PAREN);
    auto ret = expr(vt,ft,fn);
    expect(TokenType::R_PAREN);
    return ret;
}
