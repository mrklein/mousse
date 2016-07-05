#ifndef REGION_MODELS_PYROLYSIS_MODELS_PYROLYSIS_MODEL_HPP_
#define REGION_MODELS_PYROLYSIS_MODELS_PYROLYSIS_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModels::pyrolysisModels::pyrolysisModel
// Description
//   Base class for pyrolysis models

#include "run_time_selection_tables.hpp"
#include "vol_fields_fwd.hpp"
#include "region_model_1d.hpp"


namespace mousse {

// Forward declaration of classes
class fvMesh;
class Time;


namespace regionModels {
namespace pyrolysisModels {

class pyrolysisModel
:
  public regionModel1D
{
private:
  // Private Member Functions
    //- Construct fields
    void constructMeshObjects();
    //- Read pyrolysis controls
    void readPyrolysisControls();
protected:
  // Protected Member Functions
    //- Read control parameters
    virtual bool read();
    //- Read control parameters from dictionary
    virtual bool read(const dictionary& dict);
public:
  //- Runtime type information
  TYPE_NAME("pyrolysisModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      pyrolysisModel,
      mesh,
      (
        const word& modelType,
        const fvMesh& mesh,
        const word& regionType
      ),
      (modelType, mesh, regionType)
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      pyrolysisModel,
      dictionary,
      (
        const word& modelType,
        const fvMesh& mesh,
        const dictionary& dict,
        const word& regionType
      ),
      (modelType, mesh, dict, regionType)
    );
  // Constructors
    //- Construct null from mesh
    pyrolysisModel
    (
      const fvMesh& mesh,
      const word& regionType
    );
    //- Construct from type name and mesh
    pyrolysisModel
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& regionType
    );
    //- Construct from type name and mesh and dictionary
    pyrolysisModel
    (
      const word& modelType,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& regionType
    );
    //- Return clone
    autoPtr<pyrolysisModel> clone() const
    {
      NOT_IMPLEMENTED("autoPtr<pyrolysisModel> clone() const");
      return autoPtr<pyrolysisModel>{NULL};
    }
    //- Disallow default bitwise copy construct
    pyrolysisModel(const pyrolysisModel&) = delete;
    //- Disallow default bitwise assignment
    pyrolysisModel& operator=(const pyrolysisModel&) = delete;
  // Selectors
    //- Return a reference to the selected pyrolysis model
    static autoPtr<pyrolysisModel> New
    (
      const fvMesh& mesh,
      const word& regionType = "pyrolysis"
    );
    //- Return a reference to a named selected pyrolysis model
    static autoPtr<pyrolysisModel> New
    (
      const fvMesh& mesh,
      const dictionary& dict,
      const word& regionType = "pyrolysis"
    );
  //- Destructor
  virtual ~pyrolysisModel();
  // Member Functions
    // Access
      // Fields
        //- Return density [kg/m3]
        virtual const volScalarField& rho() const = 0;
        //- Return const temperature [K]
        virtual const volScalarField& T() const = 0;
        //- Return specific heat capacity [J/kg/K]
        virtual const tmp<volScalarField> Cp() const = 0;
        //- Return the region absorptivity [1/m]
        virtual tmp<volScalarField> kappaRad() const = 0;
        //- Return the region thermal conductivity [W/m/k]
        virtual tmp<volScalarField> kappa() const = 0;
        //- Return the total gas mass flux to primary region [kg/m2/s]
        virtual const surfaceScalarField& phiGas() const = 0;
      // Sources
        //- External hook to add mass to the primary region
        virtual scalar addMassSources
        (
          const label patchI,
          const label faceI
        );
    // Helper function
      //- Mean diffusion number of the solid region
      virtual scalar solidRegionDiffNo() const;
      //- Return max diffusivity allowed in the solid
      virtual scalar maxDiff() const;
};

}  // namespace pyrolysisModels
}  // namespace regionModels
}  // namespace mousse

#endif

