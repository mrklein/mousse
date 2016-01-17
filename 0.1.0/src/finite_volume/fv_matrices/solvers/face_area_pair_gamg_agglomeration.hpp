// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceAreaPairGAMGAgglomeration
// Description
//   Agglomerate using the pair algorithm.
// SourceFiles
//   face_area_pair_gamg_agglomeration.cpp
#ifndef face_area_pair_gamg_agglomeration_hpp_
#define face_area_pair_gamg_agglomeration_hpp_
#include "pair_gamg_agglomeration.hpp"
namespace mousse
{
class faceAreaPairGAMGAgglomeration
:
  public pairGAMGAgglomeration
{
public:
  //- Runtime type information
  TYPE_NAME("faceAreaPair");
  // Constructors
    //- Construct given mesh and controls
    faceAreaPairGAMGAgglomeration
    (
      const lduMesh& mesh,
      const dictionary& controlDict
    );
    //- Construct given mesh and controls
    faceAreaPairGAMGAgglomeration
    (
      const lduMesh& mesh,
      const scalarField& cellVolumes,
      const vectorField& faceAreas,
      const dictionary& controlDict
    );
};
}  // namespace mousse
#endif
