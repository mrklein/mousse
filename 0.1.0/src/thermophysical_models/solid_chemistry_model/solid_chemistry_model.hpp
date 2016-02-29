#ifndef THERMOPHYSICAL_MODELS_SOLID_CHEMISTRY_MODEL_SOLID_CHEMISTRY_MODEL_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_CHEMISTRY_MODEL_SOLID_CHEMISTRY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidChemistryModel
// Description
//   Extends base solid chemistry model by adding a thermo package, and ODE
//   functions.
//   Introduces chemistry equation system and evaluation of chemical source
//   terms.
// SourceFiles
//   solid_chemistry_model.cpp
#include "reaction.hpp"
#include "ode_system.hpp"
#include "vol_fields.hpp"
#include "dimensioned_field.hpp"
#include "simple_matrix.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
template<class CompType, class SolidThermo>
class solidChemistryModel
:
  public CompType,
  public ODESystem
{
protected:
    //- Reference to solid mass fractions
    PtrList<volScalarField>& Ys_;
    //- Reactions
    const PtrList<Reaction<SolidThermo> >& reactions_;
    //- Thermodynamic data of solids
    const PtrList<SolidThermo>& solidThermo_;
    //- Number of solid components
    label nSolids_;
    //- Number of solid reactions
    label nReaction_;
    //- List of reaction rate per solid [kg/m3/s]
    PtrList<DimensionedField<scalar, volMesh> > RRs_;
    //- List of active reacting cells
    List<bool> reactingCells_;
  // Protected Member Functions
    //- Write access to source terms for solids
    inline PtrList<DimensionedField<scalar, volMesh> >& RRs();
    //- Set reacting status of cell, cellI
    void setCellReacting(const label cellI, const bool active);
public:
  //- Runtime type information
  TYPE_NAME("solidChemistryModel");
  // Constructors
    //- Construct from mesh and phase name
    solidChemistryModel(const fvMesh& mesh, const word& phaseName);
    //- Disallow copy constructor
    solidChemistryModel(const solidChemistryModel&) = delete;
    //- Disallow default bitwise assignment
    solidChemistryModel& operator=(const solidChemistryModel&) = delete;
  //- Destructor
  virtual ~solidChemistryModel();
  // Member Functions
    //- The reactions
    inline const PtrList<Reaction<SolidThermo> >& reactions() const;
    //- The number of reactions
    inline label nReaction() const;
    //- dc/dt = omega, rate of change in concentration, for each species
    virtual scalarField omega
    (
      const scalarField& c,
      const scalar T,
      const scalar p,
      const bool updateC0 = false
    ) const = 0;
    //- Return the reaction rate for reaction r and the reference
    //  species and charateristic times
    virtual scalar omega
    (
      const Reaction<SolidThermo>& r,
      const scalarField& c,
      const scalar T,
      const scalar p,
      scalar& pf,
      scalar& cf,
      label& lRef,
      scalar& pr,
      scalar& cr,
      label& rRef
    ) const = 0;
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
    ) const = 0;
    //- Calculates the reaction rates
    virtual void calculate() = 0;
    // Solid Chemistry model functions
      //- Return const access to the chemical source terms for solids
      inline const DimensionedField<scalar, volMesh>& RRs
      (
        const label i
      ) const;
      //- Return total solid source term
      inline tmp<DimensionedField<scalar, volMesh> > RRs() const;
      //- Solve the reaction system for the given time step
      //  and return the characteristic time
      virtual scalar solve(const scalar deltaT) = 0;
      //- Solve the reaction system for the given time step
      //  and return the characteristic time
      virtual scalar solve(const scalarField& deltaT);
      //- Return the chemical time scale
      virtual tmp<volScalarField> tc() const;
      //- Return source for enthalpy equation [kg/m/s3]
      virtual tmp<volScalarField> Sh() const;
      //- Return the heat release, i.e. enthalpy/sec [m2/s3]
      virtual tmp<volScalarField> dQ() const;
    // ODE functions (overriding abstract functions in ODE.H)
      //- Number of ODE's to solve
      virtual label nEqns() const = 0;
      virtual void derivatives
      (
        const scalar t,
        const scalarField& c,
        scalarField& dcdt
      ) const = 0;
      virtual void jacobian
      (
        const scalar t,
        const scalarField& c,
        scalarField& dcdt,
        scalarSquareMatrix& dfdc
      ) const = 0;
      virtual void solve
      (
        scalarField &c,
        scalar& T,
        scalar& p,
        scalar& deltaT,
        scalar& subDeltaT
      ) const = 0;
};
}  // namespace mousse

// Member Functions 
template<class CompType, class SolidThermo>
inline mousse::PtrList<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >&
mousse::solidChemistryModel<CompType, SolidThermo>::RRs()
{
  return RRs_;
}
template<class CompType, class SolidThermo>
inline const mousse::PtrList<mousse::Reaction<SolidThermo> >&
mousse::solidChemistryModel<CompType, SolidThermo>::reactions() const
{
  return reactions_;
}
template<class CompType, class SolidThermo>
inline mousse::label
mousse::solidChemistryModel<CompType, SolidThermo>::
nReaction() const
{
  return nReaction_;
}
template<class CompType, class SolidThermo>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::solidChemistryModel<CompType, SolidThermo>::RRs
(
  const label i
) const
{
  return RRs_[i];
}
template<class CompType, class SolidThermo>
inline mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::solidChemistryModel<CompType, SolidThermo>::RRs() const
{
  tmp<DimensionedField<scalar, volMesh> > tRRs
  {
    new DimensionedField<scalar, volMesh>
    {
      // IOobject
      {
        "RRs",
        this->time().timeName(),
        this->mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      this->mesh(),
      // dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
      {"zero", dimMass/dimVolume/dimTime, 0.0}
    }
  };
  if (this->chemistry_)
  {
    DimensionedField<scalar, volMesh>& RRs = tRRs();
    for (label i=0; i < nSolids_; i++)
    {
      RRs += RRs_[i];
    }
  }
  return tRRs;
}

#ifdef NoRepository
#   include "solid_chemistry_model.cpp"
#endif
#endif
