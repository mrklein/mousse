#ifndef REGION_MODELS_THERMAL_BAFFLE_MODELS_THERMAL_BAFFLE_MODEL_HPP_
#define REGION_MODELS_THERMAL_BAFFLE_MODELS_THERMAL_BAFFLE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermalBaffleModel
// Description

#include "run_time_selection_tables.hpp"
#include "scalar_io_field.hpp"
#include "auto_ptr.hpp"
#include "vol_fields_fwd.hpp"
#include "solid_thermo.hpp"
#include "region_model_1d.hpp"
#include "radiation_model.hpp"


namespace mousse {
namespace regionModels {
namespace thermalBaffleModels {

class thermalBaffleModel
:
  public regionModel1D
{
private:
  // Private Member Functions
    //- Initialize thermal Baffle
    void init();
protected:
  // Protected Data
    //- Baffle physical thickness
    scalarField thickness_;
    //- Baffle mesh thickness
    dimensionedScalar delta_;
    //- Is it one dimension
    bool oneD_;
    //- Is thickness constant
    bool constantThickness_;
  // Protected Member Functions
    //- Read control parameters from IO dictionary
    virtual bool read();
    //- Read control parameters from dictionary
    virtual bool read(const dictionary&);
public:
  //- Runtime type information
  TYPE_NAME("thermalBaffleModel");
  // Declare runtime constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      thermalBaffleModel,
      mesh,
      (
        const word& modelType,
        const fvMesh& mesh
      ),
      (modelType, mesh)
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      thermalBaffleModel,
      dictionary,
      (
        const word& modelType,
        const fvMesh& mesh,
        const dictionary& dict
      ),
      (modelType, mesh, dict)
    );
  // Constructors
    //- Construct null from mesh
    thermalBaffleModel(const fvMesh& mesh);
    //- Construct from type name and mesh
    thermalBaffleModel(const word& modelType, const fvMesh& mesh);
    //- Construct from type name and mesh and dict
    thermalBaffleModel
    (
      const word& modelType,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    thermalBaffleModel(const thermalBaffleModel&) = delete;
    //- Disallow default bitwise assignment
    thermalBaffleModel& operator=(const thermalBaffleModel&) = delete;
  // Selectors
    //- Return a reference to the selected model
    static autoPtr<thermalBaffleModel> New(const fvMesh& mesh);
    //- Return a reference to the selected model using dictionary
    static autoPtr<thermalBaffleModel> New
    (
      const fvMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  virtual ~thermalBaffleModel();
  // Member Functions
    // Access
      //- Return solid thermo
      virtual const solidThermo& thermo() const = 0;
      //- Return thickness
      const scalarField& thickness() const
      {
        return thickness_;
      }
      //- Return geometrical thickness
      const dimensionedScalar& delta() const
      {
        return delta_;
      }
      //- Return if region is one dimensional
      bool oneD() const
      {
        return oneD_;
      }
      //- Return if region has constant thickness
      bool constantThickness() const
      {
        return constantThickness_;
      }
      // Fields
        //- Return density [kg/m3]
        virtual const volScalarField& rho() const = 0;
        //- Return const temperature [K]
        virtual const volScalarField& T() const = 0;
        //- Return specific heat capacity [J/kg/K]
        virtual const tmp<volScalarField> Cp() const = 0;
        //- Return the region absorptivity [1/m]
        virtual const volScalarField& kappaRad() const = 0;
        //- Return the region thermal conductivity [W/m/k]
        virtual const volScalarField& kappa() const = 0;
    // Evolution
      //- Pre-evolve region
      virtual void preEvolveRegion();
};

}  // namespace thermalBaffleModels
}  // namespace regionModels
}  // namespace mousse

#endif

