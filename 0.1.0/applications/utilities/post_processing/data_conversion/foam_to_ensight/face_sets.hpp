#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_FACE_SETS_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_FACE_SETS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceSets
// Description
#include "label_list.hpp"
namespace mousse
{
class faceSets
{
public:
    label nTris;
    label nQuads;
    label nPolys;
    labelList tris;
    labelList quads;
    labelList polys;
  // Constructors
    //- Construct null
    faceSets()
    :
      nTris(0),
      nQuads(0),
      nPolys(0)
    {}
};
}  // namespace mousse
#endif
