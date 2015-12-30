// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pyrolysisChemistryModel
// Description
//   Pyrolysis chemistry model. It includes gas phase in the solid
//   reaction.
// SourceFiles
//   pyrolysis_chemistry_model_i.hpp
//   pyrolysis_chemistry_model.cpp
#ifndef pyrolysis_chemistry_model_hpp_
#define pyrolysis_chemistry_model_hpp_
#include "vol_fields_fwd.hpp"
#include "dimensioned_field.hpp"
#include "solid_chemistry_model.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
template<class CompType, class SolidThermo, class GasThermo>
class pyrolysisChemistryModel
:
  public solidChemistryModel<CompType, SolidThermo>
{
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const pyrolysisChemistryModel&);
protected:
    //- List of gas species present in reaction system
    speciesTable pyrolisisGases_;
    //- Thermodynamic data of gases
    PtrList<GasThermo> gasThermo_;
    //- Number of gas species
    label nGases_;
    //- Number of components being solved by ODE
    label nSpecie_;
    //- List of reaction rate per gas [kg/m3/s]
    PtrList<DimensionedField<scalar, volMesh> > RRg_;
  // Protected Member Functions
    //- Write access to source terms for gases
    inline PtrList<DimensionedField<scalar, volMesh> >& RRg();
private:
    //- List of accumulative solid concentrations
    mutable PtrList<volScalarField> Ys0_;
    //- Cell counter
    label cellCounter_;
public:
  //- Runtime type information
  TypeName("pyrolysis");
  // Constructors
    //- Construct from mesh and phase name
    pyrolysisChemistryModel(const fvMesh& mesh, const word& phaseName);
  //- Destructor
  virtual ~pyrolysisChemistryModel();
  // Member Functions
    //- Thermodynamic data of gases
    inline const PtrList<GasThermo>& gasThermo() const;
    //- Gases table
    inline const speciesTable& gasTable() const;
    //- The number of solids
    inline label nSpecie() const;
    //- The number of solids
    inline label nGases() const;
    //- dc/dt = omega, rate of change in concentration, for each species
    virtual scalarField omega
    (
      const scalarField& c,
      const scalar T,
      const scalar p,
      const bool updateC0 = false
    ) const;
    //- Return the reaction rate for reaction r
    // NOTE: Currently does not calculate reference specie and
    // characteristic times (pf, cf,l Ref, etc.)
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
    ) const;
    //- Return the reaction rate for iReaction
    // NOTE: Currently does not calculate reference specie and
    // characteristic times (pf, cf,l Ref, etc.)
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
    // Chemistry model functions
      //- Return const access to the chemical source terms for gases
      inline const DimensionedField<scalar, volMesh>& RRg
      (
        const label i
      ) const;
      //- Return total gas source term
      inline tmp<DimensionedField<scalar, volMesh> > RRg() const;
      //- Return sensible enthalpy for gas i [J/Kg]
      virtual tmp<volScalarField> gasHs
      (
        const volScalarField& p,
        const volScalarField& T,
        const label i
      ) const;
      //- Solve the reaction system for the given time step
      //  and return the characteristic time
      virtual scalar solve(const scalar deltaT);
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
#   include "pyrolysis_chemistry_model_i.hpp"
#ifdef NoRepository
#   include "pyrolysis_chemistry_model.cpp"
#endif
#endif
