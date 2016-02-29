// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiEqModels::Gulder
// Description
//   Simple Gulder model for XiEq based on Gulders correlation
//   with a linear correction function to give a plausible profile for XiEq.
// SourceFiles
//   gulder.cpp
#ifndef GULDER_HPP_
#define GULDER_HPP_
#include "xi_eq_model.hpp"
namespace mousse
{
namespace XiEqModels
{
class Gulder
:
  public XiEqModel
{
  // Private data
    //- Model constant
    scalar XiEqCoef_;
    //- Minimum laminar burning velocity
    const dimensionedScalar SuMin_;
    //- Schelkin effect Model constant
    scalar uPrimeCoef_;
    //- Use sub-grid Schelkin effect
    bool subGridSchelkin_;
public:
  //- Runtime type information
  TYPE_NAME("Gulder");
  // Constructors
    //- Construct from components
    Gulder
    (
      const dictionary& XiEqProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
    //- Disallow copy construct
    Gulder(const Gulder&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const Gulder&) = delete;
  //- Destructor
  virtual ~Gulder();
  // Member Functions
    //- Return the flame-wrinking XiEq
    virtual tmp<volScalarField> XiEq() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiEqProperties);
};
}  // namespace XiEqModels
}  // namespace mousse
#endif
