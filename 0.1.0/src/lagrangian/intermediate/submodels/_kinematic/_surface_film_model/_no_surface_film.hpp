#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TSURFACE_FILM_MODEL_TNO_SURFACE_FILM_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TSURFACE_FILM_MODEL_TNO_SURFACE_FILM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoSurfaceFilm
// Description
//   Place holder for 'none' option
// SourceFiles
//   _no_surface_film.cpp
#include "_surface_film_model.hpp"
namespace mousse
{
template<class CloudType>
class NoSurfaceFilm
:
  public SurfaceFilmModel<CloudType>
{
protected:
  // Protected data
    //- Convenience typedef for parcel type
    typedef typename CloudType::parcelType parcelType;
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoSurfaceFilm(const dictionary&, CloudType&);
    //- Construct copy
    NoSurfaceFilm(const NoSurfaceFilm<CloudType>& dm);
    //- Construct and return a clone
    virtual autoPtr<SurfaceFilmModel<CloudType> > clone() const
    {
      return autoPtr<SurfaceFilmModel<CloudType> >
      (
        new NoSurfaceFilm<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoSurfaceFilm();
  // Member Functions
    // Evaluation
      //- Flag to indicate whether model activates the surface film model
      virtual bool active() const;
      //- Transfer parcel from cloud to surface film
      //  Returns true if parcel is to be transferred
      virtual bool transferParcel
      (
        parcelType& p,
        const polyPatch& pp,
        bool& keepParticle
      );
      //- Set parcel properties
      virtual void setParcelProperties
      (
        parcelType& p,
        const label filmCellI
      ) const;
    // I-O
      //- Write surface film info to stream
      virtual void info(Ostream& os);
};
}  // namespace mousse
#ifdef NoRepository
#   include "_no_surface_film.cpp"
#endif
#endif
