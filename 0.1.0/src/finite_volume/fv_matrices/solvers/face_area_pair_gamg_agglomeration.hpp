#ifndef FINITE_VOLUME_FV_MATRICES_SOLVERS_FACE_AREA_PAIR_GAMG_AGGLOMERATION_HPP_
#define FINITE_VOLUME_FV_MATRICES_SOLVERS_FACE_AREA_PAIR_GAMG_AGGLOMERATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceAreaPairGAMGAgglomeration
// Description
//   Agglomerate using the pair algorithm.
// SourceFiles
//   face_area_pair_gamg_agglomeration.cpp
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
