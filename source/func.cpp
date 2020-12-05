#include "analyser.h"

void Analyser::func(VaribleTable &parent, FunctionTable &ft)
{
    Function fn;
    VaribleTable vt(parent);

    expect(TokenType::FN_KW);
    auto & name = expect(TokenType::IDENT).getValue();
    if(ft.isDeclared(name)) throw Error(ErrorCode::DuplicateDecl,"function \""+name+"\" is duplicate declared!");
    expect(TokenType::L_PAREN);
    std::vector<Varible> params;
    if (peek().getTokenType() != TokenType::R_PAREN)
        params = func_param_list();
    expect(TokenType::R_PAREN);
    expect(TokenType::ARROW);
    auto & type = expect(TokenType::TY).getValue();

    if(type == "void")
    {
        fn.setReturnType(Type::VOID);
        fn.setReturnSlots(0);
        for(int i=0;i<params.size();i++)
            params[i].setAddress(i);
    }
    else if(type == "int")
    {
        fn.setReturnType(Type::INT);
        fn.setReturnSlots(1);
        for(int i=0;i<params.size();i++)
            params[i].setAddress(i+1);
    }
    else if(type == "double")
    {
        fn.setReturnType(Type::DOUBLE);
        fn.setReturnSlots(1);
        for(int i=0;i<params.size();i++)
            params[i].setAddress(i+1);
    }
    else throw Error(ErrorCode::InvalidType,"function \""+name+"\" return a invalid type!",current().getStart());
    // fn.setParamSlots(params.size());
    for(auto & param : params) {vt.insert(param,current().getStart()); fn.addParamType(param.getType());}
    block_stmt(vt,ft,fn);
    ft.insert(fn,current().getStart());
}
Varible Analyser::func_param()
{
    Varible ret;
    ret.setConst(false);
    ret.setKind(Kind::PARAM);
    ret.setSize(8);
    if(peek().getTokenType() == TokenType::CONST_KW)
    {
        ret.setConst(true);
        next();
    }
    auto & name = expect(TokenType::IDENT).getValue();
    ret.setName(name);
    expect(TokenType::COLON);
    auto & type = expect(TokenType::TY).getValue();
    if(type == "void") throw Error(ErrorCode::InvalidType,"param can\'t be void",current().getStart());
    else if(type == "int") ret.setType(Type::INT);
    else if(type == "double") ret.setType(Type::DOUBLE);
    return ret;
}
std::vector<Varible> Analyser::func_param_list()
{
    std::vector<Varible> ret;
    ret.push_back(func_param());
    while(peek().getTokenType() == TokenType::COMMA)
    {
        next();
        ret.push_back(func_param());
    }
}