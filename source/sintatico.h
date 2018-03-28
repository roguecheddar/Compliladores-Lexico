Token currentToken;
int currentIndex = 0;

int erro()
{
  cout << "ERROR on line " << currentToken.line << endl;
  return 0;
}

void getSymbol()
{
    currentToken = tokenList[currentIndex];

    ++currentIndex;
}

int opMultiplicativo()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Multiplicativo")
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int opAditivo()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Aditivo")
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int opRelacional()
{
    getSymbol();
    if(currentToken.TokenType == "Op. Relacional")
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int sinal()
{
    getSymbol();
    if( currentToken.symbol == "+"  ||
        currentToken.symbol == "-")
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int fator()
{
    getSymbol();
    if( currentToken.TokenType == "Inteiro" ||
        currentToken.TokenType == "Real" ||
        currentToken.symbol == "true" ||
        currentToken.symbol == "false"
      )
    {
        return 1;
    }
    else if (currentToken.TokenType == "Identificador")
    {
        getSymbol();

        if (currentToken.symbol == "(")
        {
            if(listaDeExpressoes())
            {
                getSymbol();

                if (currentToken.symbol == ")")
                {
                return 1;
                }
                else
                {
                    return erro();
                }
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return 1;
        }
    }
    else if (currentToken.symbol == "not")
    {
        return fator();
    }
    else if (currentToken.symbol == "(")
    {
        if (expressao())
        {
            getSymbol();

            if (currentToken.symbol == ")")
            {
                return 1;
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        return erro();
    }
}

int termo_()
{
    if(opMultiplicativo())
    {
        if(fator())
        {
            if(termo_())
            {
                return 1;
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int termo()
{
    return (fator() && termo_());
}

int expressaoSimples_()
{
    if(opAditivo())
    {
        if(termo())
        {
            if(expressaoSimples_())
            {
                return 1;
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int expressaoSimples()
{
    if(termo() && expressaoSimples_())
    {
        return 1;
    }
    else if(sinal() && termo() && expressaoSimples_())
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int expressao()
{
    if(expressao())
    {
        return 1;
    }
    else if(expressaoSimples() && opRelacional() && expressaoSimples())
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int listaDeExpressoes_()
{
    getSymbol();
    if(currentToken.symbol == ",")
    {
        if(expressao())
        {
            return listaDeExpressoes_();
        }
        else
        {
            return erro();
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int listaDeExpressoes()
{
    return (expressao() && listaDeExpressoes_());
}

int ativacaoDeProcedimento()
{
    getSymbol();
    if(currentToken.TokenType == "Identificador")
    {
        getSymbol();
        if(currentToken.symbol == "(")
        {
            if(listaDeExpressoes())
            {                   
                getSymbol();
                if(currentToken.symbol == ")")
                {
                    return 1;
                }
                else
                {
                    currentIndex = currentIndex - 2;                 //desfazendo os 2 get's
                    return erro();
                }
            }
            else
            {
                --currentIndex;
                return erro();
            }
        }
        else
        {
            --currentIndex;
            return erro();
        }
    }
    else
    {
        --currentIndex;
        return erro();
    }
}

int variavel()
{
    getSymbol();
    if(currentToken.TokenType == "Identificador")
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int parteElse()
{
    getSymbol();
    if(currentToken.symbol == "else")
    {
        return comando();
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int comando()
{
    if(variavel())
    {
        getSymbol();
        if(currentToken.TokenType == "Atribuição")
        {
            return expressao();
        }
    }
    else if (ativacaoDeProcedimento()) {
        return 1;
    }
    else if (comandoComposto())
    {
        return 1;
    }
    else
    {
      getSymbol();
      if(currentToken.symbol == "if")
      {
          if(expressao())
          {
              getSymbol();
              if(currentToken.symbol == "then")
              {
                  return comando() && parteElse();
              }
              else
              {
                  return erro();
              }
          }
          else if (currentToken.symbol == "while")
          {
              if(expressao())
              {
                  getSymbol();
                  if(currentToken.symbol == "do")
                  {
                      return comando();
                  }
                  else
                  {
                      return erro();
                  }
              }
              else
              {
                  return erro();
              }
          }
          else
          {
              return erro();
          }
      }
        return erro();
    }
}

int listaDeComandos_()
{
    getSymbol();
    if(currentToken.symbol == ";")
    {
        return comando() && listaDeComandos_();
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int listaDeComandos()
{
    return comando() && listaDeComandos_();
}

int comandosOpcionais()
{
    if(listaDeComandos())
    {
        return 1;
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int comandoComposto()
{
    getSymbol();
    if(currentToken.symbol == "begin")
    {
        if(comandosOpcionais())
        {
            getSymbol();
            if(currentToken.symbol == "end")
            {
                return 1;
            }
            else
            {
                return erro();
            }
        }
        else
        {
            erro();
        }
    }
    else
    {
        return erro();
    }
}

int listaDeParametros_()
{
    getSymbol();
    if(currentToken.symbol == ";")
    {
        if(listaDeIdentificadores())
        {
            getSymbol();
            if(currentToken.symbol == ":")
            {
                if(tipo())
                {
                    return listaDeParametros_();
                }
                else
                {
                    return erro();
                }
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int listaDeParametros()
{
    if(listaDeIdentificadores())
    {
        getSymbol();
        if(currentToken.symbol == ":")
        {
            if(tipo())
            {
                return listaDeParametros_();
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        return erro();
    }
}

int argumentos()
{
    getSymbol();
    if(currentToken.symbol == "(")
    {
        if(listaDeParametros())
        {
            getSymbol();
            if(currentToken.symbol == ")")
            {
                return 1;
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int declaracaoDeSubprograma()
{
    getSymbol();
    if(currentToken.symbol == "procedure")
    {
        getSymbol();
        if(currentToken.TokenType == "Identificador")
        {
            if(argumentos())
            {
                getSymbol();
                if(currentToken.symbol == ";")
                {
                    if(declaracoesVariaveis())
                    {
                        if(declaracoesDeSubprogramas())
                        {
                            if(comandoComposto())
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return erro();
                        }
                    }
                    else
                    {
                        return erro();
                    }
                }
                else
                {
                    return erro();
                }
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        return erro();
    }
}

int declaracoesDeSubprogramas_()
{
    if(declaracaoDeSubprograma())
    {
        getSymbol();
        if(currentToken.symbol == ";")
        {
            return declaracoesDeSubprogramas_();
        }
        else
        {
            return erro();
        }
    }
    else
    {
        return 1;
    }
}

int declaracoesDeSubprogramas()
{
    return declaracoesDeSubprogramas_();
}

int tipo()
{
    getSymbol();
    if( currentToken.TokenType == "integer" ||
        currentToken.TokenType == "real" ||
        currentToken.TokenType == "boolean")
    {
        return 1;
    }
    else
    {
        return erro();
    }
}

int listaDeIdentificadores_()
{
    getSymbol();
    if(currentToken.symbol == ",")
    {
        getSymbol();
        if(currentToken.TokenType == "Identificador")
        {
            return listaDeIdentificadores_();
        }
        else
        {
            return erro();
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int listaDeIdentificadores()
{
    getSymbol();
    if(currentToken.TokenType == "Identificador")
    {
        return listaDeIdentificadores_();
    }
    else
    {
        return erro();
    }
}

int listaDeclaracaoVariaveis_()
{
    if(listaDeIdentificadores())
    {
        getSymbol();
        if(currentToken.symbol == ":")
        {
            if(tipo())
            {
                getSymbol();
                if(currentToken.symbol == ";")
                {
                    return listaDeIdentificadores_();
                }
                else
                {
                    return erro();
                }
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        --currentIndex;
        return 1;
    }
}

int listaDeclaracaoVariaveis()
{
    if(listaDeIdentificadores())
    {
        getSymbol();
        if(currentToken.symbol == ":")
        {
            if(tipo())
            {
                getSymbol();
                if(currentToken.symbol == ";")
                {
                    return listaDeclaracaoVariaveis_();
                }
                else
                {
                    return erro();
                }
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        return erro();
    }
}

int declaracoesVariaveis()
{
    getSymbol();
    if(currentToken.symbol == "var")
    {
        return listaDeclaracaoVariaveis();
    }
    else
    {
        return 1;
    }
}

int programa()
{
    getSymbol();
    if( currentToken.symbol == "program")
    {
        getSymbol();
        if(currentToken.TokenType == "Identificador")
        {
            getSymbol();
            if(currentToken.symbol == ";")
            {
                if (declaracoesVariaveis())
                {
                    if (declaracoesDeSubprogramas())
                    {
                        if (comandoComposto())
                        {
                            getSymbol();

                            if(currentToken.symbol == ".")
                            {
                                return 1;
                            }
                            else
                            {
                                return erro();
                            }
                        }
                        else
                        {
                            return erro();
                        }
                    }
                    else
                    {
                        return erro();
                    }
                }
                else
                {
                    return erro();
                }
            }
            else
            {
                return erro();
            }
        }
        else
        {
            return erro();
        }
    }
    else
    {
        return erro();
    }
}
