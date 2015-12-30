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
// SourceFiles
//   _reacting_cloud_i.hpp
//   _reacting_cloud.cpp
#ifndef _reacting_cloud_hpp_
#define _reacting_cloud_hpp_
#include "reacting_cloud.hpp"
namespace mousse
{
// Forward declaration of classes
template<class CloudType>
class CompositionModel;
template<class CloudType>
class PhaseChangeModel;
template<class CloudType>
class ReactingCloud
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
    autoPtr<ReactingCloud<CloudType> > cloudCopyPtr_;
  // Private member functions
    //- Disallow default bitwise copy construct
    ReactingCloud(const ReactingCloud&);
    //- Disallow default bitwise assignment
    void operator=(const ReactingCloud&);
protected:
  // Protected data
    //- Parcel constant properties
    typename parcelType::constantProperties constProps_;
    // References to the cloud sub-models
      //- Reacting composition model
      autoPtr<CompositionModel<ReactingCloud<CloudType> > >
        compositionModel_;
      //- Reacting phase change model
      autoPtr<PhaseChangeModel<ReactingCloud<CloudType> > >
        phaseChangeModel_;
    // Sources
      //- Mass transfer fields - one per carrier phase specie
      PtrList<DimensionedField<scalar, volMesh> > rhoTrans_;
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
    virtual autoPtr<Cloud<parcelType> > clone(const word& name)
    {
      return autoPtr<Cloud<parcelType> >
      (
        new ReactingCloud(*this, name)
      );
    }
    //- Construct and return bare clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType> > cloneBare(const word& name) const
    {
      return autoPtr<Cloud<parcelType> >
      (
        new ReactingCloud(this->mesh(), name, *this)
      );
    }
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
        inline const CompositionModel<ReactingCloud<CloudType> >&
          composition() const;
        //- Return const access to reacting phase change model
        inline const PhaseChangeModel<ReactingCloud<CloudType> >&
          phaseChange() const;
        //- Return reference to reacting phase change model
        inline PhaseChangeModel<ReactingCloud<CloudType> >&
          phaseChange();
      // Sources
        //- Mass
          //- Return reference to mass source for field i
          inline DimensionedField<scalar, volMesh>&
            rhoTrans(const label i);
          //- Return const access to mass source fields
          inline const PtrList<DimensionedField<scalar, volMesh> >&
            rhoTrans() const;
          //- Return reference to mass source fields
          inline PtrList<DimensionedField<scalar, volMesh> >&
            rhoTrans();
          //- Return mass source term for specie i - specie eqn
          inline tmp<fvScalarMatrix> SYi
          (
            const label i,
            volScalarField& Yi
          ) const;
          //- Return tmp mass source for field i - fully explicit
          inline tmp<DimensionedField<scalar, volMesh> >
            Srho(const label i) const;
          //- Return tmp total mass source for carrier phase
          //  - fully explicit
          inline tmp<DimensionedField<scalar, volMesh> > Srho() const;
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
#include "_reacting_cloud_i.hpp"
#ifdef NoRepository
#   include "_reacting_cloud.cpp"
#endif
#endif
