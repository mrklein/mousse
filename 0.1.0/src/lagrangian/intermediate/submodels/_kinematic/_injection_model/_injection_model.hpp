#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TINJECTION_MODEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TINJECTION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InjectionModel
// Description
//   Templated injection model class.
//   The injection model nominally describes the parcel:
//   - position
//   - diameter
//   - velocity
//   In this case, the fullyDescribed() flag should be set to 0 (false). When
//   the parcel is then added to the cloud, the remaining properties are
//   populated using values supplied in the constant properties.
//   If, however, all of a parcel's properties are described in the model, the
//   fullDescribed() flag should be set to 1 (true).
// SourceFiles
//   _injection_model.cpp
//   _injection_model_new.cpp
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
#include "vector.hpp"
#include "time_data_entry.hpp"
namespace mousse
{
template<class CloudType>
class InjectionModel
:
  public CloudSubModelBase<CloudType>
{
public:
  //- Convenience typedef for parcelType
  typedef typename CloudType::parcelType parcelType;
  // Enumerations
    //- Parcel basis representation options
    //  i.e constant number of particles OR constant mass per parcel
    enum parcelBasis
    {
      pbNumber,
      pbMass,
      pbFixed
    };
protected:
  // Protected data
    // Global injection properties
      //- Start of injection [s]
      scalar SOI_;
      //- Total volume of particles introduced by this injector [m^3]
      //  - scaled to ensure massTotal is achieved
      scalar volumeTotal_;
      //- Total mass to inject [kg]
      scalar massTotal_;
      //- Mass flow rate profile for steady calculations
      TimeDataEntry<scalar> massFlowRate_;
      //- Total mass injected to date [kg]
      scalar massInjected_;
    // Counters
      //- Number of injections counter
      label nInjections_;
      //- Running counter of total number of parcels added
      label parcelsAddedTotal_;
    // Injection properties per Lagrangian time step
      //- Parcel basis enumeration
      parcelBasis parcelBasis_;
      //- nParticle to assign to parcels when the 'fixed' basis
      //  is selected
      scalar nParticleFixed_;
      //- Continuous phase time at start of injection time step [s]
      scalar time0_;
      //- Time at start of injection time step [s]
      scalar timeStep0_;
      //- Volume that should have been injected, but would lead to
      //  less than 1 particle per parcel
      scalar delayedVolume_;
  // Protected Member Functions
    //- Additional flag to identify whether or not injection of parcelI is
    //  permitted
    virtual bool validInjection(const label parcelI) = 0;
    //- Determine properties for next time step/injection interval
    virtual bool prepareForNextTimeStep
    (
      const scalar time,
      label& newParcels,
      scalar& newVolumeFraction
    );
    //- Find the cell that contains the supplied position
    //  Will modify position slightly towards the owner cell centroid to
    //  ensure that it lies in a cell and not edge/face
    virtual bool findCellAtPosition
    (
      label& cellI,
      label& tetFaceI,
      label& tetPtI,
      vector& position,
      bool errorOnNotFound = true
    );
    //- Set number of particles to inject given parcel properties
    virtual scalar setNumberOfParticles
    (
      const label parcels,
      const scalar volumeFraction,
      const scalar diameter,
      const scalar rho
    );
    //- Post injection checks
    virtual void postInjectCheck
    (
      const label parcelsAdded,
      const scalar massAdded
    );
public:
  //- Runtime type information
  TYPE_NAME("injectionModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    InjectionModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelType
    ),
    (dict, owner, modelType)
  );
  // Constructors
    //- Construct null from owner
    InjectionModel(CloudType& owner);
    //- Construct from dictionary
    InjectionModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName,
      const word& modelType
    );
    //- Construct copy
    InjectionModel(const InjectionModel<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~InjectionModel();
  // Selectors
    //- Selector with lookup from dictionary
    static autoPtr<InjectionModel<CloudType> > New
    (
      const dictionary& dict,
      CloudType& owner
    );
    //- Selector with name and type
    static autoPtr<InjectionModel<CloudType> > New
    (
      const dictionary& dict,
      const word& modelName,
      const word& modelType,
      CloudType& owner
    );
  // Member Functions
    // Mapping
      //- Update mesh
      virtual void updateMesh();
    // Global information
      //- Return the start-of-injection time
      inline scalar timeStart() const;
      //- Return the total volume to be injected across the event
      inline scalar volumeTotal() const;
      //- Return mass of particles to introduce
      inline scalar massTotal() const;
      //- Return mass of particles injected (cumulative)
      inline scalar massInjected() const;
      //- Return the end-of-injection time
      virtual scalar timeEnd() const = 0;
      //- Number of parcels to introduce relative to SOI
      virtual label parcelsToInject
      (
        const scalar time0,
        const scalar time1
      ) = 0;
      //- Volume of parcels to introduce relative to SOI
      virtual scalar volumeToInject
      (
        const scalar time0,
        const scalar time1
      ) = 0;
      //- Return the average parcel mass over the injection period
      virtual scalar averageParcelMass();
      // Counters
        //- Return the number of injections
        inline label nInjections() const;
        //- Return the total number parcels added
        inline label parcelsAddedTotal() const;
    // Per-injection event functions
      //- Main injection loop
      template<class TrackData>
      void inject(TrackData& td);
      //- Main injection loop - steady-state
      template<class TrackData>
      void injectSteadyState(TrackData& td, const scalar trackTime);
    // Injection geometry
      //- Set the injection position and owner cell, tetFace and tetPt
      virtual void setPositionAndCell
      (
        const label parcelI,
        const label nParcels,
        const scalar time,
        vector& position,
        label& cellOwner,
        label& tetFaceI,
        label& tetPtI
      ) = 0;
      //- Set the parcel properties
      virtual void setProperties
      (
        const label parcelI,
        const label nParcels,
        const scalar time,
        parcelType& parcel
      ) = 0;
      //- Flag to identify whether model fully describes the parcel
      virtual bool fullyDescribed() const = 0;
    // I-O
      //- Write injection info to stream
      virtual void info(Ostream& os);
};
}  // namespace mousse

#define MAKE_INJECTION_MODEL(CloudType)                                       \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::InjectionModel<kinematicCloudType>,                               \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
    (                                                                         \
      InjectionModel<kinematicCloudType>,                                     \
      dictionary                                                              \
    );                                                                        \
  }


#define MAKE_INJECTION_MODEL_TYPE(SS, CloudType)                              \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::SS<kinematicCloudType>, 0);     \
                                                                              \
  mousse::InjectionModel<kinematicCloudType>::                                \
    adddictionaryConstructorToTable<mousse::SS<kinematicCloudType> >          \
      add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


template<class CloudType>
mousse::scalar mousse::InjectionModel<CloudType>::timeStart() const
{
  return SOI_;
}
template<class CloudType>
mousse::scalar mousse::InjectionModel<CloudType>::volumeTotal() const
{
  return volumeTotal_;
}
template<class CloudType>
mousse::scalar mousse::InjectionModel<CloudType>::massTotal() const
{
  return massTotal_;
}
template<class CloudType>
mousse::scalar mousse::InjectionModel<CloudType>::massInjected() const
{
  return massInjected_;
}
template<class CloudType>
mousse::label mousse::InjectionModel<CloudType>::nInjections() const
{
  return nInjections_;
}
template<class CloudType>
mousse::label mousse::InjectionModel<CloudType>::parcelsAddedTotal() const
{
  return parcelsAddedTotal_;
}

#ifdef NoRepository
#   include "_injection_model.cpp"
#endif
#endif
