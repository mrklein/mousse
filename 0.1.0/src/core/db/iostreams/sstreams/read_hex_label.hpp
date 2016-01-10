// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   read_hex_label.cpp
#ifndef read_hex_label_hpp_
#define read_hex_label_hpp_
#include "label.hpp"
#include "isstream.hpp"
namespace mousse
{
//- Read a hex label from an input stream
label readHexLabel(ISstream&);
}  // namespace mousse
#endif
