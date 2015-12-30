// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModels::PaSR
// Description
//   Partially stirred reactor combustion model.  The model calculates a finite
//   rate, based on both turbulence and chemistry time scales.  Depending on
//   mesh resolution, the Cmix parameter can be used to scale the turbulence
//   mixing time scale.
// SourceFiles
//   pasr.cpp
#ifndef pasr_hpp_
#define pasr_hpp_
#include "laminar.hpp"
namespace mousse
{
namespace combustionModels
{
template<class Type>
class PaSR
:
  public laminar<Type>
{
  // Private data
    //- Mixing constant
    scalar Cmix_;
    //- Turbulent reaction switch
    Switch turbulentReaction_;
    //- Mixing parameter
    volScalarField kappa_;
  // Private Member Functions
    //- Disallow copy construct
    PaSR(const PaSR&);
    //- Disallow default bitwise assignment
    void operator=(const PaSR&);
public:
  //- Runtime type information
  TypeName("PaSR");
  // Constructors
    //- Construct from components
    PaSR(const word& modelType, const fvMesh& mesh, const word& phaseName);
  //- Destructor
  virtual ~PaSR();
  // Member Functions
    // Evolution
      //- Correct combustion rate
      virtual void correct();
      //- Fuel consumption rate matrix.
      virtual tmp<fvScalarMatrix> R(volScalarField& Y) const;
      //- Heat release rate calculated from fuel consumption rate matrix
      virtual tmp<volScalarField> dQ() const;
      //-  Return source for enthalpy equation [kg/m/s3]
      virtual tmp<volScalarField> Sh() const;
  // IO
      //- Update properties from given dictionary
      virtual bool read();
};
}  // namespace combustionModels
}  // namespace mousse
#ifdef NoRepository
#   include "pasr.cpp"
#endif
#endif
