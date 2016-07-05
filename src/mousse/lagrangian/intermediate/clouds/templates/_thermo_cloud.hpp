#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_TTHERMO_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_TTHERMO_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ThermoCloud
// Description
//   Templated base class for thermodynamic cloud
//   - Adds to kinematic cloud
//    - Heat transfer

#include "_kinematic_cloud.hpp"
#include "thermo_cloud.hpp"
#include "slg_thermo.hpp"
#include "physico_chemical_constants.hpp"


using namespace mousse::constant;


namespace mousse {

// Forward declaration of classes
template<class CloudType> class HeatTransferModel;


template<class CloudType>
class ThermoCloud
:
  public CloudType,
  public thermoCloud
{
public:
  // Public typedefs
    //- Type of cloud this cloud was instantiated for
    typedef CloudType cloudType;
    //- Type of parcel the cloud was instantiated for
    typedef typename CloudType::particleType parcelType;
    //- Convenience typedef for this cloud type
    typedef ThermoCloud<CloudType> thermoCloudType;
private:
  // Private data
    //- Cloud copy pointer
    autoPtr<ThermoCloud<CloudType>> cloudCopyPtr_;
protected:
  // Protected data
    //- Thermo parcel constant properties
    typename parcelType::constantProperties constProps_;
    // References to the carrier gas fields
      //- SLG thermodynamics package
      const SLGThermo& thermo_;
      //- Temperature [K]
      const volScalarField& T_;
      //- Pressure [Pa]
      const volScalarField& p_;
    // References to the cloud sub-models
      //- Heat transfer model
      autoPtr<HeatTransferModel<ThermoCloud<CloudType>>>
        heatTransferModel_;
    // Reference to the particle integration schemes
      //- Temperature integration
      autoPtr<scalarIntegrationScheme> TIntegrator_;
    // Modelling options
      //- Include radiation
      Switch radiation_;
      //- Radiation sum of parcel projected areas
      autoPtr<DimensionedField<scalar, volMesh>> radAreaP_;
      //- Radiation sum of parcel temperature^4
      autoPtr<DimensionedField<scalar, volMesh>> radT4_;
      //- Radiation sum of parcel projected areas * temperature^4
      autoPtr<DimensionedField<scalar, volMesh>> radAreaPT4_;
    // Sources
      //- Sensible enthalpy transfer [J/kg]
      autoPtr<DimensionedField<scalar, volMesh>> hsTrans_;
      //- Coefficient for carrier phase hs equation [W/K]
      autoPtr<DimensionedField<scalar, volMesh>> hsCoeff_;
  // Protected Member Functions
    // Initialisation
      //- Set cloud sub-models
      void setModels();
    // Cloud evolution functions
      //- Reset state of cloud
      void cloudReset(ThermoCloud<CloudType>& c);
public:
  // Constructors
    //- Construct given carrier gas fields
    ThermoCloud
    (
      const word& cloudName,
      const volScalarField& rho,
      const volVectorField& U,
      const dimensionedVector& g,
      const SLGThermo& thermo,
      bool readFields = true
    );
    //- Copy constructor with new name
    ThermoCloud(ThermoCloud<CloudType>& c, const word& name);
    //- Copy constructor with new name - creates bare cloud
    ThermoCloud
    (
      const fvMesh& mesh,
      const word& name,
      const ThermoCloud<CloudType>& c
    );
    //- Construct and return clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> clone(const word& name)
    {
      return
        autoPtr<Cloud<parcelType>>
        {
          new ThermoCloud{*this, name}
        };
    }
    //- Construct and return bare clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> cloneBare(const word& name) const
    {
      return
        autoPtr<Cloud<parcelType>>
        {
          new ThermoCloud{this->mesh(), name, *this}
        };
    }
    //- Disallow default bitwise copy construct
    ThermoCloud(const ThermoCloud&) = delete;
    //- Disallow default bitwise assignment
    ThermoCloud& operator=(const ThermoCloud&) = delete;
  //- Destructor
  virtual ~ThermoCloud();
  // Member Functions
    // Access
      //- Return a reference to the cloud copy
      inline const ThermoCloud& cloudCopy() const;
      //- Return the constant properties
      inline const typename parcelType::constantProperties&
        constProps() const;
      //- Return access to the constant properties
      inline typename parcelType::constantProperties& constProps();
      //- Return const access to thermo package
      inline const SLGThermo& thermo() const;
      //- Return const access to the carrier temperature field
      inline const volScalarField& T() const;
      //- Return const access to the carrier prressure field
      inline const volScalarField& p() const;
      // Sub-models
        //- Return reference to heat transfer model
        inline const HeatTransferModel<ThermoCloud<CloudType>>&
          heatTransfer() const;
      // Integration schemes
        //-Return reference to velocity integration
        inline const scalarIntegrationScheme& TIntegrator() const;
      // Modelling options
        //- Radiation flag
        inline bool radiation() const;
        //- Radiation sum of parcel projected areas [m2]
        inline DimensionedField<scalar, volMesh>& radAreaP();
        //- Radiation sum of parcel projected areas [m2]
        inline const DimensionedField<scalar, volMesh>&
          radAreaP() const;
        //- Radiation sum of parcel temperature^4 [K4]
        inline DimensionedField<scalar, volMesh>& radT4();
        //- Radiation sum of parcel temperature^4 [K4]
        inline const DimensionedField<scalar, volMesh>& radT4() const;
        //- Radiation sum of parcel projected area*temperature^4 [m2K4]
        inline DimensionedField<scalar, volMesh>& radAreaPT4();
        //- Radiation sum of parcel temperature^4 [m2K4]
        inline const DimensionedField<scalar, volMesh>&
          radAreaPT4() const;
      // Sources
        // Enthalpy
          //- Sensible enthalpy transfer [J/kg]
          inline DimensionedField<scalar, volMesh>& hsTrans();
          //- Sensible enthalpy transfer [J/kg]
          inline const DimensionedField<scalar, volMesh>&
            hsTrans() const;
          //- Return coefficient for carrier phase hs equation
          inline DimensionedField<scalar, volMesh>& hsCoeff();
          //- Return const coefficient for carrier phase hs equation
          inline const DimensionedField<scalar, volMesh>&
            hsCoeff() const;
          //- Return sensible enthalpy source term [J/kg/m3/s]
          inline tmp<fvScalarMatrix> Sh(volScalarField& hs) const;
        // Radiation - overrides thermoCloud virtual abstract members
          //- Return tmp equivalent particulate emission
          inline tmp<volScalarField> Ep() const;
          //- Return tmp equivalent particulate absorption
          inline tmp<volScalarField> ap() const;
          //- Return tmp equivalent particulate scattering factor
          inline tmp<volScalarField> sigmap() const;
    // Check
      //- Maximum temperature
      inline scalar Tmax() const;
      //- Minimum temperature
      inline scalar Tmin() const;
    // Cloud evolution functions
      //- Set parcel thermo properties
      void setParcelThermoProperties
      (
        parcelType& parcel,
        const scalar lagrangianDt
      );
      //- Check parcel properties
      void checkParcelProperties
      (
        parcelType& parcel,
        const scalar lagrangianDt,
        const bool fullyDescribed
      );
      //- Store the current cloud state
      void storeState();
      //- Reset the current cloud to the previously stored state
      void restoreState();
      //- Reset the cloud source terms
      void resetSourceTerms();
      //- Apply relaxation to (steady state) cloud sources
      void relaxSources(const ThermoCloud<CloudType>& cloudOldTime);
      //- Apply scaling to (transient) cloud sources
      void scaleSources();
      //- Pre-evolve
      void preEvolve();
      //- Evolve the cloud
      void evolve();
    // Mapping
      //- Remap the cells of particles corresponding to the
      //  mesh topology change with a default tracking data object
      virtual void autoMap(const mapPolyMesh&);
    // I-O
      //- Print cloud information
      void info();
};

}  // namespace mousse


