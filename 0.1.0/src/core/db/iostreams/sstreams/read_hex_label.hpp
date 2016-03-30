#ifndef CORE_DB_IOSTREAMS_SSTREAMS_READ_HEX_LABEL_HPP_
#define CORE_DB_IOSTREAMS_SSTREAMS_READ_HEX_LABEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label.hpp"
#include "isstream.hpp"

namespace mousse {

//- Read a hex label from an input stream
label readHexLabel(ISstream&);

}  // namespace mousse

#endif
