#ifndef PHASE_CHANGE_TWO_PHASE_MIXTURE_HPP_
#define PHASE_CHANGE_TWO_PHASE_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseChangeTwoPhaseMixture

#include "incompressible_two_phase_mixture.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "vol_fields.hpp"
#include "dimensioned_scalar.hpp"
#include "auto_ptr.hpp"
#include "pair.hpp"


namespace mousse {

class phaseChangeTwoPhaseMixture
:
  public incompressibleTwoPhaseMixture
{
protected:
  // Protected data
    dictionary phaseChangeTwoPhaseMixtureCoeffs_;
    //- Saturation vapour pressure
    dimensionedScalar pSat_;
public:
  //- Runtime type information
  TYPE_NAME("phaseChangeTwoPhaseMixture");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      phaseChangeTwoPhaseMixture,
      components,
      (
        const volVectorField& U,
        const surfaceScalarField& phi
      ),
      (U, phi)
    );
  // Selectors
    //- Return a reference to the selected phaseChange model
    static autoPtr<phaseChangeTwoPhaseMixture> New
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  // Constructors
    //- Construct from components
    phaseChangeTwoPhaseMixture
    (
      const word& type,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
      //- Disallow copy construct
      phaseChangeTwoPhaseMixture(const phaseChangeTwoPhaseMixture&) = delete;
      //- Disallow default bitwise assignment
      void operator=(const phaseChangeTwoPhaseMixture&) = delete;
    //- Destructor
    virtual ~phaseChangeTwoPhaseMixture()
    {}
    // Member Functions
      //- Return const-access to the saturation vapour pressure
      const dimensionedScalar& pSat() const { return pSat_; }
      //- Return the mass condensation and vaporisation rates as a
      //  coefficient to multiply (1 - alphal) for the condensation rate
      //  and a coefficient to multiply  alphal for the vaporisation rate
      virtual Pair<tmp<volScalarField>> mDotAlphal() const = 0;
    //- Return the mass condensation and vaporisation rates as coefficients
    //  to multiply (p - pSat)
    virtual Pair<tmp<volScalarField>> mDotP() const = 0;
    //- Return the volumetric condensation and vaporisation rates as a
    //  coefficient to multiply (1 - alphal) for the condensation rate
    //  and a coefficient to multiply  alphal for the vaporisation rate
    Pair<tmp<volScalarField>> vDotAlphal() const;
    //- Return the volumetric condensation and vaporisation rates as
    //  coefficients to multiply (p - pSat)
    Pair<tmp<volScalarField>> vDotP() const;
    //- Correct the phaseChange model
    virtual void correct() = 0;
    //- Read the transportProperties dictionary and update
    virtual bool read() = 0;
};

}  // namespace mousse

#endif
