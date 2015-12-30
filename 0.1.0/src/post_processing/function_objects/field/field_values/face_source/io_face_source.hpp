// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_face_source_hpp_
#define io_face_source_hpp_
#include "face_source.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<faceSource> IOfaceSource;
}
#endif
