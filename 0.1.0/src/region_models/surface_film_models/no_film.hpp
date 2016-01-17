// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noFilm
// Description
//   Dummy surface film model for 'none'
// SourceFiles
//   no_film.cpp
#ifndef no_film_hpp_
#define no_film_hpp_
#include "surface_film_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class noFilm
:
  public surfaceFilmModel
{
protected:
  // Protected member functions
    //- Read control parameters from dictionary
    virtual bool read();
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    noFilm
    (
      const word& modelType,
      const fvMesh& mesh,
      const dimensionedVector& g,
      const word& regionType
    );
    //- Disallow default bitwise copy construct
    noFilm(const noFilm&) = delete;
    //- Disallow default bitwise assignment
    noFilm& operator=(const noFilm&) = delete;
  //- Destructor
  virtual ~noFilm();
  // Member Functions
    // Access
      //- External hook to add sources to the film
      virtual void addSources
      (
        const label patchI,
        const label faceI,
        const scalar massSource,
        const vector& momentumSource,
        const scalar pressureSource,
        const scalar energySource
      );
    // Fields
      //- Return the film thickness [m]
      virtual const volScalarField& delta() const;
      //- Return the film coverage, 1 = covered, 0 = uncovered / []
      virtual const volScalarField& alpha() const;
      //- Return the film velocity [m/s]
      virtual const volVectorField& U() const;
      //- Return the film density [kg/m3]
      virtual const volScalarField& rho() const;
      //- Return the film surface velocity [m/s]
      virtual const volVectorField& Us() const;
      //- Return the film wall velocity [m/s]
      virtual const volVectorField& Uw() const;
      //- Return the film mean temperature [K]
      virtual const volScalarField& T() const;
      //- Return the film surface temperature [K]
      virtual const volScalarField& Ts() const;
      //- Return the film wall temperature [K]
      virtual const volScalarField& Tw() const;
      //- Return the film specific heat capacity [J/kg/K]
      virtual const volScalarField& Cp() const;
      //- Return the film thermal conductivity [W/m/K]
      virtual const volScalarField& kappa() const;
      //- Return const access to the surface tension / [m/s2]
      inline const volScalarField& sigma() const;
      // Transfer fields - to the primary region
        //- Return mass transfer source - Eulerian phase only
        virtual tmp<volScalarField> primaryMassTrans() const;
        //- Return the film mass available for transfer
        virtual const volScalarField& cloudMassTrans() const;
        //- Return the parcel diameters originating from film
        virtual const volScalarField& cloudDiameterTrans() const;
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
} // regionModels
}  // namespace mousse
#endif
