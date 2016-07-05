#ifndef INSTABILITY_G_HPP_
#define INSTABILITY_G_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiGModels::instabilityG
// Description
//   Flame-surface instabilityG flame-wrinking generation rate coefficient model
//   used in \link XiModel.H \endlink.
//   See Technical Report SH/RE/01R for details on the PDR modelling.

#include "xi_g_model.hpp"


namespace mousse {
namespace XiGModels {

class instabilityG
:
  public XiGModel
{
  // Private data
    //- Flame instabilityG wrinling generation rate coefficient
    dimensionedScalar GIn_;
    //- InstabilityG length-scale
    dimensionedScalar lambdaIn_;
    //- Xi generation rate model due to all other processes
    autoPtr<XiGModel> XiGModel_;
public:
  //- Runtime type information
  TYPE_NAME("instabilityG");
  // Constructors
    //- Construct from components
    instabilityG
    (
      const dictionary& XiGProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
    //- Disallow copy construct
    instabilityG(const instabilityG&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const instabilityG&) = delete;
  //- Destructor
  virtual ~instabilityG();
  // Member Functions
    //- Return the flame-wrinking generation rate
    virtual tmp<volScalarField> G() const;
    //- Return the flame diffusivity
    virtual tmp<volScalarField> Db() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiGProperties);
};

}  // namespace XiGModels
}  // namespace mousse

#endif

