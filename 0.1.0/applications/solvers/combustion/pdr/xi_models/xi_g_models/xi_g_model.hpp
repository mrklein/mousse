// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiGModel
// Description
//   Base-class for all Xi generation models used by the b-Xi combustion model.
//   See Technical Report SH/RE/01R for details on the PDR modelling. For details
//   on the use of XiGModel see \link XiModel.H \endlink. The model available is
//   \link instabilityG.H \endlink
// SourceFiles
//   xi_g_model.cpp
#ifndef XI_G_MODEL_HPP_
#define XI_G_MODEL_HPP_
#include "iodictionary.hpp"
#include "psiu_reaction_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class XiGModel
{
protected:
  // Protected data
    dictionary XiGModelCoeffs_;
    const psiuReactionThermo& thermo_;
    const compressible::RASModel& turbulence_;
    const volScalarField& Su_;
public:
  //- Runtime type information
  TYPE_NAME("XiGModel");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      XiGModel,
      dictionary,
      (
        const dictionary& XiGProperties,
        const psiuReactionThermo& thermo,
        const compressible::RASModel& turbulence,
        const volScalarField& Su
      ),
      (
        XiGProperties,
        thermo,
        turbulence,
        Su
      )
    );
  // Selectors
    //- Return a reference to the selected XiG model
    static autoPtr<XiGModel> New
    (
      const dictionary& XiGProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
  // Constructors
    //- Construct from components
    XiGModel
    (
      const dictionary& XiGProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
    //- Disallow copy construct
    XiGModel(const XiGModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const XiGModel&) = delete;
  //- Destructor
  virtual ~XiGModel();
  // Member Functions
    //- Return the flame-wrinking genration rate
    virtual tmp<volScalarField> G() const = 0;
    //- Return the flame diffusivity
    virtual tmp<volScalarField> Db() const
    {
      return turbulence_.muEff();
    }
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiGProperties) = 0;
};
}  // namespace mousse
#endif
