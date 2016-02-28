// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallLubricationModels::Antal
// Description
//   Wall lubrication model of Antal et al.
//   Reference:
//   \verbatim
//     "Analysis of phase distribution in fully developed laminar bubbly
//     two-phase flow"
//     Antal, S.P., Lahey Jr, R.T., and Flaherty, J.E.
//     International Journal of Multiphase Flow
//     Volume 17, Issue 5, September 1991, pp. 635-652
//   \endverbatim
// SourceFiles
//   antal.cpp
#ifndef antal_hpp_
#define antal_hpp_
#include "wall_lubrication_model.hpp"
namespace mousse
{
class phasePair;
namespace wallLubricationModels
{
class Antal
:
  public wallLubricationModel
{
  // Private data
    //- Coefficient 1
    const dimensionedScalar Cw1_;
    //- Coefficient 2
    const dimensionedScalar Cw2_;
public:
  //- Runtime type information
  TYPE_NAME("Antal");
  // Constructors
    //- Construct from components
    Antal
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Antal();
  // Member Functions
    //- Return phase-intensive wall lubrication force
    tmp<volVectorField> Fi() const;
};
}  // namespace wallLubricationModels
}  // namespace mousse
#endif
