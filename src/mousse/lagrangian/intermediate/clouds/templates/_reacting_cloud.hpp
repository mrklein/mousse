#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_TREACTING_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_TREACTING_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReactingCloud
// Description
//   Templated base class for reacting cloud
//   - Adds to thermodynamic cloud
//    - Variable composition (single phase)
//    - Phase change

#include "reacting_cloud.hpp"


namespace mousse {

// Forward declaration of classes
template<class CloudType> class CompositionModel;
template<class CloudType> class PhaseChangeModel;


template<class CloudType> class ReactingCloud
:
  public CloudType,
  public reactingCloud
{
public:
  // Public typedefs
    //- Type of cloud this cloud was instantiated for
    typedef CloudType cloudType;
    //- Type of parcel the cloud was instantiated for
    typedef typename CloudType::particleType parcelType;
    //- Convenience typedef for this cloud type
    typedef ReactingCloud<CloudType> reactingCloudType;
private:
  // Private data
    //- Cloud copy pointer
    autoPtr<ReactingCloud<CloudType>> cloudCopyPtr_;
protected:
  // Protected data
    //- Parcel constant properties
    typename parcelType::constantProperties constProps_;
    // References to the cloud sub-models
      //- Reacting composition model
      autoPtr<CompositionModel<ReactingCloud<CloudType>>>
        compositionModel_;
      //- Reacting phase change model
      autoPtr<PhaseChangeModel<ReactingCloud<CloudType>>>
        phaseChangeModel_;
    // Sources
      //- Mass transfer fields - one per carrier phase specie
      PtrList<DimensionedField<scalar, volMesh>> rhoTrans_;
  // Protected Member Functions
    // New parcel helper functions
      //- Check that size of a composition field is valid
      void checkSuppliedComposition
      (
        const scalarField& YSupplied,
        const scalarField& Y,
        const word& YName
      );
    // Initialisation
      //- Set cloud sub-models
      void setModels();
    // Cloud evolution functions
      //- Reset state of cloud
      void cloudReset(ReactingCloud<CloudType>& c);
public:
  // Constructors
    //- Construct given carrier gas fields
    ReactingCloud
    (
      const word& cloudName,
      const volScalarField& rho,
      const volVectorField& U,
      const dimensionedVector& g,
      const SLGThermo& thermo,
      bool readFields = true
    );
    //- Copy constructor with new name
    ReactingCloud(ReactingCloud<CloudType>& c, const word& name);
    //- Copy constructor with new name - creates bare cloud
    ReactingCloud
    (
      const fvMesh& mesh,
      const word& name,
      const ReactingCloud<CloudType>& c
    );
    //- Construct and return clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> clone(const word& name)
    {
      return
        autoPtr<Cloud<parcelType>>
        {
          new ReactingCloud{*this, name}
        };
    }
    //- Construct and return bare clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> cloneBare(const word& name) const
    {
      return
        autoPtr<Cloud<parcelType>>
        {
          new ReactingCloud{this->mesh(), name, *this}
        };
    }
    //- Disallow default bitwise copy construct
    ReactingCloud(const ReactingCloud&) = delete;
    //- Disallow default bitwise assignment
    ReactingCloud& operator=(const ReactingCloud&) = delete;
  //- Destructor
  virtual ~ReactingCloud();
  // Member Functions
    // Access
      //- Return a reference to the cloud copy
      inline const ReactingCloud& cloudCopy() const;
      //- Return the constant properties
      inline const typename parcelType::constantProperties&
        constProps() const;
      //- Return access to the constant properties
      inline typename parcelType::constantProperties& constProps();
      // Sub-models
        //- Return const access to reacting composition model
        inline const CompositionModel<ReactingCloud<CloudType>>&
          composition() const;
        //- Return const access to reacting phase change model
        inline const PhaseChangeModel<ReactingCloud<CloudType>>&
          phaseChange() const;
        //- Return reference to reacting phase change model
        inline PhaseChangeModel<ReactingCloud<CloudType>>&
          phaseChange();
      // Sources
        //- Mass
          //- Return reference to mass source for field i
          inline DimensionedField<scalar, volMesh>& rhoTrans(const label i);
          //- Return const access to mass source fields
          inline const PtrList<DimensionedField<scalar, volMesh>>&
            rhoTrans() const;
          //- Return reference to mass source fields
          inline PtrList<DimensionedField<scalar, volMesh>>& rhoTrans();
          //- Return mass source term for specie i - specie eqn
          inline tmp<fvScalarMatrix> SYi
          (
            const label i,
            volScalarField& Yi
          ) const;
          //- Return tmp mass source for field i - fully explicit
          inline tmp<DimensionedField<scalar, volMesh>>
            Srho(const label i) const;
          //- Return tmp total mass source for carrier phase
          //  - fully explicit
          inline tmp<DimensionedField<scalar, volMesh>> Srho() const;
          //- Return total mass source term [kg/m3/s]
          inline tmp<fvScalarMatrix> Srho(volScalarField& rho) const;
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
      void relaxSources(const ReactingCloud<CloudType>& cloudOldTime);
      //- Apply scaling to (transient) cloud sources
      void scaleSources();
      //- Evolve the cloud
      void evolve();
    // Mapping
      //- Remap the cells of particles corresponding to the
      //  mesh topology change with a default tracking data object
      virtual void autoMap(const mapPolyMesh&);
    // I-O
      //- Print cloud information
      void info();
      //- Write the field data for the cloud
      virtual void writeFields() const;
};

}  // namespace mousse


// Member Functions 
template<class CloudType>
inline const mousse::ReactingCloud<CloudType>&
mousse::ReactingCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}


template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
mousse::ReactingCloud<CloudType>::constProps() const
{
  return constProps_;
}


