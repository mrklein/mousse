#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_TKINEMATIC_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_TKINEMATIC_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::KinematicCloud
// Description
//   Templated base class for kinematic cloud
//   - cloud function objects
//   - particle forces, e.g.
//    - buoyancy
//    - drag
//    - pressure gradient
//    - ...
//   - sub-models:
//    - dispersion model
//    - injection model
//    - patch interaction model
//    - stochastic collision model
//    - surface film model

#include "particle.hpp"
#include "_cloud.hpp"
#include "kinematic_cloud.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "cached_random.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "fv_matrices.hpp"
#include "_integration_schemes_fwd.hpp"
#include "cloud_solution.hpp"
#include "_particle_force_list.hpp"
#include "_cloud_function_object_list.hpp"
#include "fvm_sup.hpp"
#include "sortable_list.hpp"


namespace mousse {

// Forward declaration of classes
template<class CloudType> class InjectionModelList;
template<class CloudType> class DispersionModel;
template<class CloudType> class PatchInteractionModel;
template<class CloudType> class SurfaceFilmModel;
template<class CloudType> class StochasticCollisionModel;


template<class CloudType>
class KinematicCloud
:
  public CloudType,
  public kinematicCloud
{
public:
  // Public typedefs
    //- Type of cloud this cloud was instantiated for
    typedef CloudType cloudType;
    //- Type of parcel the cloud was instantiated for
    typedef typename CloudType::particleType parcelType;
    //- Convenience typedef for this cloud type
    typedef KinematicCloud<CloudType> kinematicCloudType;
    //- Force models type
    typedef ParticleForceList<KinematicCloud<CloudType>> forceType;
    //- Function object type
    typedef CloudFunctionObjectList<KinematicCloud<CloudType>>
      functionType;
private:
  // Private data
    //- Cloud copy pointer
    autoPtr<KinematicCloud<CloudType>> cloudCopyPtr_;
protected:
  // Protected data
    //- References to the mesh and time databases
    const fvMesh& mesh_;
    //- Dictionary of particle properties
    IOdictionary particleProperties_;
    //- Dictionary of output properties
    IOdictionary outputProperties_;
    //- Solution properties
    cloudSolution solution_;
    //- Parcel constant properties
    typename parcelType::constantProperties constProps_;
    //- Sub-models dictionary
    const dictionary subModelProperties_;
    //- Random number generator - used by some injection routines
    cachedRandom rndGen_;
    //- Cell occupancy information for each parcel, (demand driven)
    autoPtr<List<DynamicList<parcelType*>>> cellOccupancyPtr_;
    //- Cell length scale
    scalarField cellLengthScale_;
    // References to the carrier gas fields
      //- Density [kg/m3]
      const volScalarField& rho_;
      //- Velocity [m/s]
      const volVectorField& U_;
      //- Dynamic viscosity [Pa.s]
      const volScalarField& mu_;
    // Environmental properties
      //- Gravity
      const dimensionedVector& g_;
      //- Averaged ambient domain pressure
      scalar pAmbient_;
    //- Optional particle forces
    forceType forces_;
    //- Optional cloud function objects
    functionType functions_;
    // References to the cloud sub-models
      //- Injector models
      InjectionModelList<KinematicCloud<CloudType>> injectors_;
      //- Dispersion model
      autoPtr<DispersionModel<KinematicCloud<CloudType>>>
        dispersionModel_;
      //- Patch interaction model
      autoPtr<PatchInteractionModel<KinematicCloud<CloudType>>>
        patchInteractionModel_;
      //- Stochastic collision model
      autoPtr<StochasticCollisionModel<KinematicCloud<CloudType>>>
        stochasticCollisionModel_;
      //- Surface film model
      autoPtr<SurfaceFilmModel<KinematicCloud<CloudType>>>
        surfaceFilmModel_;
    // Reference to the particle integration schemes
      //- Velocity integration
      autoPtr<vectorIntegrationScheme> UIntegrator_;
    // Sources
      //- Momentum
      autoPtr<DimensionedField<vector, volMesh>> UTrans_;
      //- Coefficient for carrier phase U equation
      autoPtr<DimensionedField<scalar, volMesh>> UCoeff_;
    // Initialisation
      //- Set cloud sub-models
      void setModels();
    // Cloud evolution functions
      //- Solve the cloud - calls all evolution functions
      template<class TrackData>
      void solve(TrackData& td);
      //- Build the cellOccupancy
      void buildCellOccupancy();
      //- Update (i.e. build) the cellOccupancy if it has
      //  already been used
      void updateCellOccupancy();
      //- Evolve the cloud
      template<class TrackData>
      void evolveCloud(TrackData& td);
      //- Post-evolve
      void postEvolve();
      //- Reset state of cloud
      void cloudReset(KinematicCloud<CloudType>& c);
public:
  // Constructors
    //- Construct given carrier gas fields
    KinematicCloud
    (
      const word& cloudName,
      const volScalarField& rho,
      const volVectorField& U,
      const volScalarField& mu,
      const dimensionedVector& g,
      bool readFields = true
    );
    //- Copy constructor with new name
    KinematicCloud
    (
      KinematicCloud<CloudType>& c,
      const word& name
    );
    //- Copy constructor with new name - creates bare cloud
    KinematicCloud
    (
      const fvMesh& mesh,
      const word& name,
      const KinematicCloud<CloudType>& c
    );
    //- Construct and return clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> clone(const word& name)
    {
      return
        autoPtr<Cloud<parcelType>>
        {
          new KinematicCloud{*this, name}
        };
    }
    //- Construct and return bare clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> cloneBare(const word& name) const
    {
      return
        autoPtr<Cloud<parcelType>>
        {
          new KinematicCloud{this->mesh(), name, *this}
        };
    }
    //- Disallow default bitwise copy construct
    KinematicCloud(const KinematicCloud&) = delete;
    //- Disallow default bitwise assignment
    KinematicCloud& operator=(const KinematicCloud&) = delete;
  //- Destructor
  virtual ~KinematicCloud();
  // Member Functions
    // Access
      //- Return a reference to the cloud copy
      inline const KinematicCloud& cloudCopy() const;
      //- Switch to specify if particles of the cloud can return
      //  non-zero wall distance values - true for kinematic parcels
      virtual bool hasWallImpactDistance() const;
      // References to the mesh and databases
        //- Return reference to the mesh
        inline const fvMesh& mesh() const;
        //- Return particle properties dictionary
        inline const IOdictionary& particleProperties() const;
        //- Return output properties dictionary
        inline const IOdictionary& outputProperties() const;
        //- Return non-const access to the output properties dictionary
        inline IOdictionary& outputProperties();
        //- Return const access to the solution properties
        inline const cloudSolution& solution() const;
        //- Return access to the solution properties
        inline cloudSolution& solution();
        //- Return the constant properties
        inline const typename parcelType::constantProperties&
          constProps() const;
        //- Return access to the constant properties
        inline typename parcelType::constantProperties& constProps();
          //- Return reference to the sub-models dictionary
        inline const dictionary& subModelProperties() const;
      // Cloud data
        //- Return reference to the random object
        inline cachedRandom& rndGen();
        //- Return the cell occupancy information for each
        //  parcel, non-const access, the caller is
        //  responsible for updating it for its own purposes
        //  if particles are removed or created.
        inline List<DynamicList<parcelType*>>& cellOccupancy();
        //- Return the cell length scale
        inline const scalarField& cellLengthScale() const;
      // References to the carrier gas fields
        //- Return carrier gas velocity
        inline const volVectorField& U() const;
        //- Return carrier gas density
        inline const volScalarField& rho() const;
        //- Return carrier gas dynamic viscosity
        inline const volScalarField& mu() const;
      // Environmental properties
        //- Gravity
        inline const dimensionedVector& g() const;
        //- Return const-access to the ambient pressure
        inline scalar pAmbient() const;
        //- Return reference to the ambient pressure
        inline scalar& pAmbient();
      //- Optional particle forces
//            inline const typename parcelType::forceType& forces() const;
      inline const forceType& forces() const;
      //- Return the optional particle forces
      inline forceType& forces();
      //- Optional cloud function objects
      inline functionType& functions();
      // Sub-models
        //- Return const access to the injection model
        inline const InjectionModelList<KinematicCloud<CloudType>>&
          injectors() const;
        //- Return reference to the injection model
        inline InjectionModelList<KinematicCloud<CloudType>>&
          injectors();
        //- Return const-access to the dispersion model
        inline const DispersionModel<KinematicCloud<CloudType>>&
          dispersion() const;
        //- Return reference to the dispersion model
        inline DispersionModel<KinematicCloud<CloudType>>&
          dispersion();
        //- Return const-access to the patch interaction model
        inline const PatchInteractionModel<KinematicCloud<CloudType>>&
          patchInteraction() const;
        //- Return reference to the patch interaction model
        inline PatchInteractionModel<KinematicCloud<CloudType>>&
          patchInteraction();
        //- Return const-access to the stochastic collision model
        inline const
          StochasticCollisionModel<KinematicCloud<CloudType>>&
          stochasticCollision() const;
        //- Return reference to the stochastic collision model
        inline StochasticCollisionModel<KinematicCloud<CloudType>>&
          stochasticCollision();
        //- Return const-access to the surface film model
        inline const SurfaceFilmModel<KinematicCloud<CloudType>>&
          surfaceFilm() const;
        //- Return reference to the surface film model
        inline SurfaceFilmModel<KinematicCloud<CloudType>>&
          surfaceFilm();
      // Integration schemes
        //-Return reference to velocity integration
        inline const vectorIntegrationScheme& UIntegrator() const;
      // Sources
        // Momentum
          //- Return reference to momentum source
          inline DimensionedField<vector, volMesh>& UTrans();
          //- Return const reference to momentum source
          inline const DimensionedField<vector, volMesh>&
            UTrans() const;
          //- Return coefficient for carrier phase U equation
          inline DimensionedField<scalar, volMesh>& UCoeff();
          //- Return const coefficient for carrier phase U equation
          inline const DimensionedField<scalar, volMesh>&
            UCoeff() const;
          //- Return tmp momentum source term
          inline tmp<fvVectorMatrix> SU(volVectorField& U) const;
    // Check
      //- Total number of parcels
      inline label nParcels() const;
      //- Total mass in system
      inline scalar massInSystem() const;
      //- Total linear momentum of the system
      inline vector linearMomentumOfSystem() const;
      //- Total linear kinetic energy in the system
      inline scalar linearKineticEnergyOfSystem() const;
      //- Total rotational kinetic energy in the system
      inline scalar rotationalKineticEnergyOfSystem() const;
      //- Penetration for fraction [0-1] of the current total mass
      inline scalar penetration(const scalar fraction) const;
      //- Mean diameter Dij
      inline scalar Dij(const label i, const label j) const;
      //- Max diameter
      inline scalar Dmax() const;
      // Fields
        //- Volume swept rate of parcels per cell
        inline const tmp<volScalarField> vDotSweep() const;
        //- Return the particle volume fraction field
        //  Note: for particles belonging to this cloud only
        inline const tmp<volScalarField> theta() const;
        //- Return the particle mass fraction field
        //  Note: for particles belonging to this cloud only
        inline const tmp<volScalarField> alpha() const;
        //- Return the particle effective density field
        //  Note: for particles belonging to this cloud only
        inline const tmp<volScalarField> rhoEff() const;
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
      //- Relax field
      template<class Type>
      void relax
      (
        DimensionedField<Type, volMesh>& field,
        const DimensionedField<Type, volMesh>& field0,
        const word& name
      ) const;
      //- Scale field
      template<class Type>
      void scale
      (
        DimensionedField<Type, volMesh>& field,
        const word& name
      ) const;
      //- Apply relaxation to (steady state) cloud sources
      void relaxSources(const KinematicCloud<CloudType>& cloudOldTime);
      //- Apply scaling to (transient) cloud sources
      void scaleSources();
      //- Pre-evolve
      void preEvolve();
      //- Evolve the cloud
      void evolve();
      //- Particle motion
      template<class TrackData>
      void motion(TrackData& td);
      //- Calculate the patch normal and velocity to interact with,
      //  accounting for patch motion if required.
      void patchData
      (
        const parcelType& p,
        const polyPatch& pp,
        const scalar trackFraction,
        const tetIndices& tetIs,
        vector& normal,
        vector& Up
      ) const;
    // Mapping
      //- Update mesh
      void updateMesh();
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
inline const mousse::KinematicCloud<CloudType>&
mousse::KinematicCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}


template<class CloudType>
inline const mousse::fvMesh& mousse::KinematicCloud<CloudType>::mesh() const
{
  return mesh_;
}


template<class CloudType>
inline const mousse::IOdictionary&
mousse::KinematicCloud<CloudType>::particleProperties() const
{
  return particleProperties_;
}


template<class CloudType>
inline const mousse::IOdictionary&
mousse::KinematicCloud<CloudType>::outputProperties() const
{
  return outputProperties_;
}


template<class CloudType>
inline mousse::IOdictionary& mousse::KinematicCloud<CloudType>::outputProperties()
{
  return outputProperties_;
}


template<class CloudType>
inline const mousse::cloudSolution&
mousse::KinematicCloud<CloudType>::solution() const
{
  return solution_;
}


template<class CloudType>
inline mousse::cloudSolution& mousse::KinematicCloud<CloudType>::solution()
{
  return solution_;
}


template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
mousse::KinematicCloud<CloudType>::constProps() const
{
  return constProps_;
}


template<class CloudType>
inline typename CloudType::particleType::constantProperties&
mousse::KinematicCloud<CloudType>::constProps()
{
  return constProps_;
}


template<class CloudType>
inline const mousse::dictionary&
mousse::KinematicCloud<CloudType>::subModelProperties() const
{
  return subModelProperties_;
}


template<class CloudType>
inline const mousse::volScalarField& mousse::KinematicCloud<CloudType>::rho() const
{
  return rho_;
}


template<class CloudType>
inline const mousse::volVectorField& mousse::KinematicCloud<CloudType>::U() const
{
  return U_;
}


template<class CloudType>
inline const mousse::volScalarField& mousse::KinematicCloud<CloudType>::mu() const
{
  return mu_;
}


template<class CloudType>
inline const mousse::dimensionedVector& mousse::KinematicCloud<CloudType>::g() const
{
  return g_;
}


template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::pAmbient() const
{
  return pAmbient_;
}


template<class CloudType>
inline mousse::scalar& mousse::KinematicCloud<CloudType>::pAmbient()
{
  return pAmbient_;
}


template<class CloudType>
//inline const typename CloudType::parcelType::forceType&
inline const typename mousse::KinematicCloud<CloudType>::forceType&
mousse::KinematicCloud<CloudType>::forces() const
{
  return forces_;
}


template<class CloudType>
inline typename mousse::KinematicCloud<CloudType>::forceType&
mousse::KinematicCloud<CloudType>::forces()
{
  return forces_;
}


template<class CloudType>
inline typename mousse::KinematicCloud<CloudType>::functionType&
mousse::KinematicCloud<CloudType>::functions()
{
  return functions_;
}


template<class CloudType>
inline const mousse::InjectionModelList<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::injectors() const
{
  return injectors_;
}


template<class CloudType>
inline mousse::InjectionModelList<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::injectors()
{
  return injectors_;
}


template<class CloudType>
inline const mousse::DispersionModel<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::dispersion() const
{
  return dispersionModel_;
}


template<class CloudType>
inline mousse::DispersionModel<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::dispersion()
{
  return dispersionModel_();
}


template<class CloudType>
inline const mousse::PatchInteractionModel<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::patchInteraction() const
{
  return patchInteractionModel_;
}


template<class CloudType>
inline mousse::PatchInteractionModel<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::patchInteraction()
{
  return patchInteractionModel_();
}


template<class CloudType>
inline const mousse::StochasticCollisionModel<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::stochasticCollision() const
{
  return stochasticCollisionModel_();
}


template<class CloudType>
inline mousse::StochasticCollisionModel<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::stochasticCollision()
{
  return stochasticCollisionModel_();
}


template<class CloudType>
inline const mousse::SurfaceFilmModel<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::surfaceFilm() const
{
  return surfaceFilmModel_();
}


template<class CloudType>
inline mousse::SurfaceFilmModel<mousse::KinematicCloud<CloudType>>&
mousse::KinematicCloud<CloudType>::surfaceFilm()
{
  return surfaceFilmModel_();
}


template<class CloudType>
inline const mousse::vectorIntegrationScheme&
mousse::KinematicCloud<CloudType>::UIntegrator() const
{
  return UIntegrator_;
}


template<class CloudType>
inline mousse::label mousse::KinematicCloud<CloudType>::nParcels() const
{
  return this->size();
}


template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::massInSystem() const
{
  scalar sysMass = 0.0;
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    sysMass += p.nParticle()*p.mass();
  }
  return sysMass;
}


