#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TCONE_INJECTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TCONE_INJECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ConeInjection
// Description
//   Multi-point cone injection model
//   - User specifies
//    - time of start of injection
//    - list of injector positions and directions (along injection axes)
//    - number of parcels to inject per injector
//    - parcel velocities
//    - inner and outer half-cone angles
//   - Parcel diameters obtained by distribution model
// SourceFiles
//   _cone_injection.cpp
#include "_injection_model.hpp"
#include "distribution_model.hpp"
#include "vector_list.hpp"
#include "time_data_entry.hpp"
namespace mousse
{
template<class CloudType>
class ConeInjection
:
  public InjectionModel<CloudType>
{
  // Private data
    //- List of position and axis for each injector
    List<Tuple2<vector, vector> > positionAxis_;
    //- List of cell labels corresponding to injector positions
    labelList injectorCells_;
    //- List of tetFace labels corresponding to injector positions
    labelList injectorTetFaces_;
    //- List of tetPt labels corresponding to injector positions
    labelList injectorTetPts_;
    //- Injection duration [s]
    scalar duration_;
    //- Number of parcels to introduce per injector
    const label parcelsPerInjector_;
    //- Flow rate profile relative to SOI []
    const TimeDataEntry<scalar> flowRateProfile_;
    //- Parcel velocity magnitude relative to SOI [m/s]
    const TimeDataEntry<scalar> Umag_;
    //- Inner half-cone angle relative to SOI [deg]
    const TimeDataEntry<scalar> thetaInner_;
    //- Outer half-cone angle relative to SOI [deg]
    const TimeDataEntry<scalar> thetaOuter_;
    //- Parcel size distribution model
    const autoPtr<distributionModels::distributionModel> sizeDistribution_;
    //- Number of parcels per injector already injected
    mutable label nInjected_;
    // Tangential vectors to the direction vector
      //- First tangential vector
      vectorList tanVec1_;
      //- Second tangential vector
      vectorList tanVec2_;
public:
  //- Runtime type information
  TYPE_NAME("coneInjection");
  // Constructors
    //- Construct from dictionary
    ConeInjection
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    ConeInjection(const ConeInjection<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType> > clone() const
    {
      return autoPtr<InjectionModel<CloudType> >
      (
        new ConeInjection<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ConeInjection();
  // Member Functions
    //- Set injector locations when mesh is updated
    virtual void updateMesh();
    //- Return the end-of-injection time
    scalar timeEnd() const;
    //- Number of parcels to introduce relative to SOI
    virtual label parcelsToInject(const scalar time0, const scalar time1);
    //- Volume of parcels to introduce relative to SOI
    virtual scalar volumeToInject(const scalar time0, const scalar time1);
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
      );
      //- Set the parcel properties
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
#   include "_cone_injection.cpp"
#endif
#endif
