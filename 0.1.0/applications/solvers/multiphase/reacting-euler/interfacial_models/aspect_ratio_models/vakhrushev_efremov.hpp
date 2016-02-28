// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::VakhrushevEfremov
// Description
//   Aspect ratio model of Vakhrushev and Efremov.
//   Reference:
//   \verbatim
//     "Interpolation formula for computing the velocities of single gas
//     bubbles in liquids"
//     Vakhrushev, I.A. and Efremov, G.I.,
//     Chemistry and Technology of Fuels and Oils
//     Volume 6, Issue 5, May 1970, pp. 376-379,
//   \endverbatim
// SourceFiles
//   vakhrushev_efremov.cpp
#ifndef vakhrushev_efremov_hpp_
#define vakhrushev_efremov_hpp_
#include "aspect_ratio_model.hpp"
namespace mousse
{
namespace aspectRatioModels
{
class VakhrushevEfremov
:
  public aspectRatioModel
{
public:
  //- Runtime type information
  TYPE_NAME("VakhrushevEfremov");
  // Constructors
    //- Construct from a dictionary and an ordered phase pair
    VakhrushevEfremov
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~VakhrushevEfremov();
  // Member Functions
    //- Aspect ratio
    virtual tmp<volScalarField> E() const;
};
}  // namespace aspectRatioModels
}  // namespace mousse
#endif