// Member Functions 
template<class CloudType>
inline const mousse::ThermoCloud<CloudType>&
mousse::ThermoCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}


template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
mousse::ThermoCloud<CloudType>::constProps() const
{
  return constProps_;
}


template<class CloudType>
inline typename CloudType::particleType::constantProperties&
mousse::ThermoCloud<CloudType>::constProps()
{
  return constProps_;
}


template<class CloudType>
inline const mousse::SLGThermo& mousse::ThermoCloud<CloudType>::thermo() const
{
  return thermo_;
}


template<class CloudType>
inline const mousse::volScalarField& mousse::ThermoCloud<CloudType>::T() const
{
  return T_;
}


template<class CloudType>
inline const mousse::volScalarField& mousse::ThermoCloud<CloudType>::p() const
{
  return p_;
}


template<class CloudType>
inline const mousse::HeatTransferModel<mousse::ThermoCloud<CloudType> >&
mousse::ThermoCloud<CloudType>::heatTransfer() const
{
  return heatTransferModel_;
}


template<class CloudType>
inline const mousse::scalarIntegrationScheme&
mousse::ThermoCloud<CloudType>::TIntegrator() const
{
  return TIntegrator_;
}


template<class CloudType>
inline bool mousse::ThermoCloud<CloudType>::radiation() const
{
  return radiation_;
}


