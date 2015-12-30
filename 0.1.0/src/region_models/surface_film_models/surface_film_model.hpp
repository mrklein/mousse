// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModels::surfaceFilmModels::surfaceFilmModel
// Description
//   Base class for surface film models
// SourceFiles
//   surface_film_model_i.hpp
//   surface_film_model.cpp
//   surface_film_model_new.cpp
#ifndef surface_film_model_hpp_
#define surface_film_model_hpp_
#include "single_layer_region.hpp"
#include "dimensioned_vector.hpp"
#include "run_time_selection_tables.hpp"
#include "vol_fields_fwd.hpp"
#include "dimensioned_field.hpp"
#include "label_list.hpp"
#include "named_enum.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class surfaceFilmModel
:
  public singleLayerRegion
{
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    surfaceFilmModel(const surfaceFilmModel&);
    //- Disallow default bitwise assignment
    void operator=(const surfaceFilmModel&);
protected:
  // Protected data
    //- Acceleration due to gravity [m/s2]
    const dimensionedVector& g_;
  // Protected member functions
    //- Read control parameters from dictionary
    virtual bool read();
public:
  //- Runtime type information
  TypeName("surfaceFilmModel");
  // Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      surfaceFilmModel,
      mesh,
      (
        const word& modelType,
        const fvMesh& mesh,
        const dimensionedVector& g,
        const word& regionType
      ),
      (modelType, mesh, g, regionType)
    );
  // Constructors
    //- Construct from type name, mesh and gravity vector
    surfaceFilmModel
    (
      const word& modelType,
      const fvMesh& mesh,
      const dimensionedVector& g,
      const word& regionType
    );
  // Selectors
    //- Return a reference to the selected surface film model
    static autoPtr<surfaceFilmModel> New
    (
      const fvMesh& mesh,
      const dimensionedVector& g,
      const word& regionType = "surfaceFilm"
    );
  //- Destructor
  virtual ~surfaceFilmModel();
  // Member Functions
    // Access
      //- Return the accleration due to gravity
      inline const dimensionedVector& g() const;
      //- External hook to add sources to the film
      virtual void addSources
      (
        const label patchI,
        const label faceI,
        const scalar massSource,
        const vector& momentumSource,
        const scalar pressureSource,
        const scalar energySource
      ) = 0;
    // Solution parameters
      //- Courant number evaluation
      virtual scalar CourantNumber() const;
    // Fields
      //- Return the film thickness [m]
      virtual const volScalarField& delta() const = 0;
      //- Return the film coverage, 1 = covered, 0 = uncovered / []
      virtual const volScalarField& alpha() const = 0;
      //- Return the film velocity [m/s]
      virtual const volVectorField& U() const = 0;
      //- Return the film surface velocity [m/s]
      virtual const volVectorField& Us() const = 0;
      //- Return the film wall velocity [m/s]
      virtual const volVectorField& Uw() const = 0;
      //- Return the film density [kg/m3]
      virtual const volScalarField& rho() const = 0;
      //- Return the film mean temperature [K]
      virtual const volScalarField& T() const = 0;
      //- Return the film surface temperature [K]
      virtual const volScalarField& Ts() const = 0;
      //- Return the film wall temperature [K]
      virtual const volScalarField& Tw() const = 0;
      //- Return the film specific heat capacity [J/kg/K]
      virtual const volScalarField& Cp() const = 0;
      //- Return the film thermal conductivity [W/m/K]
      virtual const volScalarField& kappa() const = 0;
      //- Return the film surface tension [N/m]
      virtual const volScalarField& sigma() const = 0;
      // Transfer fields - to the primary region
        //- Return mass transfer source - Eulerian phase only
        virtual tmp<volScalarField> primaryMassTrans() const = 0;
        //- Return the film mass available for transfer
        virtual const volScalarField& cloudMassTrans() const = 0;
        //- Return the parcel diameters originating from film
        virtual const volScalarField& cloudDiameterTrans() const = 0;
    // Source fields
      // Mapped into primary region
        //- Return total mass source - Eulerian phase only
        virtual tmp<DimensionedField<scalar, volMesh> > Srho() const;
        //- Return mass source for specie i - Eulerian phase only
        virtual tmp<DimensionedField<scalar, volMesh> > Srho
        (
          const label i
        ) const;
        //- Return enthalpy source - Eulerian phase only
        virtual tmp<DimensionedField<scalar, volMesh> > Sh() const;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#include "surface_film_model_i.hpp"
#endif
