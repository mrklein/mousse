#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_WALL_LUBRICATION_MODELS_TOMIYAMA_WALL_LUBRICATION_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_WALL_LUBRICATION_MODELS_TOMIYAMA_WALL_LUBRICATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallLubricationModels::TomiyamaWallLubrication
// Description
//   Wall lubrication model of Tomiyama.
//   References:
//   \verbatim
//     "Implementation and Comparison of Correlations for interfacial Forces
//     in a Gas-Liquid System within an Euler-Euler Framework"
//     Otromke, M.
//     PhD Thesis, April 2013
//   \endverbatim
//   \verbatim
//     "Struggle with Computational Bubble Dynamics"
//     Tomiyama, A.,
//     Multiphase Science and Technology
//     Volume 10, Issue 4, 1998, pp. 369-405
//   \endverbatim
// SourceFiles
//   tomiyama_wall_lubrication.cpp
#include "wall_lubrication_model.hpp"
namespace mousse
{
class phasePair;
namespace wallLubricationModels
{
class TomiyamaWallLubrication
:
  public wallLubricationModel
{
  // Private data
    //- Characteristic channel dimension
    const dimensionedScalar D_;
public:
  //- Runtime type information
  TYPE_NAME("Tomiyama");
  // Constructors
    //- Construct from components
    TomiyamaWallLubrication
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~TomiyamaWallLubrication();
  // Member Functions
    //- Return phase-intensive wall lubrication force
    tmp<volVectorField> Fi() const;
};
}  // namespace wallLubricationModels
}  // namespace mousse
#endif
