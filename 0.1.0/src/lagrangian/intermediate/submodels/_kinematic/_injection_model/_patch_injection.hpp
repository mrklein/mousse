#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TPATCH_INJECTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TPATCH_INJECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PatchInjection
// Description
//   Patch injection
//   - User specifies
//    - Total mass to inject
//    - Name of patch
//    - Injection duration
//    - Initial parcel velocity
//    - Injection volume flow rate
//   - Parcel diameters obtained by distribution model
//   - Parcels injected randomly across the patch
// SourceFiles
//   _patch_injection.cpp
#include "_injection_model.hpp"
#include "patch_injection_base.hpp"
namespace mousse
{
template<class Type>
class TimeDataEntry;
class distributionModel;
template<class CloudType>
class PatchInjection
:
  public InjectionModel<CloudType>,
  public patchInjectionBase
{
  // Private data
    //- Injection duration [s]
    scalar duration_;
    //- Number of parcels to introduce per second []
    const label parcelsPerSecond_;
    //- Initial parcel velocity [m/s]
    const vector U0_;
    //- Flow rate profile relative to SOI []
    const TimeDataEntry<scalar> flowRateProfile_;
    //- Parcel size distribution model
    const autoPtr<distributionModels::distributionModel> sizeDistribution_;
public:
  //- Runtime type information
  TYPE_NAME("patchInjection");
  // Constructors
    //- Construct from dictionary
    PatchInjection
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    PatchInjection(const PatchInjection<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType> > clone() const
    {
      return autoPtr<InjectionModel<CloudType> >
      (
        new PatchInjection<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~PatchInjection();
  // Member Functions
    //- Inherit updateMesh from patchInjectionBase
    using patchInjectionBase::updateMesh;
    //- Set injector locations when mesh is updated
    virtual void updateMesh();
    //- Return the end-of-injection time
    virtual scalar timeEnd() const;
    //- Number of parcels to introduce relative to SOI
    virtual label parcelsToInject(const scalar time0, const scalar time1);
    //- Volume of parcels to introduce relative to SOI
    virtual scalar volumeToInject(const scalar time0, const scalar time1);
    // Injection geometry
      //- Inherit setPositionAndCell from patchInjectionBase
      using patchInjectionBase::setPositionAndCell;
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
      );
      virtual void setProperties
      (
        const label parcelI,
        const label nParcels,
        const scalar time,
        typename CloudType::parcelType& parcel
      );
      //- Flag to identify whether model fully describes the parcel
      virtual bool fullyDescribed() const;
      //- Return flag to identify whether or not injection of parcelI is
      //  permitted
      virtual bool validInjection(const label parcelI);
};
}  // namespace mousse
#ifdef NoRepository
#   include "_patch_injection.cpp"
#endif
#endif
