#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_DRAG_MODELS_LAIN_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_DRAG_MODELS_LAIN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::Lain
// Description
//   Drag model of Lain et al.
//   References:
//   \verbatim
//     "Implementation and Comparison of Correlations for interfacial Forces
//     in a Gas-Liquid System within an Euler-Euler Framework"
//     Otromke, M.,
//     PhD Thesis, April 2013
//   \endverbatim
//   \verbatim
//     "Modelling hydrodynamics and turbulence in a bubble column using the
//     Euler-Lagrange procedure"
//     Lain, S., Brodera, D., Sommerfelda, M., Goza, M.F.,
//     International Journal of Multiphase Flow
//     Volume 28, Issue 8, August 2002, pp. 1381-1407
//   \endverbatim
// SourceFiles
//   lain.cpp
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class Lain
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("Lain");
  // Constructors
    //- Construct from a dictionary and a phase pair
    Lain
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~Lain();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
