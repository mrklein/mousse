#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_TEMPLATES_TKINEMATIC_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_TEMPLATES_TKINEMATIC_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::KinematicParcel
// Description
//   Kinematic parcel class with rotational motion (as spherical
//   particles only) and one/two-way coupling with the continuous
//   phase.
//   Sub-models include:
//   - drag
//   - turbulent dispersion
//   - wall interactions

#include "particle.hpp"
#include "iostream.hpp"
#include "auto_ptr.hpp"
#include "interpolation.hpp"
#include "demand_driven_entry.hpp"
#include "mathematical_constants.hpp"
// #include "_particle_force_list.hpp" // TODO


using namespace mousse::constant::mathematical;

namespace mousse {

template<class ParcelType> class KinematicParcel;

// Forward declaration of friend functions
template<class ParcelType>
Ostream& operator<<
(
  Ostream&,
  const KinematicParcel<ParcelType>&
);


template<class ParcelType>
class KinematicParcel
:
  public ParcelType
{
  // Private data
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
    //- Number of particle tracking attempts before we assume that it stalls
    static label maxTrackAttempts;
public:
  //- Class to hold kinematic particle constant properties
  class constantProperties
  {
  protected:
    // Protected data
      //- Constant properties dictionary
      const dictionary dict_;
  private:
    // Private data
      //- Parcel type id - used for post-processing to flag the type
      //  of parcels issued by this cloud
      demandDrivenEntry<label> parcelTypeId_;
      //- Minimum density [kg/m3]
      demandDrivenEntry<scalar> rhoMin_;
      //- Particle density [kg/m3] (constant)
      demandDrivenEntry<scalar> rho0_;
      //- Minimum parcel mass [kg]
      demandDrivenEntry<scalar> minParcelMass_;
  public:
    // Constructors
      //- Null constructor
      constantProperties();
      //- Copy constructor
      constantProperties(const constantProperties& cp);
      //- Construct from dictionary
      constantProperties(const dictionary& parentDict);
    // Member functions
      //- Return const access to the constant properties dictionary
      inline const dictionary& dict() const;
      //- Return const access to the parcel type id
      inline label parcelTypeId() const;
      //- Return const access to the minimum density
      inline scalar rhoMin() const;
      //- Return const access to the particle density
      inline scalar rho0() const;
      //- Return const access to the minimum parcel mass
      inline scalar minParcelMass() const;
  };
  template<class CloudType>
  class TrackingData
  :
    public ParcelType::template TrackingData<CloudType>
  {
  public:
    enum trackPart
    {
      tpVelocityHalfStep,
      tpLinearTrack,
      tpRotationalTrack
    };
  private:
    // Private data
      // Interpolators for continuous phase fields
        //- Density interpolator
        autoPtr<interpolation<scalar>> rhoInterp_;
        //- Velocity interpolator
        autoPtr<interpolation<vector>> UInterp_;
        //- Dynamic viscosity interpolator
        autoPtr<interpolation<scalar>> muInterp_;
      //- Local gravitational or other body-force acceleration
      const vector& g_;
      // label specifying which part of the integration
      // algorithm is taking place
      trackPart part_;
  public:
    // Constructors
      //- Construct from components
      inline TrackingData
      (
        CloudType& cloud,
        trackPart part = tpLinearTrack
      );
    // Member functions
      //- Return conat access to the interpolator for continuous
      //  phase density field
      inline const interpolation<scalar>& rhoInterp() const;
      //- Return conat access to the interpolator for continuous
      //  phase velocity field
      inline const interpolation<vector>& UInterp() const;
      //- Return conat access to the interpolator for continuous
      //  phase dynamic viscosity field
      inline const interpolation<scalar>& muInterp() const;
      // Return const access to the gravitational acceleration vector
      inline const vector& g() const;
      //- Return the part of the tracking operation taking place
      inline trackPart part() const;
      //- Return access to the part of the tracking operation taking place
      inline trackPart& part();
  };
protected:
  // Protected data
    // Parcel properties
      //- Active flag - tracking inactive when active = false
      bool active_;
      //- Parcel type id
      label typeId_;
      //- Number of particles in Parcel
      scalar nParticle_;
      //- Diameter [m]
      scalar d_;
      //- Target diameter [m]
      scalar dTarget_;
      //- Velocity of Parcel [m/s]
      vector U_;
      //- Density [kg/m3]
      scalar rho_;
      //- Age [s]
      scalar age_;
      //- Time spent in turbulent eddy [s]
      scalar tTurb_;
      //- Turbulent velocity fluctuation [m/s]
      vector UTurb_;
    // Cell-based quantities
      //- Density [kg/m3]
      scalar rhoc_;
      //- Velocity [m/s]
      vector Uc_;
      //- Viscosity [Pa.s]
      scalar muc_;
  // Protected Member Functions
    //- Calculate new particle velocity
    template<class TrackData>
    const vector calcVelocity
    (
      TrackData& td,
      const scalar dt,           // timestep
      const label cellI,         // owner cell
      const scalar Re,           // Reynolds number
      const scalar mu,           // local carrier viscosity
      const scalar mass,         // mass
      const vector& Su,          // explicit particle momentum source
      vector& dUTrans,           // momentum transfer to carrier
      scalar& Spu                // linearised drag coefficient
    ) const;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("KinematicParcel");
    //- String representation of properties
    ADD_TO_PROPERTY_LIST
    (
      ParcelType,
      " active"
      + " typeId"
      + " nParticle"
      + " d"
      + " dTarget "
      + " (Ux Uy Uz)"
      + " rho"
      + " age"
      + " tTurb"
      + " (UTurbx UTurby UTurbz)"
    );
  // Constructors
    //- Construct from owner, position, and cloud owner
    //  Other properties initialised as null
    inline KinematicParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI
    );
    //- Construct from components
    inline KinematicParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI,
      const label typeId,
      const scalar nParticle0,
      const scalar d0,
      const scalar dTarget0,
      const vector& torque0,
      const constantProperties& constProps
    );
    //- Construct from Istream
    KinematicParcel
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    );
    //- Construct as a copy
    KinematicParcel(const KinematicParcel& p);
    //- Construct as a copy
    KinematicParcel(const KinematicParcel& p, const polyMesh& mesh);
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>{new KinematicParcel{*this}};
    }
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone(const polyMesh& mesh) const
    {
      return autoPtr<particle>{new KinematicParcel{*this, mesh}};
    }
    //- Factory class to read-construct particles used for
    //  parallel transfer
    class iNew
    {
      const polyMesh& mesh_;
    public:
      iNew(const polyMesh& mesh)
      :
        mesh_{mesh}
      {}
      autoPtr<KinematicParcel<ParcelType>> operator()(Istream& is) const
      {
        return autoPtr<KinematicParcel<ParcelType>>
        {
          new KinematicParcel<ParcelType>{mesh_, is, true}
        };
      }
    };
  // Member Functions
    // Access
      //- Return const access to active flag
      inline bool active() const;
      //- Return const access to type id
      inline label typeId() const;
      //- Return const access to number of particles
      inline scalar nParticle() const;
      //- Return const access to diameter
      inline scalar d() const;
      //- Return const access to target diameter
      inline scalar dTarget() const;
      //- Return const access to velocity
      inline const vector& U() const;
      //- Return const access to density
      inline scalar rho() const;
      //- Return const access to the age
      inline scalar age() const;
      //- Return const access to time spent in turbulent eddy
      inline scalar tTurb() const;
      //- Return const access to turbulent velocity fluctuation
      inline const vector& UTurb() const;
      //- Return const access to carrier density [kg/m3]
      inline scalar rhoc() const;
      //- Return const access to carrier velocity [m/s]
      inline const vector& Uc() const;
      //- Return const access to carrier viscosity [Pa.s]
      inline scalar muc() const;
    // Edit
      //- Return const access to active flag
      inline bool& active();
      //- Return access to type id
      inline label& typeId();
      //- Return access to number of particles
      inline scalar& nParticle();
      //- Return access to diameter
      inline scalar& d();
      //- Return access to target diameter
      inline scalar& dTarget();
      //- Return access to velocity
      inline vector& U();
      //- Return access to density
      inline scalar& rho();
      //- Return access to the age
      inline scalar& age();
      //- Return access to time spent in turbulent eddy
      inline scalar& tTurb();
      //- Return access to turbulent velocity fluctuation
      inline vector& UTurb();
    // Helper functions
      //- Return the index of the face used in the interpolation routine
      inline label faceInterpolation() const;
      //- Cell owner mass
      inline scalar massCell(const label cellI) const;
      //- Particle mass
      inline scalar mass() const;
      //- Particle moment of inertia around diameter axis
      inline scalar momentOfInertia() const;
      //- Particle volume
      inline scalar volume() const;
      //- Particle volume for a given diameter
      inline static scalar volume(const scalar d);
      //- Particle projected area
      inline scalar areaP() const;
      //- Projected area for given diameter
      inline static scalar areaP(const scalar d);
      //- Particle surface area
      inline scalar areaS() const;
      //- Surface area for given diameter
      inline static scalar areaS(const scalar d);
      //- Reynolds number
      inline scalar Re
      (
        const vector& U,        // particle velocity
        const scalar d,         // particle diameter
        const scalar rhoc,      // carrier density
        const scalar muc        // carrier dynamic viscosity
      ) const;
      //- Weber number
      inline scalar We
      (
        const vector& U,        // particle velocity
        const scalar d,         // particle diameter
        const scalar rhoc,      // carrier density
        const scalar sigma      // particle surface tension
      ) const;
      //- Eotvos number
      inline scalar Eo
      (
        const vector& a,        // acceleration
        const scalar d,         // particle diameter
        const scalar sigma      // particle surface tension
      ) const;
    // Main calculation loop
      //- Set cell values
      template<class TrackData>
      void setCellValues
      (
        TrackData& td,
        const scalar dt,
        const label cellI
      );
      //- Correct cell values using latest transfer information
      template<class TrackData>
      void cellValueSourceCorrection
      (
        TrackData& td,
        const scalar dt,
        const label cellI
      );
      //- Update parcel properties over the time interval
      template<class TrackData>
      void calc
      (
        TrackData& td,
        const scalar dt,
        const label cellI
      );
    // Tracking
      //- Move the parcel
      template<class TrackData>
      bool move(TrackData& td, const scalar trackTime);
    // Patch interactions
      //- Overridable function to handle the particle hitting a face
      //  without trackData
      void hitFace(int& td);
      //- Overridable function to handle the particle hitting a face
      template<class TrackData>
      void hitFace(TrackData& td);
      //- Overridable function to handle the particle hitting a patch
      //  Executed before other patch-hitting functions
      template<class TrackData>
      bool hitPatch
      (
        const polyPatch& p,
        TrackData& td,
        const label patchI,
        const scalar trackFraction,
        const tetIndices& tetIs
      );
      //- Overridable function to handle the particle hitting a
      //  processorPatch
      template<class TrackData>
      void hitProcessorPatch
      (
        const processorPolyPatch&,
        TrackData& td
      );
      //- Overridable function to handle the particle hitting a wallPatch
      template<class TrackData>
      void hitWallPatch
      (
        const wallPolyPatch&,
        TrackData& td,
        const tetIndices&
      );
      //- Overridable function to handle the particle hitting a polyPatch
      template<class TrackData>
      void hitPatch
      (
        const polyPatch&,
        TrackData& td
      );
      //- Transform the physical properties of the particle
      //  according to the given transformation tensor
      virtual void transformProperties(const tensor& T);
      //- Transform the physical properties of the particle
      //  according to the given separation vector
      virtual void transformProperties(const vector& separation);
      //- The nearest distance to a wall that the particle can be
      //  in the n direction
      virtual scalar wallImpactDistance(const vector& n) const;
    // I-O
      //- Read
      template<class CloudType>
      static void readFields(CloudType& c);
      //- Write
      template<class CloudType>
      static void writeFields(const CloudType& c);
  // Ostream Operator
    friend Ostream& operator<< <ParcelType>
    (
      Ostream&,
      const KinematicParcel<ParcelType>&
    );
};

}  // namespace mousse