template<class CloudType>
inline mousse::vector
mousse::KinematicCloud<CloudType>::linearMomentumOfSystem() const
{
  vector linearMomentum{vector::zero};
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    linearMomentum += p.nParticle()*p.mass()*p.U();
  }
  return linearMomentum;
}


template<class CloudType>
inline mousse::scalar
mousse::KinematicCloud<CloudType>::linearKineticEnergyOfSystem() const
{
  scalar linearKineticEnergy = 0.0;
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    linearKineticEnergy += p.nParticle()*0.5*p.mass()*(p.U() & p.U());
  }
  return linearKineticEnergy;
}


template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::Dij
(
  const label i,
  const label j
) const
{
  scalar si = 0.0;
  scalar sj = 0.0;
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    si += p.nParticle()*pow(p.d(), i);
    sj += p.nParticle()*pow(p.d(), j);
  }
  reduce(si, sumOp<scalar>());
  reduce(sj, sumOp<scalar>());
  sj = max(sj, VSMALL);
  return si/sj;
}


template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::Dmax() const
{
  scalar d = -GREAT;
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    d = max(d, p.d());
  }
  reduce(d, maxOp<scalar>());
  return max(0.0, d);
}


template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::penetration
(
  const scalar fraction
) const
{
  if ((fraction < 0) || (fraction > 1)) {
    FATAL_ERROR_IN
    (
      "inline mousse::scalar mousse::KinematicCloud<CloudType>::penetration"
      "("
        "const scalar"
      ") const"
    )
    << "fraction should be in the range 0 < fraction < 1"
    << exit(FatalError);
  }
  scalar distance = 0.0;
  const label nParcel = this->size();
  globalIndex globalParcels{nParcel};
  const label nParcelSum = globalParcels.size();
  if (nParcelSum == 0) {
    return distance;
  }
  // lists of parcels mass and distance from initial injection point
  List<List<scalar>> procMass{Pstream::nProcs()};
  List<List<scalar>> procDist{Pstream::nProcs()};
  List<scalar>& mass = procMass[Pstream::myProcNo()];
  List<scalar>& dist = procDist[Pstream::myProcNo()];
  mass.setSize(nParcel);
  dist.setSize(nParcel);
  label i = 0;
  scalar mSum = 0.0;
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    scalar m = p.nParticle()*p.mass();
    scalar d = mag(p.position() - p.position0());
    mSum += m;
    mass[i] = m;
    dist[i] = d;
    i++;
  }
  // calculate total mass across all processors
  reduce(mSum, sumOp<scalar>());
  Pstream::gatherList(procMass);
  Pstream::gatherList(procDist);
  if (Pstream::master()) {
    // flatten the mass lists
    List<scalar> allMass{nParcelSum, 0.0};
    SortableList<scalar> allDist{nParcelSum, 0.0};
    for (label procI = 0; procI < Pstream::nProcs(); procI++) {
      SubList<scalar>
      {
        allMass,
        globalParcels.localSize(procI),
        globalParcels.offset(procI)
      }.assign(procMass[procI]);
      // flatten the distance list
      SubList<scalar>
      {
        allDist,
        globalParcels.localSize(procI),
        globalParcels.offset(procI)
      }.assign(procDist[procI]);
    }
    // sort allDist distances into ascending order
    // note: allMass masses are left unsorted
    allDist.sort();
    if (nParcelSum > 1) {
      const scalar mLimit = fraction*mSum;
      const labelList& indices = allDist.indices();
      if (mLimit > (mSum - allMass[indices.last()])) {
        distance = allDist.last();
      } else {
        // assuming that 'fraction' is generally closer to 1 than 0,
        // loop through in reverse distance order
        const scalar mThreshold = (1.0 - fraction)*mSum;
        scalar mCurrent = 0.0;
        label i0 = 0;
        FOR_ALL_REVERSE(indices, i) {
          label indI = indices[i];
          mCurrent += allMass[indI];
          if (mCurrent > mThreshold) {
            i0 = i;
            break;
          }
        }
        if (i0 == indices.size() - 1) {
          distance = allDist.last();
        } else {
          // linearly interpolate to determine distance
          scalar alpha = (mCurrent - mThreshold)/allMass[indices[i0]];
          distance = allDist[i0] + alpha*(allDist[i0+1] - allDist[i0]);
        }
      }
    } else {
      distance = allDist.first();
    }
  }
  Pstream::scatter(distance);
  return distance;
}


