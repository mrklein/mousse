// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef primitive_face_zone_hpp_
#define primitive_face_zone_hpp_
#include "face.hpp"
#include "primitive_patch.hpp"
#include "indirect_face_list.hpp"
#include "point_field.hpp"
namespace mousse
{
  typedef PrimitivePatch<face, IndirectList, const pointField&>
    primitiveFaceZone;
}
#endif
