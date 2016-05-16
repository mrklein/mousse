#ifndef SCOPE_BLEND_XI_EQ_HPP_
#define SCOPE_BLEND_XI_EQ_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiEqModels::SCOPEBlend
// Description
//   Simple SCOPEBlendXiEq model for XiEq based on SCOPEXiEqs correlation
//   with a linear correction function to give a plausible profile for XiEq.
//   See @link scope_laminar_flame_speed.hpp @endlink for details on the SCOPE
//   laminar flame speed model.

#include "xi_eq_model.hpp"


namespace mousse {
namespace XiEqModels {

class SCOPEBlend
:
  public XiEqModel
{
  // Private data
    //- Low turbulence intensity equilibrium Xi model
    autoPtr<XiEqModel> XiEqModelL_;
    //- High turbulence intensity equilibrium Xi model
    autoPtr<XiEqModel> XiEqModelH_;
public:
  //- Runtime type information
  TYPE_NAME("SCOPEBlend");
  // Constructors
    //- Construct from components
    SCOPEBlend
    (
      const dictionary& XiEqProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
    //- Disallow copy construct
    SCOPEBlend(const SCOPEBlend&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const SCOPEBlend&) = delete;
  //- Destructor
  virtual ~SCOPEBlend();
  // Member Functions
    //- Return the flame-wrinking XiEq
    virtual tmp<volScalarField> XiEq() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& /*XiEqProperties*/)
    {
      return true;
    }
};

}  // namespace XiEqModels
}  // namespace mousse

#endif