template<class CloudType>
inline typename CloudType::particleType::constantProperties&
mousse::ReactingCloud<CloudType>::constProps()
{
  return constProps_;
}


template<class CloudType>
inline const mousse::CompositionModel<mousse::ReactingCloud<CloudType>>&
mousse::ReactingCloud<CloudType>::composition() const
{
  return compositionModel_;
}


template<class CloudType>
inline const mousse::PhaseChangeModel<mousse::ReactingCloud<CloudType>>&
mousse::ReactingCloud<CloudType>::phaseChange() const
{
  return phaseChangeModel_;
}


template<class CloudType>
inline mousse::PhaseChangeModel<mousse::ReactingCloud<CloudType>>&
mousse::ReactingCloud<CloudType>::phaseChange()
{
  return phaseChangeModel_();
}


template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ReactingCloud<CloudType>::rhoTrans(const label i)
{
  return rhoTrans_[i];
}


template<class CloudType>
inline
const mousse::PtrList<mousse::DimensionedField<mousse::scalar, mousse::volMesh>>&
mousse::ReactingCloud<CloudType>::rhoTrans() const
{
  return rhoTrans_;
}


template<class CloudType>
inline mousse::PtrList<mousse::DimensionedField<mousse::scalar, mousse::volMesh>>&
mousse::ReactingCloud<CloudType>::rhoTrans()
{
  return rhoTrans_;
}


template<class CloudType>
inline mousse::tmp<mousse::fvScalarMatrix> mousse::ReactingCloud<CloudType>::SYi
(
  const label i,
  volScalarField& Yi
) const
{
  if (this->solution().coupled()) {
    if (this->solution().semiImplicit("Yi")) {
      tmp<volScalarField> trhoTrans
      {
        new volScalarField
        {
          {
            this->name() + ":rhoTrans",
            this->db().time().timeName(),
            this->db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
          },
          this->mesh(),
          // dimensionedScalar("zero", dimMass/dimTime/dimVolume, 0.0)
          {"zero", dimMass/dimTime/dimVolume, 0.0}
        }
      };
      volScalarField& sourceField = trhoTrans();
      sourceField.internalField() =
        rhoTrans_[i]/(this->db().time().deltaTValue()*this->mesh().V());
      const dimensionedScalar YiSMALL("YiSMALL", dimless, SMALL);
      return
        fvm::Sp(neg(sourceField)*sourceField/(Yi + YiSMALL), Yi)
        + pos(sourceField)*sourceField;
    } else {
      tmp<fvScalarMatrix> tfvm{new fvScalarMatrix{Yi, dimMass/dimTime}};
      fvScalarMatrix& fvm = tfvm();
      fvm.source() = -rhoTrans_[i]/this->db().time().deltaTValue();
      return tfvm;
    }
  }
  return tmp<fvScalarMatrix>{new fvScalarMatrix{Yi, dimMass/dimTime}};
}


template<class CloudType>
inline mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh>>
mousse::ReactingCloud<CloudType>::Srho(const label i) const
{
  tmp<DimensionedField<scalar, volMesh>> tRhoi
  {
    new DimensionedField<scalar, volMesh>
    {
      // IOobject
      {
        this->name() + ":rhoTrans",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      // dimensionedScalar
      {
        "zero",
        rhoTrans_[0].dimensions()/dimTime/dimVolume,
        0.0
      }
    }
  };
  if (this->solution().coupled()) {
    scalarField& rhoi = tRhoi();
    rhoi = rhoTrans_[i]/(this->db().time().deltaTValue()*this->mesh().V());
  }
  return tRhoi;
}


template<class CloudType>
inline mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh>>
mousse::ReactingCloud<CloudType>::Srho() const
{
  tmp<DimensionedField<scalar, volMesh>> trhoTrans
  {
    new DimensionedField<scalar, volMesh>
    {
      {
        this->name() + ":rhoTrans",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      {"zero", rhoTrans_[0].dimensions()/dimTime/dimVolume, 0.0}
    }
  };
  if (this->solution().coupled()) {
    scalarField& sourceField = trhoTrans();
    FOR_ALL(rhoTrans_, i) {
      sourceField += rhoTrans_[i];
    }
    sourceField /= this->db().time().deltaTValue()*this->mesh().V();
  }
  return trhoTrans;
}


template<class CloudType>
inline mousse::tmp<mousse::fvScalarMatrix>
mousse::ReactingCloud<CloudType>::Srho(volScalarField& rho) const
{
  if (this->solution().coupled()) {
    tmp<volScalarField> trhoTrans
    {
      new volScalarField
      {
        {
          this->name() + ":rhoTrans",
          this->db().time().timeName(),
          this->db(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        this->mesh(),
        {"zero", dimMass/dimTime/dimVolume, 0.0}
      }
    };
    scalarField& sourceField = trhoTrans();
    if (this->solution().semiImplicit("rho")) {
      FOR_ALL(rhoTrans_, i) {
        sourceField += rhoTrans_[i];
      }
      sourceField /= this->db().time().deltaTValue()*this->mesh().V();
      return fvm::SuSp(trhoTrans()/rho, rho);
    } else {
      tmp<fvScalarMatrix> tfvm{new fvScalarMatrix{rho, dimMass/dimTime}};
      fvScalarMatrix& fvm = tfvm();
      FOR_ALL(rhoTrans_, i) {
        sourceField += rhoTrans_[i];
      }
      fvm.source() = -trhoTrans()/this->db().time().deltaT();
      return tfvm;
    }
  }
  return tmp<fvScalarMatrix>{new fvScalarMatrix{rho, dimMass/dimTime}};
}

#include "_reacting_cloud.ipp"

#endif

