// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiEqModels::SCOPEXiEq
// Description
//   Simple SCOPEXiEq model for XiEq based on SCOPEXiEqs correlation
//   with a linear correction function to give a plausible profile for XiEq.
//   See \link SCOPELaminarFlameSpeed.H \endlink for details on the SCOPE laminar
//   flame speed model.
// SourceFiles
//   scope_xi_eq.cpp
#ifndef SCOPE_XI_EQ_HPP_
#define SCOPE_XI_EQ_HPP_
#include "xi_eq_model.hpp"
#include "scope_laminar_flame_speed.hpp"
namespace mousse
{
namespace XiEqModels
{
class SCOPEXiEq
:
  public XiEqModel
{
  // Private data
    // Model constant
    scalar XiEqCoef_;
    // Model constant
    scalar XiEqExp_;
    // Model constant
    scalar lCoef_;
    //- Minimum Su
    dimensionedScalar SuMin_;
    //- Schelkin effect Model constant
    scalar uPrimeCoef_;
    //- Use sub-grid Schelkin effect
    bool subGridSchelkin_;
    //- The SCOPE laminar flame speed model used to obtain the
    //  Marstein number.  Note: the laminar flame speed need not be
    //  obtained form the same model.
    laminarFlameSpeedModels::SCOPE MaModel;
public:
  //- Runtime type information
  TYPE_NAME("SCOPEXiEq");
  // Constructors
    //- Construct from components
    SCOPEXiEq
    (
      const dictionary& XiEqProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
    //- Disallow copy construct
    SCOPEXiEq(const SCOPEXiEq&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const SCOPEXiEq&) = delete;
  //- Destructor
  virtual ~SCOPEXiEq();
  // Member Functions
    //- Return the flame-wrinking XiEq
    virtual tmp<volScalarField> XiEq() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiEqProperties);
};
}  // namespace XiEqModels
}  // namespace mousse
#endif
