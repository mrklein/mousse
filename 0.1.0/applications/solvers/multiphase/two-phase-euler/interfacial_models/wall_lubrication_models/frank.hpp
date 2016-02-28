// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallLubricationModels::Frank
// Description
//   Wall lubrication model of Frank.
//   References:
//   \verbatim
//     "Implementation and Comparison of Correlations for interfacial Forces
//     in a Gas-Liquid System within an Euler-Euler Framework"
//     Otromke, M.
//     PhD Thesis
//     April 2013
//   \endverbatim
//   \verbatim
//     "Advances in Computational Fluid Dynamics (CFD) of 3-dimensional Gas-
//     Liquid Multiphase Flows"
//     Frank, T.
//     NAFEMS Seminar: Simulation of Complex Flows (CFD), April 2005, pp. 1-18
//   \endverbatim
// SourceFiles
//   frank.cpp
#ifndef FRANK_HPP_
#define FRANK_HPP_
#include "wall_lubrication_model.hpp"
namespace mousse
{
class phasePair;
namespace wallLubricationModels
{
class Frank
:
  public wallLubricationModel
{
  // Private data
    //- Coefficient d
    const dimensionedScalar Cwd_;
    //- Coefficient c
    const dimensionedScalar Cwc_;
    //- Power p
    const scalar p_;
public:
  //- Runtime type information
  TYPE_NAME("Frank");
  // Constructors
    //- Construct from components
    Frank
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Frank();
  // Member Functions
    //- Return phase-intensive wall lubrication force
    tmp<volVectorField> Fi() const;
};
}  // namespace wallLubricationModels
}  // namespace mousse
#endif
