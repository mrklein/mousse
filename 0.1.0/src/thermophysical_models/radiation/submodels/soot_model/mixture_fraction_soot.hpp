#ifndef THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_SOOT_MODEL_MIXTURE_FRACTION_SOOT_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_SOOT_MODEL_MIXTURE_FRACTION_SOOT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::mixtureFractionSoot
// Description
//   This soot model is purely an state model. The ammount of soot produced is
//   determined by a single step chemistry as :
//     nuf Fuel + nuOx Ox = nuP P + nuSoot soot
//   nuSoot is prescribed by the user.
//   The single step chemistry used is read from the combustion.
//   The soot is not considered into the thermodynamics of the system and it
//   is not considered as an extra specie in the solver.
//   The spacial distribution is given by the normalization of the first product
//   on the rhs of the reaction by default or it can be added as input.
//   The input dictionary reads like in the radiationProperties dictionary:
//   sootModel mixtureFractionSoot<gasHThermoPhysics>;
//   mixtureFractionSootCoeffs
//   {
//     nuSoot              0.015;
//     Wsoot               12;
//     mappingFieldName    P;
//   }
// SourceFiles
//   mixture_fraction_soot.cpp
#include "interpolation_look_up_table.hpp"
#include "soot_model.hpp"
#include "hash_table.hpp"
#include "fluid_thermo.hpp"
#include "single_step_reacting_mixture.hpp"
namespace mousse
{
namespace radiation
{
template<class ThermoType>
class mixtureFractionSoot
:
  public sootModel
{
  // Static functions
    //- Check mixture in thermo
    static const singleStepReactingMixture<ThermoType>& checkThermo
    (
      const fluidThermo&
    );
  // Private data
    //- Soot mass fraction
    volScalarField soot_;
    //- Soot model dictionary
    dictionary coeffsDict_;
    //- Soot yield
    scalar nuSoot_;
    //- Soot molecular weight
    scalar Wsoot_;
    //- Maximum soot mass concentration at stoichiometric
    scalar sootMax_;
    //- Name of the field mapping the soot
    word mappingFieldName_;
    //- Maximum value of the map field
    scalar mapFieldMax_;
    //- Thermo package
    const fluidThermo& thermo_;
    //- Auto Ptr to singleStepReactingMixture
    const singleStepReactingMixture<ThermoType>& mixture_;
public:
  //- Runtime type information
  TYPE_NAME("mixtureFractionSoot");
  // Constructors
    //- Construct from components
    mixtureFractionSoot
    (
      const dictionary& dict,
      const fvMesh& mesh,
      const word& modelType
    );
  //- Destructor
  virtual ~mixtureFractionSoot();
  // Member Functions
    // Edit
      //- Main update/correction routine
      virtual void correct();
    // Access
      //- Return Ysoot
      const volScalarField& soot() const
      {
        return soot_;
      }
};
}  // namespace mousse
}  // namespace radiation
#ifdef NoRepository
#   include "mixture_fraction_soot.cpp"
#endif
#endif
