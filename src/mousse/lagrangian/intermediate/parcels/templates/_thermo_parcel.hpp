#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_TEMPLATES_TTHERMO_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_TEMPLATES_TTHERMO_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ThermoParcel
// Description
//   Thermodynamic parcel class with one/two-way coupling with the continuous
//   phase. Includes Kinematic parcel sub-models, plus:
//   - heat transfer

#include "particle.hpp"
#include "slg_thermo.hpp"
#include "demand_driven_entry.hpp"


namespace mousse {

template<class ParcelType> class ThermoParcel;

template<class ParcelType>
Ostream& operator<<
(
  Ostream&,
  const ThermoParcel<ParcelType>&
);


template<class ParcelType>
class ThermoParcel
:
  public ParcelType
{
  // Private data
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
public:
  //- Class to hold thermo particle constant properties
  class constantProperties
  :
    public ParcelType::constantProperties
  {
    // Private data
      //- Particle initial temperature [K]
      demandDrivenEntry<scalar> T0_;
      //- Minimum temperature [K]
      demandDrivenEntry<scalar> TMin_;
      //- Maximum temperature [K]
      demandDrivenEntry<scalar> TMax_;
      //- Particle specific heat capacity [J/(kg.K)]
      demandDrivenEntry<scalar> Cp0_;
      //- Particle emissivity [] (radiation)
      demandDrivenEntry<scalar> epsilon0_;
      //- Particle scattering factor [] (radiation)
      demandDrivenEntry<scalar> f0_;
  public:
    // Constructors
      //- Null constructor
      constantProperties();
      //- Copy constructor
      constantProperties(const constantProperties& cp);
      //- Construct from dictionary
      constantProperties(const dictionary& parentDict);
    // Member functions
      // Access
        //- Return const access to the particle initial temperature [K]
        inline scalar T0() const;
        //- Return const access to minimum temperature [K]
        inline scalar TMin() const;
        //- Return const access to maximum temperature [K]
        inline scalar TMax() const;
        //- Set the maximum temperature [K]
        inline void setTMax(const scalar TMax);
        //- Return const access to the particle specific heat capacity
        //  [J/(kg.K)]
        inline scalar Cp0() const;
        //- Return const access to the particle emissivity []
        //  Active for radiation only
        inline scalar epsilon0() const;
        //- Return const access to the particle scattering factor []
        //  Active for radiation only
        inline scalar f0() const;
  };
  template<class CloudType>
  class TrackingData
  :
    public ParcelType::template TrackingData<CloudType>
  {
  private:
    // Private data
      //- Local copy of carrier specific heat field
      //  Cp not stored on carrier thermo, but returned as tmp<...>
      const volScalarField Cp_;
      //- Local copy of carrier thermal conductivity field
      //  kappa not stored on carrier thermo, but returned as tmp<...>
      const volScalarField kappa_;
      // Interpolators for continuous phase fields
        //- Temperature field interpolator
        autoPtr<interpolation<scalar> > TInterp_;
        //- Specific heat capacity field interpolator
        autoPtr<interpolation<scalar> > CpInterp_;
        //- Thermal conductivity field interpolator
        autoPtr<interpolation<scalar> > kappaInterp_;
        //- Radiation field interpolator
        autoPtr<interpolation<scalar> > GInterp_;
  public:
    typedef typename ParcelType::template TrackingData<CloudType>::trackPart
      trackPart;
    // Constructors
      //- Construct from components
      inline TrackingData
      (
        CloudType& cloud,
        trackPart part = ParcelType::template
          TrackingData<CloudType>::tpLinearTrack
      );
    // Member functions
      //- Return access to the locally stored carrier Cp field
      inline const volScalarField& Cp() const;
      //- Return access to the locally stored carrier kappa field
      inline const volScalarField& kappa() const;
      //- Return const access to the interpolator for continuous
      //  phase temperature field
      inline const interpolation<scalar>& TInterp() const;
      //- Return const access to the interpolator for continuous
      //  phase specific heat capacity field
      inline const interpolation<scalar>& CpInterp() const;
      //- Return const access to the interpolator for continuous
      //  phase thermal conductivity field
      inline const interpolation<scalar>& kappaInterp() const;
      //- Return const access to the interpolator for continuous
      //  radiation field
      inline const interpolation<scalar>& GInterp() const;
  };
protected:
  // Protected data
    // Parcel properties
      //- Temperature [K]
      scalar T_;
      //- Specific heat capacity [J/(kg.K)]
      scalar Cp_;
    // Cell-based quantities
      //- Temperature [K]
      scalar Tc_;
      //- Specific heat capacity [J/(kg.K)]
      scalar Cpc_;
  // Protected Member Functions
    //- Calculate new particle temperature
    template<class TrackData>
    scalar calcHeatTransfer
    (
      TrackData& td,
      const scalar dt,           // timestep
      const label cellI,         // owner cell
      const scalar Re,           // Reynolds number
      const scalar Pr,           // Prandtl number - surface
      const scalar kappa,        // Thermal conductivity - surface
      const scalar NCpW,         // Sum of N*Cp*W of emission species
      const scalar Sh,           // explicit particle enthalpy source
      scalar& dhsTrans,          // sensible enthalpy transfer to carrier
      scalar& Sph                // linearised heat transfer coefficient
    );
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("ThermoParcel");
    //- String representation of properties
    ADD_TO_PROPERTY_LIST
    (
      ParcelType,
      " T"
      + " Cp"
    );
  // Constructors
    //- Construct from owner, position, and cloud owner
    //  Other properties initialised as null
    inline ThermoParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI
    );
    //- Construct from components
    inline ThermoParcel
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
      const vector& U0,
      const vector& f0,
      const vector& angularMomentum0,
      const vector& torque0,
      const constantProperties& constProps
    );
    //- Construct from Istream
    ThermoParcel
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    );
    //- Construct as a copy
    ThermoParcel(const ThermoParcel& p);
    //- Construct as a copy
    ThermoParcel(const ThermoParcel& p, const polyMesh& mesh);
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>{new ThermoParcel{*this}};
    }
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone(const polyMesh& mesh) const
    {
      return autoPtr<particle>{new ThermoParcel{*this, mesh}};
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
      autoPtr<ThermoParcel<ParcelType>> operator()(Istream& is) const
      {
        return
          autoPtr<ThermoParcel<ParcelType>>
          {
            new ThermoParcel<ParcelType>{mesh_, is, true}
          };
      }
    };
  // Member Functions
    // Access
      //- Return const access to temperature
      inline scalar T() const;
      //- Return const access to specific heat capacity
      inline scalar Cp() const;
      //- Return the parcel sensible enthalpy
      inline scalar hs() const;
      //- Return const access to carrier temperature
      inline scalar Tc() const;
      //- Return const access to carrier specific heat capacity
      inline scalar Cpc() const;
    // Edit
      //- Return access to temperature
      inline scalar& T();
      //- Return access to specific heat capacity
      inline scalar& Cp();
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
      //- Calculate surface thermo properties
      template<class TrackData>
      void calcSurfaceValues
      (
        TrackData& td,
        const label cellI,
        const scalar T,
        scalar& Ts,
        scalar& rhos,
        scalar& mus,
        scalar& Pr,
        scalar& kappas
      ) const;
      //- Update parcel properties over the time interval
      template<class TrackData>
      void calc
      (
        TrackData& td,
        const scalar dt,
        const label cellI
      );
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
      const ThermoParcel<ParcelType>&
    );
};

}  // namespace mousse


