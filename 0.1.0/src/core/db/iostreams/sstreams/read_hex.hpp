#ifndef CORE_DB_IOSTREAMS_SSTREAMS_READ_HEX_HPP_
#define CORE_DB_IOSTREAMS_SSTREAMS_READ_HEX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   read_hex.cpp
#include "isstream.hpp"
namespace mousse
{
//- Read a hex label from an input stream
template<class T>
T ReadHex(ISstream&);
}  // namespace mousse
#ifdef NoRepository
#   include "read_hex.cpp"
#endif
#endif
