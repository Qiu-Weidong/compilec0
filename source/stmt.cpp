#include "analyser.h"

void Analyser::stmt(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    auto type = peek().getTokenType();
    if(type == TokenType::LET_KW || type == TokenType::CONST_KW)
        decl_stmt(vt,ft,fn);
    else if(type == TokenType::IF_KW)
        if_stmt(vt,ft,fn);
    else if(type == TokenType::WHILE_KW)
        while_stmt(vt,ft,fn);
    else if(type == TokenType::BREAK_KW)
        break_stmt(vt,ft,fn);
    else if(type == TokenType::CONTINUE_KW)
        continue_stmt(vt,ft,fn);
    else if(type == TokenType::RETURN_KW)
        return_stmt(vt,ft,fn);
    else if(type == TokenType::L_BRACE)
    {
        VaribleTable new_vt(vt);
        block_stmt(new_vt,ft,fn);
    }
    else if(type == TokenType::SEMICOLON)
        empty_stmt();
    else if(isExpressionTermination(type))
        expr(vt,ft,fn);
    else throw Error(ErrorCode::invalidStmt,"invalid stmt",current().getStart());
}
void Analyser::block_stmt(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::L_BRACE);
    while (has_next() && peek().getTokenType() != TokenType::R_BRACE)
        stmt(vt,ft,fn);
    expect(TokenType::R_BRACE);
}
void Analyser::expr_stmt(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expr(vt,ft,fn);
    expect(TokenType::SEMICOLON);
}
void Analyser::if_stmt(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::IF_KW);
    expr(vt,ft,fn);
    VaribleTable new_vt(vt);
    block_stmt(new_vt,ft,fn);
    new_vt.clear();
    while (peek().getTokenType() == TokenType::ELSE_KW)
    {
        next();
        if (peek().getTokenType() == TokenType::IF_KW)
        {
            next();
            expr(vt,ft,fn);
            block_stmt(new_vt,ft,fn);
            new_vt.clear();
        }
        else
        {
            block_stmt(new_vt,ft,fn);
            new_vt.clear();
            break;
        }
    }
}
void Analyser::while_stmt(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::WHILE_KW);
    expr(vt,ft,fn);
    VaribleTable new_vt(vt);
    block_stmt(new_vt,ft,fn);
}
void Analyser::break_stmt(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::BREAK_KW);
    expect(TokenType::SEMICOLON);
}
void Analyser::continue_stmt(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::CONTINUE_KW);
    expect(TokenType::SEMICOLON);
}
void Analyser::return_stmt(VaribleTable &vt, FunctionTable &ft, Function &fn)
{
    expect(TokenType::RETURN_KW);
    if(expr(vt,ft,fn) != fn.getReturnType()) throw Error(ErrorCode::TypeNotMatch,"function return error type!",current().getStart());
    expect(TokenType::SEMICOLON);
}
void Analyser::empty_stmt()// 可以不要
{
    expect(TokenType::SEMICOLON);
}
void Analyser::decl_stmt(VaribleTable &vt,FunctionTable & ft, Function &fn)
{
    Varible var;
    var.setSize(8);
    if(peek().getTokenType() == TokenType::LET_KW)
        var.setConst(false);
    else if(peek().getTokenType() == TokenType::CONST_KW)
        var.setConst(true);
    else throw Error(ErrorCode::InvalidDeclare,"invalid declare!",current().getStart());
    next();
    var.setName(expect(TokenType::IDENT).getValue());
    expect(TokenType::COLON);
    auto type = expect(TokenType::TY).getValue();
    if(type == "void") throw Error(ErrorCode::InvalidDeclare, "variant can\'t be \'void\'!", current().getStart());
    else if(type == "int") var.setType(Type::INT);
    else if(type == "double") var.setType(Type::DOUBLE);
    else throw Error(ErrorCode::InvalidType,"No such type",current().getStart());
    if(vt.isGlobalTable()) var.setKind(Kind::GLOBAL);
    else var.setKind(Kind::VAR);
    var.setAddress(fn.nextLoca());
    vt.insert(var,current().getStart());
    if(var.isConst() && peek().getTokenType() != TokenType::ASSIGN)
        throw Error(ErrorCode::ConstMustBeInitial,"const varible is not initial!",current().getStart());
    if(peek().getTokenType() == TokenType::ASSIGN)
    {
        next();
        if(var.getKind() == Kind::GLOBAL)
        fn.addInstruction(Instruction(Operation::GLOBA,var.getAddress()));
        else if(var.getKind() == Kind::VAR)
        fn.addInstruction(Instruction(Operation::LOCA,var.getAddress()));
        if(expr(vt,ft,fn)!=var.getType()) throw Error(ErrorCode::TypeNotMatch,"type is not match!",current().getStart());
        fn.addInstruction(Instruction(Operation::STORE_64));
    }
    expect(TokenType::SEMICOLON);
}