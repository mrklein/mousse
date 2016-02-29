#ifndef THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_MODEL_CHEMISTRY_MODEL_HPP_
#define THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_MODEL_CHEMISTRY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::chemistryModel
// Description
//   Extends base chemistry model by adding a thermo package, and ODE functions.
//   Introduces chemistry equation system and evaluation of chemical source
//   terms.
// SourceFiles
//   chemistry_model.cpp
#include "reaction.hpp"
#include "ode_system.hpp"
#include "simple_matrix.hpp"
#include "dimensioned_field.hpp"
#include "vol_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
template<class CompType, class ThermoType>
class chemistryModel
:
  public CompType,
  public ODESystem
{
  // Private Member Functions
    //- Solve the reaction system for the given time step
    //  of given type and return the characteristic time
    template<class DeltaTType>
    scalar solve(const DeltaTType& deltaT);
protected:
  typedef ThermoType thermoType;
  // Private data
    //- Reference to the field of specie mass fractions
    PtrList<volScalarField>& Y_;
    //- Reactions
    const PtrList<Reaction<ThermoType> >& reactions_;
    //- Thermodynamic data of the species
    const PtrList<ThermoType>& specieThermo_;
    //- Number of species
    label nSpecie_;
    //- Number of reactions
    label nReaction_;
    //- Temperature below which the reaction rates are assumed 0
    scalar Treact_;
    //- List of reaction rate per specie [kg/m3/s]
    PtrList<DimensionedField<scalar, volMesh> > RR_;
  // Protected Member Functions
    //- Write access to chemical source terms
    //  (e.g. for multi-chemistry model)
    inline PtrList<DimensionedField<scalar, volMesh> >& RR();
public:
  //- Runtime type information
  TYPE_NAME("chemistryModel");
  // Constructors
    //- Construct from mesh
    chemistryModel(const fvMesh& mesh, const word& phaseName);
    //- Disallow copy constructor
    chemistryModel(const chemistryModel&) = delete;
    //- Disallow default bitwise assignment
    chemistryModel& operator=(const chemistryModel&) = delete;
  //- Destructor
  virtual ~chemistryModel();
  // Member Functions
    //- The reactions
    inline const PtrList<Reaction<ThermoType> >& reactions() const;
    //- Thermodynamic data of the species
    inline const PtrList<ThermoType>& specieThermo() const;
    //- The number of species
    inline label nSpecie() const;
    //- The number of reactions
    inline label nReaction() const;
    //- Temperature below which the reaction rates are assumed 0
    inline scalar Treact() const;
    //- Temperature below which the reaction rates are assumed 0
    inline scalar& Treact();
    //- dc/dt = omega, rate of change in concentration, for each species
    virtual tmp<scalarField> omega
    (
      const scalarField& c,
      const scalar T,
      const scalar p
    ) const;
    //- Return the reaction rate for reaction r and the reference
    //  species and charateristic times
    virtual scalar omega
    (
      const Reaction<ThermoType>& r,
      const scalarField& c,
      const scalar T,
      const scalar p,
      scalar& pf,
      scalar& cf,
      label& lRef,
      scalar& pr,
      scalar& cr,
      label& rRef
    ) const;
    //- Return the reaction rate for iReaction and the reference
    //  species and charateristic times
    virtual scalar omegaI
    (
      label iReaction,
      const scalarField& c,
      const scalar T,
      const scalar p,
      scalar& pf,
      scalar& cf,
      label& lRef,
      scalar& pr,
      scalar& cr,
      label& rRef
    ) const;
    //- Calculates the reaction rates
    virtual void calculate();
    // Chemistry model functions (overriding abstract functions in
    // basicChemistryModel.H)
      //- Return const access to the chemical source terms for specie, i
      inline const DimensionedField<scalar, volMesh>& RR
      (
        const label i
      ) const;
      //- Return non const access to chemical source terms [kg/m3/s]
      virtual DimensionedField<scalar, volMesh>& RR
      (
        const label i
      );
      //- Return reaction rate of the speciei in reactionI
      virtual tmp<DimensionedField<scalar, volMesh> > calculateRR
      (
        const label reactionI,
        const label speciei
      ) const;
      //- Solve the reaction system for the given time step
      //  and return the characteristic time
      virtual scalar solve(const scalar deltaT);
      //- Solve the reaction system for the given time step
      //  and return the characteristic time
      virtual scalar solve(const scalarField& deltaT);
      //- Return the chemical time scale
      virtual tmp<volScalarField> tc() const;
      //- Return source for enthalpy equation [kg/m/s3]
      virtual tmp<volScalarField> Sh() const;
      //- Return the heat release, i.e. enthalpy/sec [kg/m2/s3]
      virtual tmp<volScalarField> dQ() const;
    // ODE functions (overriding abstract functions in ODE.H)
      //- Number of ODE's to solve
      virtual label nEqns() const;
      virtual void derivatives
      (
        const scalar t,
        const scalarField& c,
        scalarField& dcdt
      ) const;
      virtual void jacobian
      (
        const scalar t,
        const scalarField& c,
        scalarField& dcdt,
        scalarSquareMatrix& dfdc
      ) const;
      virtual void solve
      (
        scalarField &c,
        scalar& T,
        scalar& p,
        scalar& deltaT,
        scalar& subDeltaT
      ) const;
};
}  // namespace mousse

// Member Functions 
template<class CompType, class ThermoType>
inline mousse::PtrList<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >&
mousse::chemistryModel<CompType, ThermoType>::RR()
{
  return RR_;
}
template<class CompType, class ThermoType>
inline const mousse::PtrList<mousse::Reaction<ThermoType> >&
mousse::chemistryModel<CompType, ThermoType>::reactions() const
{
  return reactions_;
}
template<class CompType, class ThermoType>
inline const mousse::PtrList<ThermoType>&
mousse::chemistryModel<CompType, ThermoType>::specieThermo() const
{
  return specieThermo_;
}
template<class CompType, class ThermoType>
inline mousse::label
mousse::chemistryModel<CompType, ThermoType>::nSpecie() const
{
  return nSpecie_;
}
template<class CompType, class ThermoType>
inline mousse::label
mousse::chemistryModel<CompType, ThermoType>::nReaction() const
{
  return nReaction_;
}
template<class CompType, class ThermoType>
inline mousse::scalar
mousse::chemistryModel<CompType, ThermoType>::Treact() const
{
  return Treact_;
}
template<class CompType, class ThermoType>
inline mousse::scalar&
mousse::chemistryModel<CompType, ThermoType>::Treact()
{
  return Treact_;
}
template<class CompType, class ThermoType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::chemistryModel<CompType, ThermoType>::RR
(
  const label i
) const
{
  return RR_[i];
}
template<class CompType, class ThermoType>
mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::chemistryModel<CompType, ThermoType>::RR
(
  const label i
)
{
  return RR_[i];
}
#ifdef NoRepository
#   include "chemistry_model.cpp"
#endif
#endif
