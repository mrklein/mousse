// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicSolidChemistryModel
// Description
//   Chemistry model for solid thermodynamics
// SourceFiles
//   basic_solid_chemistry_model_i.hpp
//   basic_solid_chemistry_model.cpp
//   new_chemistry_solid_model.cpp
#ifndef basic_solid_chemistry_model_hpp_
#define basic_solid_chemistry_model_hpp_
#include "basic_chemistry_model.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "solid_reaction_thermo.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
class basicSolidChemistryModel
:
  public basicChemistryModel
{
protected:
  // Protected data
    //- Solid thermo package
    autoPtr<solidReactionThermo> solidThermo_;
public:
  //- Runtime type information
  TYPE_NAME("basicSolidChemistryModel");
  //- Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    basicSolidChemistryModel,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Constructors
    //- Construct from mesh
    basicSolidChemistryModel(const fvMesh& mesh, const word& phaseName);
    //- Construct as copy (not implemented)
    basicSolidChemistryModel(const basicSolidChemistryModel&) = delete;
    //- Disallow default bitwise assignment
    basicSolidChemistryModel& operator=
    (
      const basicSolidChemistryModel&
    ) = delete;
  //- Selector
  static autoPtr<basicSolidChemistryModel> New
  (
    const fvMesh& mesh,
    const word& phaseName=word::null
  );
  //- Destructor
  virtual ~basicSolidChemistryModel();
  // Member Functions
    //- Return access to the solid thermo package
    inline solidReactionThermo& solidThermo();
    //- Return const access to the solid thermo package
    inline const solidReactionThermo& solidThermo() const;
    //- Return total gases mass source term [kg/m3/s]
    virtual tmp<DimensionedField<scalar, volMesh> > RRg() const = 0;
    //- Return total solids mass source term [kg/m3/s]
    virtual tmp<DimensionedField<scalar, volMesh> > RRs() const = 0;
    //- Return chemical source terms for solids [kg/m3/s]
    virtual const DimensionedField<scalar, volMesh>& RRs
    (
      const label i
    ) const = 0;
    //- Return chemical source terms for gases [kg/m3/s]
    virtual const DimensionedField<scalar, volMesh>& RRg
    (
      const label i
    ) const = 0;
    //- Returns the reaction rate of the speciei in reactionI
    virtual tmp<DimensionedField<scalar, volMesh> > calculateRR
    (
      const label reactionI,
      const label speciei
    ) const;
    //- Return sensible enthalpy for gas i [J/Kg]
    virtual tmp<volScalarField> gasHs
    (
      const volScalarField& p,
      const volScalarField& T,
      const label i
    ) const = 0;
    //- Return specie Table for gases
    virtual const speciesTable& gasTable() const = 0;
    //- Set reacting status of cell, cellI
    virtual void setCellReacting(const label cellI, const bool active) = 0;
    //- Calculates the reaction rates
    virtual void calculate() = 0;
    //- Return const access to the total source terms
    virtual const DimensionedField<scalar, volMesh>& RR
    (
      const label i
    ) const;
    //- Return non-const access to the total source terms
    virtual DimensionedField<scalar, volMesh>& RR(const label i);
};
}  // namespace mousse
#include "basic_solid_chemistry_model_i.hpp"
#endif