template<class CloudType>
inline mousse::cachedRandom& mousse::KinematicCloud<CloudType>::rndGen()
{
  return rndGen_;
}


template<class CloudType>
inline mousse::List<mousse::DynamicList<typename CloudType::particleType*>>&
mousse::KinematicCloud<CloudType>::cellOccupancy()
{
  if (cellOccupancyPtr_.empty()) {
    buildCellOccupancy();
  }
  return cellOccupancyPtr_();
}


template<class CloudType>
inline const mousse::scalarField&
mousse::KinematicCloud<CloudType>::cellLengthScale() const
{
  return cellLengthScale_;
}


template<class CloudType>
inline mousse::DimensionedField<mousse::vector, mousse::volMesh>&
mousse::KinematicCloud<CloudType>::UTrans()
{
  return UTrans_();
}


template<class CloudType>
inline const mousse::DimensionedField<mousse::vector, mousse::volMesh>&
mousse::KinematicCloud<CloudType>::UTrans() const
{
  return UTrans_();
}


template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::KinematicCloud<CloudType>::UCoeff()
{
  return UCoeff_();
}


template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::KinematicCloud<CloudType>::UCoeff() const
{
  return UCoeff_();
}


template<class CloudType>
inline mousse::tmp<mousse::fvVectorMatrix>
mousse::KinematicCloud<CloudType>::SU(volVectorField& U) const
{
  if (debug) {
    Info << "UTrans min/max = " << min(UTrans()).value() << ", "
      << max(UTrans()).value() << nl
      << "UCoeff min/max = " << min(UCoeff()).value() << ", "
      << max(UCoeff()).value() << endl;
  }
  if (solution_.coupled()) {
    if (solution_.semiImplicit("U")) {
      const DimensionedField<scalar, volMesh>
        Vdt{mesh_.V()*this->db().time().deltaT()};
      return UTrans()/Vdt - fvm::Sp(UCoeff()/Vdt, U) + UCoeff()/Vdt*U;
    } else {
      tmp<fvVectorMatrix> tfvm{new fvVectorMatrix{U, dimForce}};
      fvVectorMatrix& fvm = tfvm();
      fvm.source() = -UTrans()/(this->db().time().deltaT());
      return tfvm;
    }
  }
  return tmp<fvVectorMatrix>{new fvVectorMatrix{U, dimForce}};
}