// Constructors 
template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::constantProperties::constantProperties()
:
  ParcelType::constantProperties{},
  T0_{this->dict_, 0.0},
  TMin_{this->dict_, 0.0},
  TMax_{this->dict_, VGREAT},
  Cp0_{this->dict_, 0.0},
  epsilon0_{this->dict_, 0.0},
  f0_{this->dict_, 0.0}
{}


template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::constantProperties::constantProperties
(
  const constantProperties& cp
)
:
  ParcelType::constantProperties{cp},
  T0_{cp.T0_},
  TMin_{cp.TMin_},
  TMax_{cp.TMax_},
  Cp0_{cp.Cp0_},
  epsilon0_{cp.epsilon0_},
  f0_{cp.f0_}
{}


template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::constantProperties::constantProperties
(
  const dictionary& parentDict
)
:
  ParcelType::constantProperties{parentDict},
  T0_{this->dict_, "T0"},
  TMin_{this->dict_, "TMin", 200.0},
  TMax_{this->dict_, "TMax", 5000.0},
  Cp0_{this->dict_, "Cp0"},
  epsilon0_{this->dict_, "epsilon0"},
  f0_{this->dict_, "f0"}
{}


template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::ThermoParcel
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType{mesh, position, cellI, tetFaceI, tetPtI},
  T_{0.0},
  Cp_{0.0},
  Tc_{0.0},
  Cpc_{0.0}
{}