template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radAreaP()
{
  if (!radiation_) {
    FATAL_ERROR_IN
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radAreaP()"
    )
    << "Radiation field requested, but radiation model not active"
    << abort(FatalError);
  }
  return radAreaP_();
}


template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radAreaP() const
{
  if (!radiation_) {
    FATAL_ERROR_IN
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radAreaP()"
    )
    << "Radiation field requested, but radiation model not active"
    << abort(FatalError);
  }
  return radAreaP_();
}


template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radT4()
{
  if (!radiation_) {
    FATAL_ERROR_IN
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radT4()"
    )
    << "Radiation field requested, but radiation model not active"
    << abort(FatalError);
  }
  return radT4_();
}


template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radT4() const
{
  if (!radiation_) {
    FATAL_ERROR_IN
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radT4()"
    )
    << "Radiation field requested, but radiation model not active"
    << abort(FatalError);
  }
  return radT4_();
}


template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radAreaPT4()
{
  if (!radiation_) {
    FATAL_ERROR_IN
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radAreaPT4()"
    )
    << "Radiation field requested, but radiation model not active"
    << abort(FatalError);
  }
  return radAreaPT4_();
}


template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radAreaPT4() const
{
  if (!radiation_) {
    FATAL_ERROR_IN
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radAreaPT4()"
    )
    << "Radiation field requested, but radiation model not active"
    << abort(FatalError);
  }
  return radAreaPT4_();
}


template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::hsTrans()
{
  return hsTrans_();
}


template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::hsTrans() const
{
  return hsTrans_();
}


template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::hsCoeff()
{
  return hsCoeff_();
}


template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::hsCoeff() const
{
  return hsCoeff_();
}


template<class CloudType>
inline mousse::tmp<mousse::fvScalarMatrix>
mousse::ThermoCloud<CloudType>::Sh(volScalarField& hs) const
{
  if (debug) {
    Info << "hsTrans min/max = " << min(hsTrans()).value() << ", "
      << max(hsTrans()).value() << nl
      << "hsCoeff min/max = " << min(hsCoeff()).value() << ", "
      << max(hsCoeff()).value() << endl;
  }
  if (this->solution().coupled()) {
    if (this->solution().semiImplicit("h")) {
      const volScalarField Cp{thermo_.thermo().Cp()};
      const DimensionedField<scalar, volMesh>
        Vdt{this->mesh().V()*this->db().time().deltaT()};
      return
        hsTrans()/Vdt - fvm::SuSp(hsCoeff()/(Cp*Vdt), hs)
        + hsCoeff()/(Cp*Vdt)*hs;
    } else {
      tmp<fvScalarMatrix> tfvm{new fvScalarMatrix{hs, dimEnergy/dimTime}};
      fvScalarMatrix& fvm = tfvm();
      fvm.source() = -hsTrans()/(this->db().time().deltaT());
      return tfvm;
    }
  }
  return tmp<fvScalarMatrix>{new fvScalarMatrix{hs, dimEnergy/dimTime}};
}


