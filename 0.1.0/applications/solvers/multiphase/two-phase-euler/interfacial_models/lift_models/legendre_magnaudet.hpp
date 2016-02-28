// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liftModels::LegendreMagnaudet
// Description
//   Lift model of Legendre and Magnaudet.
//   References:
//   \verbatim
//     "Implementation and Comparison of Correlations for interfacial Forces
//     in a Gas-Liquid System within an Euler-Euler Framework"
//     Otromke, M.
//     PhD Thesis
//     April 2013
//   \endverbatim
//   \verbatim
//     "The lift force on a spherical bubble in a viscous linear shear flow"
//     Legendre, D., Magnaudet, J.,
//     Journal of Fluid Mechanics
//     Volume 368, August 1998, pp. 81-126
//   \endverbatim
// SourceFiles
//   legendre_magnaudet.cpp
#ifndef LEGENDRE_MAGNAUDET_HPP_
#define LEGENDRE_MAGNAUDET_HPP_
#include "lift_model.hpp"
namespace mousse
{
class phasePair;
namespace liftModels
{
class LegendreMagnaudet
:
  public liftModel
{
  // Private data
    //- Residual Reynold's number
    const dimensionedScalar residualRe_;
public:
  //- Runtime type information
  TYPE_NAME("LegendreMagnaudet");
  // Constructors
    //- Construct from a dictionary and a phase pair
    LegendreMagnaudet
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~LegendreMagnaudet();
  // Member Functions
    //- Lift coefficient
    virtual tmp<volScalarField> Cl() const;
};
}  // namespace liftModels
}  // namespace mousse
#endif
