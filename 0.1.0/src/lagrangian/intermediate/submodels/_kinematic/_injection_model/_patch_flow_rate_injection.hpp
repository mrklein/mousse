// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PatchFlowRateInjection
// Description
//   Patch injection
//   - uses patch flow rate to determine concentration and velociy
//   - User specifies
//    - Total mass to inject
//    - Name of patch
//    - Injection duration
//    - Injection target concentration/carrier volume flow rate
//   - Initial parcel velocity given by local flow velocity
//   - Parcel diameters obtained by distribution model
//   - Parcels injected randomly across the patch
// SourceFiles
//   _patch_flow_rate_injection.cpp
#ifndef _patch_flow_rate_injection_hpp_
#define _patch_flow_rate_injection_hpp_
#include "_injection_model.hpp"
#include "patch_injection_base.hpp"
#include "time_data_entry.hpp"
namespace mousse
{
class distributionModel;
template<class CloudType>
class PatchFlowRateInjection
:
  public InjectionModel<CloudType>,
  public patchInjectionBase
{
  // Private data
    //- Name of carrier (mass or volume) flux field
    const word phiName_;
    //- Name of carrier density field
    const word rhoName_;
    //- Injection duration [s]
    scalar duration_;
    //- Concentration profile of particle volume to carrier volume [-]
    const TimeDataEntry<scalar> concentration_;
    //- Parcels to introduce per unit volume flow rate m3 [n/m3]
    const scalar parcelConcentration_;
    //- Parcel size distribution model
    const autoPtr<distributionModels::distributionModel> sizeDistribution_;
public:
  //- Runtime type information
  TypeName("patchFlowRateInjection");
  // Constructors
    //- Construct from dictionary
    PatchFlowRateInjection
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    PatchFlowRateInjection(const PatchFlowRateInjection<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType> > clone() const
    {
      return autoPtr<InjectionModel<CloudType> >
      (
        new PatchFlowRateInjection<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~PatchFlowRateInjection();
  // Member Functions
    //- Inherit updateMesh from patchInjectionBase
    using patchInjectionBase::updateMesh;
    //- Set injector locations when mesh is updated
    virtual void updateMesh();
    //- Return the end-of-injection time
    scalar timeEnd() const;
    //- Return the total volumetric flow rate across the patch [m3/s]
    virtual scalar flowRate() const;
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
#   include "_patch_flow_rate_injection.cpp"
#endif
#endif