template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::ThermoParcel
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
  const vector& U0,
  const vector& f0,
  const vector& angularMomentum0,
  const vector& torque0,
  const constantProperties& constProps
)
:
  ParcelType
  {
    mesh,
    position,
    cellI,
    tetFaceI,
    tetPtI,
    typeId,
    nParticle0,
    d0,
    dTarget0,
    U0,
    f0,
    angularMomentum0,
    torque0,
    constProps
  },
  T_{constProps.T0()},
  Cp_{constProps.Cp0()},
  Tc_{0.0},
  Cpc_{0.0}
{}


// constantProperties Member Functions
template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::T0() const
{
  return T0_.value();
}


template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::TMin() const
{
  return TMin_.value();
}


template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::TMax() const
{
  return TMax_.value();
}


template<class ParcelType>
inline void
mousse::ThermoParcel<ParcelType>::constantProperties::setTMax(const scalar TMax)
{
  TMax_.setValue(TMax);
}


template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::Cp0() const
{
  return Cp0_.value();
}


template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::epsilon0() const
{
  return epsilon0_.value();
}


template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::f0() const
{
  return f0_.value();
}


// ThermoParcel Member Functions
template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::T() const
{
  return T_;
}


template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::Cp() const
{
  return Cp_;
}


template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::hs() const
{
  return Cp_*(T_ - 298.15);
}


template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::Tc() const
{
  return Tc_;
}


template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::Cpc() const
{
  return Cpc_;
}


template<class ParcelType>
inline mousse::scalar& mousse::ThermoParcel<ParcelType>::T()
{
  return T_;
}


template<class ParcelType>
inline mousse::scalar& mousse::ThermoParcel<ParcelType>::Cp()
{
  return Cp_;
}



template<class ParcelType>
template<class CloudType>
inline mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::TrackingData
(
  CloudType& cloud,
  trackPart part
)
:
  ParcelType::template TrackingData<CloudType>{cloud, part},
  Cp_{cloud.thermo().thermo().Cp()},
  kappa_{cloud.thermo().thermo().kappa()},
  TInterp_
  {
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      cloud.T()
    )
  },
  CpInterp_
  {
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      Cp_
    )
  },
  kappaInterp_
  {
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      kappa_
    )
  },
  GInterp_{nullptr}
{
  if (cloud.radiation()) {
    GInterp_.reset
    (
      interpolation<scalar>::New
      (
        cloud.solution().interpolationSchemes(),
        cloud.mesh().objectRegistry::template
          lookupObject<volScalarField>("G")
      ).ptr()
    );
  }
}


template<class ParcelType>
template<class CloudType>
inline const mousse::volScalarField&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::Cp() const
{
  return Cp_;
}


template<class ParcelType>
template<class CloudType>
inline const mousse::volScalarField&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::kappa() const
{
  return kappa_;
}


template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::TInterp() const
{
  return TInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::CpInterp() const
{
  return CpInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::kappaInterp() const
{
  return kappaInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::GInterp() const
{
  if (!GInterp_.valid()) {
    FATAL_ERROR_IN
    (
      "inline const mousse::interpolation<mousse::scalar>&"
      "mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::"
      "GInterp() const"
    )
    << "Radiation G interpolation object not set"
    << abort(FatalError);
  }
  return GInterp_();
}

#include "_thermo_parcel.ipp"

#endif
