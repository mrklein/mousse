#ifndef SAMPLING_WRITE_PATCH_GRAPH_HPP_
#define SAMPLING_WRITE_PATCH_GRAPH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields_fwd.hpp"


namespace mousse {

void writePatchGraph
(
  const volScalarField& vsf,
  const label patchLabel,
  const direction d,
  const word& graphFormat
);

}  // namespace mousse

#endif

