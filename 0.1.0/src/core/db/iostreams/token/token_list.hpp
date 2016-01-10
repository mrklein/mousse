// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef token_list_hpp_
#define token_list_hpp_
#include "token.hpp"
#include "list.hpp"
namespace mousse
{
  typedef List<token> tokenList;
  typedef List<token::tokenType> tokenTypeList;
}  // namespace mousse
#endif