template<class CloudType>
inline const mousse::tmp<mousse::volScalarField>
mousse::KinematicCloud<CloudType>::vDotSweep() const
{
  tmp<volScalarField> tvDotSweep
  {
    new volScalarField
    {
      {
        this->name() + ":vDotSweep",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      {"zero", dimless/dimTime, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  volScalarField& vDotSweep = tvDotSweep();
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    const label cellI = p.cell();
    vDotSweep[cellI] += p.nParticle()*p.areaP()*mag(p.U() - U_[cellI]);
  }
  vDotSweep.internalField() /= mesh_.V();
  vDotSweep.correctBoundaryConditions();
  return tvDotSweep;
}


template<class CloudType>
inline const mousse::tmp<mousse::volScalarField>
mousse::KinematicCloud<CloudType>::theta() const
{
  tmp<volScalarField> ttheta
  {
    new volScalarField
    {
      {
        this->name() + ":theta",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      {"zero", dimless, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  volScalarField& theta = ttheta();
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    const label cellI = p.cell();
    theta[cellI] += p.nParticle()*p.volume();
  }
  theta.internalField() /= mesh_.V();
  theta.correctBoundaryConditions();
  return ttheta;
}


template<class CloudType>
inline const mousse::tmp<mousse::volScalarField>
mousse::KinematicCloud<CloudType>::alpha() const
{
  tmp<volScalarField> talpha
  {
    new volScalarField
    {
      {
        this->name() + ":alpha",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      {"zero", dimless, 0.0}
    }
  };
  scalarField& alpha = talpha().internalField();
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    const label cellI = p.cell();
    alpha[cellI] += p.nParticle()*p.mass();
  }
  alpha /= (mesh_.V()*rho_);
  return talpha;
}


template<class CloudType>
inline const mousse::tmp<mousse::volScalarField>
mousse::KinematicCloud<CloudType>::rhoEff() const
{
  tmp<volScalarField> trhoEff
  {
    new volScalarField
    {
      {
        this->name() + ":rhoEff",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      {"zero", dimDensity, 0.0}
    }
  };
  scalarField& rhoEff = trhoEff().internalField();
  FOR_ALL_CONST_ITER(typename KinematicCloud<CloudType>, *this, iter) {
    const parcelType& p = iter();
    const label cellI = p.cell();
    rhoEff[cellI] += p.nParticle()*p.mass();
  }
  rhoEff /= mesh_.V();
  return trhoEff;
}

#include "_kinematic_cloud.ipp"

#endif
