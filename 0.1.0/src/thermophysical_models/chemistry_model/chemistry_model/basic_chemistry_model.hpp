#ifndef THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_MODEL_BASIC_CHEMISTRY_MODEL_HPP_
#define THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_MODEL_BASIC_CHEMISTRY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicChemistryModel
// Description
//   Base class for chemistry models
// SourceFiles
//   basic_chemistry_model.cpp
#include "iodictionary.hpp"
#include "switch.hpp"
#include "scalar_field.hpp"
#include "vol_fields_fwd.hpp"
#include "vol_mesh.hpp"
#include "dimensioned_field.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
class basicChemistryModel
:
  public IOdictionary
{
protected:
  // Protected data
    //- Reference to the mesh database
    const fvMesh& mesh_;
    //- Chemistry activation switch
    Switch chemistry_;
    //- Initial chemical time step
    const scalar deltaTChemIni_;
    //- Latest estimation of integration step
    DimensionedField<scalar, volMesh> deltaTChem_;
  // Protected Member Functions
    //- Return non-const access to the latest estimation of integration
    //  step, e.g. for multi-chemistry model
    inline DimensionedField<scalar, volMesh>& deltaTChem();
    //- Correct function - updates due to mesh changes
    void correct();
public:
  //- Runtime type information
  TYPE_NAME("basicChemistryModel");
  // Constructors
    //- Construct from mesh
    basicChemistryModel(const fvMesh& mesh, const word& phaseName);
    //- Construct as copy (not implemented)
    basicChemistryModel(const basicChemistryModel&) = delete;
    //- Disallow default bitwise assignment
    basicChemistryModel& operator=(const basicChemistryModel&) = delete;
  // Selectors
    //- Generic New for each of the related chemistry model
    template<class Thermo>
    static autoPtr<Thermo> New(const fvMesh& mesh, const word& phaseName);
  //- Destructor
  virtual ~basicChemistryModel();
  // Member Functions
    //- Return const access to the mesh database
    inline const fvMesh& mesh() const;
    //- Chemistry activation switch
    inline Switch chemistry() const;
    //- Return the latest estimation of integration step
    inline const DimensionedField<scalar, volMesh>& deltaTChem() const;
    // Functions to be derived in derived classes
      // Fields
        //- Return const access to chemical source terms [kg/m3/s]
        virtual const DimensionedField<scalar, volMesh>& RR
        (
          const label i
        ) const = 0;
        //- Return access to chemical source terms [kg/m3/s]
        virtual DimensionedField<scalar, volMesh>& RR
        (
          const label i
        ) = 0;
        //- Return reaction rate of the speciei in reactioni
        virtual tmp<DimensionedField<scalar, volMesh> > calculateRR
        (
          const label reactioni,
          const label speciei
        ) const = 0;
      // Chemistry solution
        //- Calculates the reaction rates
        virtual void calculate() = 0;
        //- Solve the reaction system for the given time step
        //  and return the characteristic time
        virtual scalar solve(const scalar deltaT) = 0;
        //- Solve the reaction system for the given time step
        //  and return the characteristic time
        virtual scalar solve(const scalarField& deltaT) = 0;
        //- Return the chemical time scale
        virtual tmp<volScalarField> tc() const = 0;
        //- Return source for enthalpy equation [kg/m/s3]
        virtual tmp<volScalarField> Sh() const = 0;
        //- Return the heat release, i.e. enthalpy/sec [m2/s3]
        virtual tmp<volScalarField> dQ() const = 0;
};
}  // namespace mousse

// Member Functions 
inline const mousse::fvMesh& mousse::basicChemistryModel::mesh() const
{
  return mesh_;
}
inline mousse::Switch mousse::basicChemistryModel::chemistry() const
{
  return chemistry_;
}
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::basicChemistryModel::deltaTChem() const
{
  return deltaTChem_;
}
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::basicChemistryModel::deltaTChem()
{
  return deltaTChem_;
}

#ifdef NoRepository
#   include "basic_chemistry_model_templates.cpp"
#endif
#endif
