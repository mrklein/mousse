#ifndef XI_EQ_MODEL_HPP_
#define XI_EQ_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiEqModel
// Description
//   Base-class for all XiEq models used by the b-XiEq combustion model.
//   The available models are :
//     \link basic_xi_sub_xi_eq.hpp \endlink
//     \link gulder.hpp \endlink
//     \link instability_xi_eq.hpp \endlink
//     \link scope_blend_xi_eq.hpp \endlink
//     \link scope_xi_eq.hpp \endlink

#include "iodictionary.hpp"
#include "psiu_reaction_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

class XiEqModel
{
protected:
  // Protected data
    //- Dictionary
    dictionary XiEqModelCoeffs_;
    //- Thermo
    const psiuReactionThermo& thermo_;
    //- Turbulence
    const compressible::RASModel& turbulence_;
    //- Laminar burning velocity
    const volScalarField& Su_;
public:
  //- Runtime type information
  TYPE_NAME("XiEqModel");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      XiEqModel,
      dictionary,
      (
        const dictionary& XiEqProperties,
        const psiuReactionThermo& thermo,
        const compressible::RASModel& turbulence,
        const volScalarField& Su
      ),
      (
        XiEqProperties,
        thermo,
        turbulence,
        Su
      )
    );
  // Selectors
    //- Return a reference to the selected XiEq model
    static autoPtr<XiEqModel> New
    (
      const dictionary& XiEqProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
  // Constructors
    //- Construct from components
    XiEqModel
    (
      const dictionary& XiEqProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
    //- Disallow copy construct
    XiEqModel(const XiEqModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const XiEqModel&) = delete;
  //- Destructor
  virtual ~XiEqModel();
  // Member Functions
    //- Return the flame-wrinking XiEq
    virtual tmp<volScalarField> XiEq() const
    {
      return turbulence_.muEff();
    }
    //- Return the sub-grid Schelkin effect
    tmp<volScalarField> calculateSchelkinEffect(const scalar) const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiEqProperties) = 0;
    //- Write fields
    void writeFields() const;
};

}  // namespace mousse

#endif

