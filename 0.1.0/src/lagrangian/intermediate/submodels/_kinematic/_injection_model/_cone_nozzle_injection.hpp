#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TCONE_NOZZLE_INJECTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TCONE_NOZZLE_INJECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ConeNozzleInjection
// Description
//   Cone injection
//   - User specifies
//    - time of start of injection
//    - injector position
//    - direction (along injection axis)
//    - parcel flow rate
//    - inner and outer half-cone angles
//   - Parcel diameters obtained by size distribution model
//   - Parcel velocity is calculated as:
//     - Constant velocity
//       U = <specified by user>
//     - Pressure driven velocity
//       U = sqrt(2*(Pinj - Pamb)/rho)
//     - Flow rate and discharge
//       U = V_dot/(A*Cd)

#include "_injection_model.hpp"


namespace mousse {

// Forward declaration of classes
template<class Type> class TimeDataEntry;
class distributionModel;


template<class CloudType>
class ConeNozzleInjection
:
  public InjectionModel<CloudType>
{
public:
  //- Injection method enumeration
  enum injectionMethod
  {
    imPoint,
    imDisc
  };
  //- Flow type enumeration
  enum flowType
  {
    ftConstantVelocity,
    ftPressureDrivenVelocity,
    ftFlowRateAndDischarge
  };
private:
  // Private data
    //- Point/disc injection method
    injectionMethod injectionMethod_;
    //- Flow type
    flowType flowType_;
    //- Outer nozzle diameter [m]
    const scalar outerDiameter_;
    //- Inner nozzle diameter [m]
    const scalar innerDiameter_;
    //- Injection duration [s]
    scalar duration_;
    //- Injector position [m]
    vector position_;
    //- Cell containing injector position []
    label injectorCell_;
    //- Index of tet face for injector cell
    label tetFaceI_;
    //- Index of tet point for injector cell
    label tetPtI_;
    //- Injector direction []
    vector direction_;
    //- Number of parcels to introduce per second []
    const label parcelsPerSecond_;
    //- Flow rate profile relative to SOI []
    const TimeDataEntry<scalar> flowRateProfile_;
    //- Inner half-cone angle relative to SOI [deg]
    const TimeDataEntry<scalar> thetaInner_;
    //- Outer half-cone angle relative to SOI [deg]
    const TimeDataEntry<scalar> thetaOuter_;
    //- Parcel size PDF model
    const autoPtr<distributionModels::distributionModel> sizeDistribution_;
    // Tangential vectors to the direction vector
      //- First tangential vector
      vector tanVec1_;
      //- Second tangential vector
      vector tanVec2_;
      //- Injection vector orthogonal to direction
      vector normal_;
    // Velocity model coefficients
      //- Constant velocity [m/s]
      scalar UMag_;
      //- Discharge coefficient, relative to SOI [m/s]
      TimeDataEntry<scalar> Cd_;
      //- Injection pressure [Pa]
      TimeDataEntry<scalar> Pinj_;
  // Private Member Functions
    //- Set the injection type
    void setInjectionMethod();
    //- Set the injection flow type
    void setFlowType();
public:
  //- Runtime type information
  TYPE_NAME("coneNozzleInjection");
  // Constructors
    //- Construct from dictionary
    ConeNozzleInjection
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    ConeNozzleInjection(const ConeNozzleInjection<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType>> clone() const
    {
      return
        autoPtr<InjectionModel<CloudType>>
        {
          new ConeNozzleInjection<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~ConeNozzleInjection();
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
      //- Set the injection position and owner cell
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

#include "_cone_nozzle_injection.ipp"

#endif
