// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liftModels::Moraga
// Description
//   Lift model of Moraga et al.
//   References:
//   \verbatim
//     "Implementation and Comparison of Correlations for interfacial Forces
//     in a Gas-Liquid System within an Euler-Euler Framework"
//     Otromke, M.,
//     PhD Thesis, April 2013
//   \endverbatim
//   \verbatim
//     "Lateral forces on spheres in turbulent uniform shear flow"
//     Moraga, F.J., Bonetto, F.J., Lahey, R.T.,
//     International Journal of Multiphase Flow
//     Volume 25, Issues 6-7, September 1999, pp. 1321-1372
//   \endverbatim
// SourceFiles
//   moraga.cpp
#ifndef MORAGA_HPP_
#define MORAGA_HPP_
#include "lift_model.hpp"
namespace mousse
{
class phasePair;
namespace liftModels
{
class Moraga
:
  public liftModel
{
public:
  //- Runtime type information
  TYPE_NAME("Moraga");
  // Constructors
    //- Construct from a dictionary and a phase pair
    Moraga
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Moraga();
  // Member Functions
    //- Lift coefficient
    virtual tmp<volScalarField> Cl() const;
};
}  // namespace liftModels
}  // namespace mousse
#endif
