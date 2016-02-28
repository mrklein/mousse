// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::virtualMassModels::noVirtualMass
// Description
// SourceFiles
//   no_virtual_mass.cpp
#ifndef no_virtual_mass_hpp_
#define no_virtual_mass_hpp_
#include "virtual_mass_model.hpp"
namespace mousse
{
class phasePair;
namespace virtualMassModels
{
class noVirtualMass
:
  public virtualMassModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from a dictionary and a phase pair
    noVirtualMass
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~noVirtualMass();
  // Member Functions
    //- Virtual mass coefficient
    virtual tmp<volScalarField> Cvm() const;
    //- The virtual mass function K used in the momentum equation
    virtual tmp<volScalarField> K() const;
};
}  // namespace virtualMassModels
}  // namespace mousse
#endif