// Constructors 
template<class ParcelType>
inline
mousse::KinematicParcel<ParcelType>::constantProperties::constantProperties()
:
  dict_{dictionary::null},
  parcelTypeId_{dict_, -1},
  rhoMin_{dict_, 0.0},
  rho0_{dict_, 0.0},
  minParcelMass_{dict_, 0.0}
{}


template<class ParcelType>
inline mousse::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
  const constantProperties& cp
)
:
  dict_{cp.dict_},
  parcelTypeId_{cp.parcelTypeId_},
  rhoMin_{cp.rhoMin_},
  rho0_{cp.rho0_},
  minParcelMass_{cp.minParcelMass_}
{}


template<class ParcelType>
inline mousse::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
  const dictionary& parentDict
)
:
  dict_{parentDict.subOrEmptyDict("constantProperties")},
  parcelTypeId_{dict_, "parcelTypeId", -1},
  rhoMin_{dict_, "rhoMin", 1e-15},
  rho0_{dict_, "rho0"},
  minParcelMass_{dict_, "minParcelMass", 1e-15}
{}


template<class ParcelType>
inline mousse::KinematicParcel<ParcelType>::KinematicParcel
(
  const polyMesh& owner,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType{owner, position, cellI, tetFaceI, tetPtI},
  active_{true},
  typeId_{-1},
  nParticle_{0},
  d_{0.0},
  dTarget_{0.0},
  U_{vector::zero},
  rho_{0.0},
  age_{0.0},
  tTurb_{0.0},
  UTurb_{vector::zero},
  rhoc_{0.0},
  Uc_{vector::zero},
  muc_{0.0}
{}


template<class ParcelType>
inline mousse::KinematicParcel<ParcelType>::KinematicParcel
(
  const polyMesh& owner,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label typeId,
  const scalar nParticle0,
  const scalar d0,
  const scalar dTarget0,
  const vector& U0,
  const constantProperties& constProps
)
:
  ParcelType{owner, position, cellI, tetFaceI, tetPtI},
  active_{true},
  typeId_{typeId},
  nParticle_{nParticle0},
  d_{d0},
  dTarget_{dTarget0},
  U_{U0},
  rho_{constProps.rho0()},
  age_{0.0},
  tTurb_{0.0},
  UTurb_{vector::zero},
  rhoc_{0.0},
  Uc_{vector::zero},
  muc_{0.0}
{}


// constantProperties Member Functions
template<class ParcelType>
inline const mousse::dictionary&
mousse::KinematicParcel<ParcelType>::constantProperties::dict() const
{
  return dict_;
}


template<class ParcelType>
inline mousse::label
mousse::KinematicParcel<ParcelType>::constantProperties::parcelTypeId() const
{
  return parcelTypeId_.value();
}


template<class ParcelType>
inline mousse::scalar
mousse::KinematicParcel<ParcelType>::constantProperties::rhoMin() const
{
  return rhoMin_.value();
}


template<class ParcelType>
inline mousse::scalar
mousse::KinematicParcel<ParcelType>::constantProperties::rho0() const
{
  return rho0_.value();
}


template<class ParcelType>
inline mousse::scalar
mousse::KinematicParcel<ParcelType>::constantProperties::minParcelMass() const
{
  return minParcelMass_.value();
}


// KinematicParcel Member Functions 
template<class ParcelType>
inline bool mousse::KinematicParcel<ParcelType>::active() const
{
  return active_;
}


template<class ParcelType>
inline mousse::label mousse::KinematicParcel<ParcelType>::typeId() const
{
  return typeId_;
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::nParticle() const
{
  return nParticle_;
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::d() const
{
  return d_;
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::dTarget() const
{
  return dTarget_;
}


template<class ParcelType>
inline const mousse::vector& mousse::KinematicParcel<ParcelType>::U() const
{
  return U_;
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::rho() const
{
  return rho_;
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::age() const
{
  return age_;
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::tTurb() const
{
  return tTurb_;
}


template<class ParcelType>
inline const mousse::vector& mousse::KinematicParcel<ParcelType>::UTurb() const
{
  return UTurb_;
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::rhoc() const
{
  return rhoc_;
}


template<class ParcelType>
inline const mousse::vector& mousse::KinematicParcel<ParcelType>::Uc() const
{
  return Uc_;
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::muc() const
{
  return muc_;
}


template<class ParcelType>
inline bool& mousse::KinematicParcel<ParcelType>::active()
{
  return active_;
}


template<class ParcelType>
inline mousse::label& mousse::KinematicParcel<ParcelType>::typeId()
{
  return typeId_;
}


template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::nParticle()
{
  return nParticle_;
}


template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::d()
{
  return d_;
}


template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::dTarget()
{
  return dTarget_;
}


template<class ParcelType>
inline mousse::vector& mousse::KinematicParcel<ParcelType>::U()
{
  return U_;
}


template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::rho()
{
  return rho_;
}


template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::age()
{
  return age_;
}


template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::tTurb()
{
  return tTurb_;
}


template<class ParcelType>
inline mousse::vector& mousse::KinematicParcel<ParcelType>::UTurb()
{
  return UTurb_;
}


template<class ParcelType>
inline mousse::label mousse::KinematicParcel<ParcelType>::faceInterpolation() const
{
  // Use volume-based interpolation if dealing with external faces
  if (this->cloud().internalFace(this->face())) {
    return this->face();
  } else {
    return -1;
  }
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::massCell
(
  const label cellI
) const
{
  return rhoc_*this->mesh().cellVolumes()[cellI];
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::mass() const
{
  return rho_*volume();
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::momentOfInertia() const
{
  return 0.1*mass()*sqr(d_);
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::volume() const
{
  return volume(d_);
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::volume(const scalar d)
{
  return pi/6.0*pow3(d);
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::areaP() const
{
  return areaP(d_);
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::areaP(const scalar d)
{
  return 0.25*areaS(d);
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::areaS() const
{
  return areaS(d_);
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::areaS(const scalar d)
{
  return pi*d*d;
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::Re
(
  const vector& U,
  const scalar d,
  const scalar rhoc,
  const scalar muc
) const
{
  return rhoc*mag(U - Uc_)*d/(muc + ROOTVSMALL);
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::We
(
  const vector& U,
  const scalar d,
  const scalar rhoc,
  const scalar sigma
) const
{
  return rhoc*magSqr(U - Uc_)*d/(sigma + ROOTVSMALL);
}


template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::Eo
(
  const vector& a,
  const scalar d,
  const scalar sigma
) const
{
  vector dir = U_/(mag(U_) + ROOTVSMALL);
  return mag(a & dir)*(rho_ - rhoc_)*sqr(d)/(sigma + ROOTVSMALL);
}



template<class ParcelType>
template<class CloudType>
inline mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::TrackingData
(
  CloudType& cloud,
  trackPart part
)
:
  ParcelType::template TrackingData<CloudType>{cloud},
  rhoInterp_
  {
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      cloud.rho()
    )
  },
  UInterp_
  {
    interpolation<vector>::New
    (
      cloud.solution().interpolationSchemes(),
      cloud.U()
    )
  },
  muInterp_
  {
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      cloud.mu()
    )
  },
  g_{cloud.g().value()},
  part_{part}
{}


template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::rhoInterp() const
{
  return rhoInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::vector>&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::UInterp() const
{
  return UInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::muInterp() const
{
  return muInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const mousse::vector&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::g() const
{
  return g_;
}


template<class ParcelType>
template<class CloudType>
inline typename mousse::KinematicParcel<ParcelType>::template
TrackingData<CloudType>::trackPart
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::part() const
{
  return part_;
}


template<class ParcelType>
template<class CloudType>
inline typename mousse::KinematicParcel<ParcelType>::template
TrackingData<CloudType>::trackPart&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::part()
{
  return part_;
}

#include "_kinematic_parcel.ipp"

#endif
