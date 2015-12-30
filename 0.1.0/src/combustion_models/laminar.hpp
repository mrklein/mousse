// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModels::laminar
// Description
//   Laminar combustion model.
// SourceFiles
//   laminar.cpp
#ifndef laminar_hpp_
#define laminar_hpp_
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
private:
  // Private Member Functions
    //- Disallow copy construct
    laminar(const laminar&);
    //- Disallow default bitwise assignment
    void operator=(const laminar&);
public:
  //- Runtime type information
  TypeName("laminar");
  // Constructors
    //- Construct from components
    laminar
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
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
