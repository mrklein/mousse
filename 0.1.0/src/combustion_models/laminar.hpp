#ifndef COMBUSTION_MODELS_LAMINAR_HPP_
#define COMBUSTION_MODELS_LAMINAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModels::laminar
// Description
//   Laminar combustion model.
// SourceFiles
//   laminar.cpp
namespace mousse
{
namespace combustionModels
{
template<class Type>
class laminar
:
  public Type
{
  // Private data
    //- Integrate reaction rate over the time-step
    //  using the selected ODE solver
    bool integrateReactionRate_;
protected:
  // Protected Member Functions
    //- Return the chemical time scale
    tmp<volScalarField> tc() const;
public:
  //- Runtime type information
  TYPE_NAME("laminar");
  // Constructors
    //- Construct from components
    laminar
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
    //- Disallow copy construct
    laminar(const laminar&) = delete;
    //- Disallow default bitwise assignment
    laminar& operator=(const laminar&) = delete;
  //- Destructor
  virtual ~laminar();
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
#   include "laminar.cpp"
#endif
#endif