template<class CloudType>
inline mousse::tmp<mousse::volScalarField> mousse::ThermoCloud<CloudType>::Ep() const
{
  tmp<volScalarField> tEp
  {
    new volScalarField
    {
      // IOobject
      {
        this->name() + ":radiation:Ep",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      {"zero", dimMass/dimLength/pow3(dimTime), 0.0}
    }
  };
  if (radiation_) {
    scalarField& Ep = tEp().internalField();
    const scalar dt = this->db().time().deltaTValue();
    const scalarField& V = this->mesh().V();
    const scalar epsilon = constProps_.epsilon0();
    const scalarField& sumAreaPT4 = radAreaPT4_->field();
    Ep = sumAreaPT4*epsilon*physicoChemical::sigma.value()/V/dt;
  }
  return tEp;
}


template<class CloudType>
inline mousse::tmp<mousse::volScalarField> mousse::ThermoCloud<CloudType>::ap() const
{
  tmp<volScalarField> tap
  {
    new volScalarField
    {
      {
        this->name() + ":radiation:ap",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      {"zero", dimless/dimLength, 0.0}
    }
  };
  if (radiation_) {
    scalarField& ap = tap().internalField();
    const scalar dt = this->db().time().deltaTValue();
    const scalarField& V = this->mesh().V();
    const scalar epsilon = constProps_.epsilon0();
    const scalarField& sumAreaP = radAreaP_->field();
    ap = sumAreaP*epsilon/V/dt;
  }
  return tap;
}


template<class CloudType>
inline mousse::tmp<mousse::volScalarField>
mousse::ThermoCloud<CloudType>::sigmap() const
{
  tmp<volScalarField> tsigmap
  {
    new volScalarField
    {
      {
        this->name() + ":radiation:sigmap",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      {"zero", dimless/dimLength, 0.0}
    }
  };
  if (radiation_) {
    scalarField& sigmap = tsigmap().internalField();
    const scalar dt = this->db().time().deltaTValue();
    const scalarField& V = this->mesh().V();
    const scalar epsilon = constProps_.epsilon0();
    const scalar f = constProps_.f0();
    const scalarField& sumAreaP = radAreaP_->field();
    sigmap *= sumAreaP*(1.0 - f)*(1.0 - epsilon)/V/dt;
  }
  return tsigmap;
}


template<class CloudType>
inline mousse::scalar mousse::ThermoCloud<CloudType>::Tmax() const
{
  scalar T = -GREAT;
  scalar n = 0;
  FOR_ALL_CONST_ITER(typename ThermoCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    T = max(T, p.T());
    n++;
  }
  reduce(T, maxOp<scalar>());
  reduce(n, sumOp<label>());
  if (n > 0) {
    return T;
  } else {
    return 0.0;
  }
}


template<class CloudType>
inline mousse::scalar mousse::ThermoCloud<CloudType>::Tmin() const
{
  scalar T = GREAT;
  scalar n = 0;
  FOR_ALL_CONST_ITER(typename ThermoCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    T = min(T, p.T());
    n++;
  }
  reduce(T, minOp<scalar>());
  reduce(n, sumOp<label>());
  if (n > 0) {
    return T;
  } else {
    return 0.0;
  }
}

#include "_thermo_cloud.ipp"

#endif
