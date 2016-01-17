// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef face_io_list_hpp_
#define face_io_list_hpp_
#include "face.hpp"
#include "compact_io_list.hpp"
namespace mousse
{
  typedef IOList<face> faceIOList;
  typedef CompactIOList<face, label> faceCompactIOList;
}
#endif
