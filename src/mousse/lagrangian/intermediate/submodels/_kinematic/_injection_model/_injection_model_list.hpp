#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TINJECTION_MODEL_LIST_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TINJECTION_MODEL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InjectionModelList
// Description
//   List of injection models

#include "ptr_list.hpp"
#include "_injection_model.hpp"


namespace mousse {

template<class CloudType>
class InjectionModelList
:
  public PtrList<InjectionModel<CloudType>>
{
public:
  // Constructors
    //- Construct null from owner
    InjectionModelList(CloudType& owner);
    //- Construct from dictionary and cloud owner
    InjectionModelList(const dictionary& dict, CloudType& owner);
    //- Construct copy
    InjectionModelList(const InjectionModelList<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModelList<CloudType>> clone() const
    {
      return autoPtr<InjectionModelList<CloudType>>
      {
        new InjectionModelList<CloudType>{*this}
      };
    }
  //- Destructor
  virtual ~InjectionModelList();
  // Member Functions
    // Access
      //- Return the minimum start-of-injection time
      scalar timeStart() const;
      //- Return the maximum end-of-injection time
      scalar timeEnd() const;
      //- Volume of parcels to introduce relative to SOI
      scalar volumeToInject(const scalar time0, const scalar time1);
      //- Return the average parcel mass
      scalar averageParcelMass();
    // Edit
      //- Set injector locations when mesh is updated
      void updateMesh();
    // Per-injection event functions
      //- Main injection loop
      template<class TrackData>
      void inject(TrackData& td);
      //- Main injection loop - steady-state
      template<class TrackData>
      void injectSteadyState(TrackData& td, const scalar trackTime);
    // I-O
      //- Write injection info to stream
      virtual void info(Ostream& os);
};

}  // namespace mousse

#include "_injection_model_list.ipp"

#endif
