// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModels::noCombustion
// Description
//   Dummy combustion model for 'no combustion'
// SourceFiles
//   no_combustion.cpp
#ifndef no_combustion_hpp_
#define no_combustion_hpp_
namespace mousse
{
namespace combustionModels
{
template<class CombThermoType>
class noCombustion
:
  public CombThermoType
{
  //- Disallow copy construct
  noCombustion(const noCombustion&);
  //- Disallow default bitwise assignment
  void operator=(const noCombustion&);
public:
  //- Runtime type information
  TypeName("noCombustion");
  // Constructors
    //- Construct from components
    noCombustion
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
  //- Destructor
  virtual ~noCombustion();
  // Member Functions
    // Evolution
      //- Correct combustion rate
      virtual void correct();
      //- Fuel consumption rate matrix
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
#   include "no_combustion.cpp"
#endif
#endif
