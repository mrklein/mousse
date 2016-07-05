#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TSURFACE_FILM_MODEL_TSURFACE_FILM_MODEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TSURFACE_FILM_MODEL_TSURFACE_FILM_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SurfaceFilmModel
// Description
//   Templated wall surface film model class.

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"


namespace mousse {

// Forward declaration of classes
namespace regionModels {
namespace surfaceFilmModels {

class surfaceFilmModel;

}
}


template<class CloudType>
class SurfaceFilmModel
:
  public CloudSubModelBase<CloudType>
{
protected:
  // Protected data
    //- Convenience typedef to the cloud's parcel type
    typedef typename CloudType::parcelType parcelType;
    //- Gravitational acceleration constant
    const dimensionedVector& g_;
    //- Ejected parcel type label - id assigned to identify parcel for
    //  post-processing. If not specified, defaults to originating cloud
    //  type
    label ejectedParcelType_;
    // Cached injector fields per film patch
      //- Parcel mass / patch face
      scalarList massParcelPatch_;
      //- Parcel diameter / patch face
      scalarList diameterParcelPatch_;
      //- Film velocity / patch face
      List<vector> UFilmPatch_;
      //- Film density / patch face
      scalarList rhoFilmPatch_;
      //- Film height of all film patches / patch face
      scalarListList deltaFilmPatch_;
    // Counters
      //- Number of parcels transferred to the film model
      label nParcelsTransferred_;
      //- Number of parcels injected from the film model
      label nParcelsInjected_;
  // Protected functions
    //- Cache the film fields in preparation for injection
    virtual void cacheFilmFields
    (
      const label filmPatchI,
      const label primaryPatchI,
      const regionModels::surfaceFilmModels::surfaceFilmModel& filmModel
    );
    //- Set the individual parcel properties
    virtual void setParcelProperties
    (
      parcelType& p,
      const label filmFaceI
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("surfaceFilmModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    SurfaceFilmModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    SurfaceFilmModel(CloudType& owner);
    //- Construct from components
    SurfaceFilmModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    SurfaceFilmModel(const SurfaceFilmModel<CloudType>& sfm);
    //- Construct and return a clone
    virtual autoPtr<SurfaceFilmModel<CloudType>> clone() const = 0;
  //- Destructor
  virtual ~SurfaceFilmModel();
  //- Selector
  static autoPtr<SurfaceFilmModel<CloudType>> New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Member Functions
    // Access
      //- Return gravitational acceleration constant
      inline const dimensionedVector& g() const;
      //- Return const access to the number of parcels transferred to the
      //  film model
      inline label nParcelsTransferred() const;
      //- Return non-const access to the number of parcels transferred to
      //  the film model
      inline label& nParcelsTransferred();
      //- Return const access to the number of parcels injected from the
      //  film model
      inline label nParcelsInjected() const;
      //- Return non-const access to the number of parcels injected from
      //  the film model
      inline label& nParcelsInjected();
    // Member Functions
      //- Transfer parcel from cloud to surface film
      //  Returns true if parcel is to be transferred
      virtual bool transferParcel
      (
        parcelType& p,
        const polyPatch& pp,
        bool& keepParticle
      ) = 0;
      //- Inject parcels into the cloud
      template<class TrackData>
      void inject(TrackData& td);
    // I-O
      //- Write surface film info to stream
      virtual void info(Ostream& os);
};

}  // namespace mousse


#define MAKE_SURFACE_FILM_MODEL(CloudType)                                    \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::SurfaceFilmModel<kinematicCloudType>,                             \
    0                                                                         \
  );                                                                          \
  namespace mousse {                                                          \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                    \
  (                                                                           \
    SurfaceFilmModel<kinematicCloudType>,                                     \
    dictionary                                                                \
  );                                                                          \
  }


#define MAKE_SURFACE_FILM_MODEL_TYPE(SS, CloudType)                           \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::SS<kinematicCloudType>, 0);\
                                                                              \
  mousse::SurfaceFilmModel<kinematicCloudType>::                              \
    adddictionaryConstructorToTable<mousse::SS<kinematicCloudType>>          \
      add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// Member Functions 
template<class CloudType>
const mousse::dimensionedVector& mousse::SurfaceFilmModel<CloudType>::g() const
{
  return g_;
}


template<class CloudType>
mousse::label& mousse::SurfaceFilmModel<CloudType>::nParcelsTransferred()
{
  return nParcelsTransferred_;
}


template<class CloudType>
mousse::label mousse::SurfaceFilmModel<CloudType>::nParcelsTransferred() const
{
  return nParcelsTransferred_;
}


template<class CloudType>
mousse::label& mousse::SurfaceFilmModel<CloudType>::nParcelsInjected()
{
  return nParcelsInjected_;
}


template<class CloudType>
mousse::label mousse::SurfaceFilmModel<CloudType>::nParcelsInjected() const
{
  return nParcelsInjected_;
}


#include "_surface_film_model.ipp"

#endif
