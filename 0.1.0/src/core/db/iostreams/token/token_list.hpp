#ifndef CORE_DB_IOSTREAMS_TOKEN_TOKEN_LIST_HPP_
#define CORE_DB_IOSTREAMS_TOKEN_TOKEN_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "token.hpp"
#include "list.hpp"


namespace mousse {

typedef List<token> tokenList;
typedef List<token::tokenType> tokenTypeList;

}  // namespace mousse

#endif
