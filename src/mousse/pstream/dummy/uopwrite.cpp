// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uopstream.hpp"
#include "iostreams.hpp"


// Member Functions 
bool mousse::UOPstream::write
(
  const commsTypes /*commsType*/,
  const int /*toProcNo*/,
  const char* /*buf*/,
  const std::streamsize /*bufSize*/,
  const int /*tag*/,
  const label /*communicator*/
)
{
  NOT_IMPLEMENTED
  (
    "UOPstream::write"
    "("
      "const commsTypes commsType,"
      "const int fromProcNo,"
      "char* buf,"
      "const label bufSize,"
      "const int tag,"
      "const label communicator"
    ")"
  );
  return false;
}

