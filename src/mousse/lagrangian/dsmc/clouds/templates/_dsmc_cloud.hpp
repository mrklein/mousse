#ifndef LAGRANGIAN_DSMC_CLOUDS_TEMPLATES_TDSMC_CLOUD_HPP_
#define LAGRANGIAN_DSMC_CLOUDS_TEMPLATES_TDSMC_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DSMCCloud
// Description
//   Templated base class for dsmc cloud

#include "_cloud.hpp"
#include "dsmc_base_cloud.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "random.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "scalar_io_field.hpp"
#include "constants.hpp"


using namespace mousse::constant;
using namespace mousse::constant::mathematical;


namespace mousse {

// Forward declaration of classes
template<class CloudType> class BinaryCollisionModel;
template<class CloudType> class WallInteractionModel;
template<class CloudType> class InflowBoundaryModel;


template<class ParcelType>
class DSMCCloud
:
  public Cloud<ParcelType>,
  public DSMCBaseCloud
{
  // Private data
    //- Cloud type - used to set the name of the parcel properties
    //  dictionary by appending "Properties"
    const word cloudName_;
    //- References to the mesh and time databases
    const fvMesh& mesh_;
    //- Dictionary of particle properties
    IOdictionary particleProperties_;
    //- A list of unique instances of molecule types in the
    //  simulation.  The position of an entry in the list maps to
    //  the label identifying the typeId, i.e. where typeIdList_ =
    //  (N2 O2 CO2) N2 has typeId label = 0, O2 = 1, CO2 = 2.
    List<word> typeIdList_;
    //- Number of real atoms/molecules represented by a parcel
    scalar nParticle_;
    //- A data structure holding which particles are in which cell
    List<DynamicList<ParcelType*> > cellOccupancy_;
    //- A field holding the value of (sigmaT * cR)max for each
    //  cell (see Bird p220). Initialised with the parcels,
    //  updated as required, and read in on start/restart.
    volScalarField sigmaTcRMax_;
    //- A field holding the remainder from the previous collision selections
    DimensionedField<scalar, volMesh> collisionSelectionRemainder_;
    //- Heat flux at surface field
    volScalarField q_;
    //- Force density at surface field
    volVectorField fD_;
    //- Number density field
    volScalarField rhoN_;
    //- Mass density field
    volScalarField rhoM_;
    //- Dsmc particle density field
    volScalarField dsmcRhoN_;
    //- Linear kinetic energy density field
    volScalarField linearKE_;
    //- Internal energy density field
    volScalarField internalE_;
    // Internal degree of freedom density field
    volScalarField iDof_;
    //- Momentum density field
    volVectorField momentum_;
    //- Parcel constant properties - one for each type
    List<typename ParcelType::constantProperties> constProps_;
    //- Random number generator
    Random rndGen_;
    // boundary value fields
      //- Boundary temperature
      volScalarField boundaryT_;
      //- Boundary velocity
      volVectorField boundaryU_;
    // References to the cloud sub-models
      //- Binary collision model
      autoPtr<BinaryCollisionModel<DSMCCloud<ParcelType> > >
        binaryCollisionModel_;
      //- Wall interaction model
      autoPtr<WallInteractionModel<DSMCCloud<ParcelType> > >
        wallInteractionModel_;
      //- Inflow boundary model
      autoPtr<InflowBoundaryModel<DSMCCloud<ParcelType> > >
        inflowBoundaryModel_;
  // Private Member Functions
    //- Build the constant properties for all of the species
    void buildConstProps();
    //- Record which particles are in which cell
    void buildCellOccupancy();
    //- Initialise the system
    void initialise(const IOdictionary& dsmcInitialiseDict);
    //- Calculate collisions between molecules
    void collisions();
    //- Reset the data accumulation field values to zero
    void resetFields();
    //- Calculate the volume field data
    void calculateFields();
public:
  // Constructors
    //- Construct given name and mesh, will read Parcels and fields from
    //  file
    DSMCCloud
    (
      const word& cloudName,
      const fvMesh& mesh,
      bool readFields = true
    );
    //- Construct given name, mesh and initialisation dictionary.
    DSMCCloud
    (
      const word& cloudName,
      const fvMesh& mesh,
      const IOdictionary& dsmcInitialiseDict
    );
    //- Disallow default bitwise copy construct
    DSMCCloud(const DSMCCloud&) = delete;
    //- Disallow default bitwise assignment
    DSMCCloud& operator=(const DSMCCloud&) = delete;
  //- Destructor
  virtual ~DSMCCloud();
  //- Type of parcel the cloud was instantiated for
  typedef ParcelType parcelType;
  // Member Functions
    // Access
      // References to the mesh and databases
        //- Return the cloud type
        inline const word& cloudName() const;
        //- Return refernce to the mesh
        inline const fvMesh& mesh() const;
      // References to the dsmc specific data
        //- Return particle properties dictionary
        inline const IOdictionary& particleProperties() const;
        //- Return the idList
        inline const List<word>& typeIdList() const;
        //- Return the number of real particles represented by one
        //  parcel
        inline scalar nParticle() const;
        //- Return the cell occupancy addressing
        inline const List<DynamicList<ParcelType*> >&
          cellOccupancy() const;
        //- Return the sigmaTcRMax field.  non-const access to allow
        // updating.
        inline volScalarField& sigmaTcRMax();
        //- Return the collision selection remainder field.  non-const
        // access to allow updating.
        inline scalarField& collisionSelectionRemainder();
        //- Return all of the constant properties
        inline const List<typename ParcelType::constantProperties>&
          constProps() const;
        //- Return the constant properties of the given typeId
        inline const typename ParcelType::constantProperties&
          constProps(label typeId) const;
        //- Return refernce to the random object
        inline Random& rndGen();
      // References to the boundary fields for surface data collection
        //- Return non-const heat flux boundary field reference
        inline volScalarField::GeometricBoundaryField& qBF();
        //- Return non-const force density at boundary field reference
        inline volVectorField::GeometricBoundaryField& fDBF();
        //- Return non-const number density boundary field reference
        inline volScalarField::GeometricBoundaryField& rhoNBF();
        //- Return non-const mass density boundary field reference
        inline volScalarField::GeometricBoundaryField& rhoMBF();
        //- Return non-const linear kinetic energy density boundary
        //  field reference
        inline volScalarField::GeometricBoundaryField& linearKEBF();
        //- Return non-const internal energy density boundary field
        // reference
        inline volScalarField::GeometricBoundaryField& internalEBF();
        //- Return non-const internal degree of freedom density boundary
        //  field reference
        inline volScalarField::GeometricBoundaryField& iDofBF();
        //- Return non-const momentum density boundary field reference
        inline volVectorField::GeometricBoundaryField& momentumBF();
      // References to the macroscopic fields
        //- Return macroscopic temperature
        inline const volScalarField& boundaryT() const;
        //- Return macroscopic velocity
        inline const volVectorField& boundaryU() const;
        //- Return heat flux at surface field
        inline const volScalarField& q() const;
        //- Return force density at surface field
        inline const volVectorField& fD() const;
        //- Return the real particle number density field
        inline const volScalarField& rhoN() const;
        //- Return the particle mass density field
        inline const volScalarField& rhoM() const;
        //- Return the field of number of DSMC particles
        inline const volScalarField& dsmcRhoN() const;
        //- Return the total linear kinetic energy (translational and
        // thermal density field
        inline const volScalarField& linearKE() const;
        //- Return the internal energy density field
        inline const volScalarField& internalE() const;
        //- Return the average internal degrees of freedom  field
        inline const volScalarField& iDof() const;
        //- Return the momentum density field
        inline const volVectorField& momentum() const;
      // Kinetic theory helper functions
        //- Generate a random velocity sampled from the Maxwellian speed
        // distribution
        vector equipartitionLinearVelocity
        (
          scalar temperature,
          scalar mass
        );
        //- Generate a random internal energy, sampled from the
        // equilibrium distribution (Bird eqn 11.22 and 11.23 and
        // adapting code from DSMC3.FOR)
        scalar equipartitionInternalEnergy
        (
          scalar temperature,
          direction internalDegreesOfFreedom
        );
        // From the Maxwellian distribution:
        //- Average particle speed
        inline scalar maxwellianAverageSpeed
        (
          scalar temperature,
          scalar mass
        ) const;
        inline scalarField maxwellianAverageSpeed
        (
          scalarField temperature,
          scalar mass
        ) const;
        //- RMS particle speed
        inline scalar maxwellianRMSSpeed
        (
          scalar temperature,
          scalar mass
        ) const;
        inline scalarField maxwellianRMSSpeed
        (
          scalarField temperature,
          scalar mass
        ) const;
        //- Most probable speed
        inline scalar maxwellianMostProbableSpeed
        (
          scalar temperature,
          scalar mass
        ) const;
        inline scalarField maxwellianMostProbableSpeed
        (
          scalarField temperature,
          scalar mass
        ) const;
      // Sub-models
        //- Return reference to binary elastic collision model
        inline const BinaryCollisionModel<DSMCCloud<ParcelType> >&
          binaryCollision() const;
        //- Return non-const reference to binary elastic collision model
        inline BinaryCollisionModel<DSMCCloud<ParcelType> >&
          binaryCollision();
        //- Return reference to wall interaction model
        inline const WallInteractionModel<DSMCCloud<ParcelType> >&
          wallInteraction() const;
        //- Return non-const reference to wall interaction model
        inline WallInteractionModel<DSMCCloud<ParcelType> >&
          wallInteraction();
        //- Return reference to wall interaction model
        inline const InflowBoundaryModel<DSMCCloud<ParcelType> >&
          inflowBoundary() const;
        //- Return non-const reference to wall interaction model
        inline InflowBoundaryModel<DSMCCloud<ParcelType> >&
          inflowBoundary();
    // Check
      //- Total mass in system
      inline scalar massInSystem() const;
      //- Total linear momentum of the system
      inline vector linearMomentumOfSystem() const;
      //- Total linear kinetic energy in the system
      inline scalar linearKineticEnergyOfSystem() const;
      //- Total internal energy in the system
      inline scalar internalEnergyOfSystem() const;
      //- Print cloud information
      void info() const;
      //- Dump particle positions to .obj file
      void dumpParticlePositions() const;
    // Cloud evolution functions
      //- Add new parcel
      void addNewParcel
      (
        const vector& position,
        const vector& U,
        const scalar Ei,
        const label cellI,
        const label tetFaceI,
        const label tetPtI,
        const label typeId
      );
      //- Evolve the cloud (move, collide)
      void evolve();
      //- Clear the Cloud
      inline void clear();
    // Mapping
      //- Remap the particles to the correct cells following mesh change
      virtual void autoMap(const mapPolyMesh&);
};

}  // namespace mousse


// Member Functions 
template<class ParcelType>
inline const mousse::word& mousse::DSMCCloud<ParcelType>::cloudName() const
{
  return cloudName_;
}


template<class ParcelType>
inline const mousse::fvMesh& mousse::DSMCCloud<ParcelType>::mesh() const
{
  return mesh_;
}


template<class ParcelType>
inline const mousse::IOdictionary&
mousse::DSMCCloud<ParcelType>::particleProperties() const
{
  return particleProperties_;
}


template<class ParcelType>
inline const mousse::List<mousse::word>&
mousse::DSMCCloud<ParcelType>::typeIdList() const
{
  return typeIdList_;
}


template<class ParcelType>
inline mousse::scalar mousse::DSMCCloud<ParcelType>::nParticle() const
{
  return nParticle_;
}


template<class ParcelType>
inline const mousse::List<mousse::DynamicList<ParcelType*> >&
mousse::DSMCCloud<ParcelType>::cellOccupancy() const
{
  return cellOccupancy_;
}


template<class ParcelType>
inline mousse::volScalarField& mousse::DSMCCloud<ParcelType>::sigmaTcRMax()
{
  return sigmaTcRMax_;
}


template<class ParcelType>
inline mousse::scalarField&
mousse::DSMCCloud<ParcelType>::collisionSelectionRemainder()
{
  return collisionSelectionRemainder_;
}


template<class ParcelType>
inline const mousse::List<typename ParcelType::constantProperties>&
mousse::DSMCCloud<ParcelType>::constProps() const
{
  return constProps_;
}


template<class ParcelType>
inline const typename ParcelType::constantProperties&
mousse::DSMCCloud<ParcelType>::constProps
(
  label typeId
) const
{
  if (typeId < 0 || typeId >= constProps_.size()) {
    FATAL_ERROR_IN("mousse::DSMCCloud<ParcelType>::constProps(label typeId)")
      << "constantProperties for requested typeId index "
      << typeId << " do not exist" << nl
      << abort(FatalError);
  }
  return constProps_[typeId];
}


template<class ParcelType>
inline mousse::Random& mousse::DSMCCloud<ParcelType>::rndGen()
{
  return rndGen_;
}


template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::qBF()
{
  return q_.boundaryField();
}


template<class ParcelType>
inline mousse::volVectorField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::fDBF()
{
  return fD_.boundaryField();
}


template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::rhoNBF()
{
  return rhoN_.boundaryField();
}


template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::rhoMBF()
{
  return rhoM_.boundaryField();
}


template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::linearKEBF()
{
  return linearKE_.boundaryField();
}


template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::internalEBF()
{
  return internalE_.boundaryField();
}


template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::iDofBF()
{
  return iDof_.boundaryField();
}


template<class ParcelType>
inline mousse::volVectorField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::momentumBF()
{
  return momentum_.boundaryField();
}


template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::boundaryT() const
{
  return boundaryT_;
}


template<class ParcelType>
inline const mousse::volVectorField&
mousse::DSMCCloud<ParcelType>::boundaryU() const
{
  return boundaryU_;
}


template<class ParcelType>
inline const mousse::BinaryCollisionModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::binaryCollision() const
{
  return binaryCollisionModel_;
}


template<class ParcelType>
inline mousse::BinaryCollisionModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::binaryCollision()
{
  return binaryCollisionModel_();
}


template<class ParcelType>
inline const mousse::WallInteractionModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::wallInteraction() const
{
  return wallInteractionModel_;
}


template<class ParcelType>
inline mousse::WallInteractionModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::wallInteraction()
{
  return wallInteractionModel_();
}


template<class ParcelType>
inline const mousse::InflowBoundaryModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::inflowBoundary() const
{
  return inflowBoundaryModel_;
}


template<class ParcelType>
inline mousse::InflowBoundaryModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::inflowBoundary()
{
  return inflowBoundaryModel_();
}


template<class ParcelType>
inline mousse::scalar mousse::DSMCCloud<ParcelType>::massInSystem() const
{
  scalar sysMass = 0.0;
  FOR_ALL_CONST_ITER(typename DSMCCloud<ParcelType>, *this, iter) {
    const ParcelType& p = iter();
    const typename ParcelType::constantProperties& cP = constProps(p.typeId());
    sysMass += cP.mass();
  }
  return nParticle_*sysMass;
}


template<class ParcelType>
inline mousse::vector mousse::DSMCCloud<ParcelType>::linearMomentumOfSystem() const
{
  vector linearMomentum(vector::zero);
  FOR_ALL_CONST_ITER(typename DSMCCloud<ParcelType>, *this, iter) {
    const ParcelType& p = iter();
    const typename ParcelType::constantProperties& cP = constProps(p.typeId());
    linearMomentum += cP.mass()*p.U();
  }
  return nParticle_*linearMomentum;
}


template<class ParcelType>
inline mousse::scalar
mousse::DSMCCloud<ParcelType>::linearKineticEnergyOfSystem() const
{
  scalar linearKineticEnergy = 0.0;
  FOR_ALL_CONST_ITER(typename DSMCCloud<ParcelType>, *this, iter) {
    const ParcelType& p = iter();
    const typename ParcelType::constantProperties& cP = constProps(p.typeId());
    linearKineticEnergy += 0.5*cP.mass()*(p.U() & p.U());
  }
  return nParticle_*linearKineticEnergy;
}


template<class ParcelType>
inline mousse::scalar
mousse::DSMCCloud<ParcelType>::internalEnergyOfSystem() const
{
  scalar internalEnergy = 0.0;
  FOR_ALL_CONST_ITER(typename DSMCCloud<ParcelType>, *this, iter) {
    const ParcelType& p = iter();
    internalEnergy += p.Ei();
  }
  return nParticle_*internalEnergy;
}


template<class ParcelType>
inline mousse::scalar mousse::DSMCCloud<ParcelType>::maxwellianAverageSpeed
(
  scalar temperature,
  scalar mass
) const
{
  return 2.0*sqrt(2.0*physicoChemical::k.value()*temperature/(pi*mass));
}


template<class ParcelType>
inline mousse::scalarField mousse::DSMCCloud<ParcelType>::maxwellianAverageSpeed
(
  scalarField temperature,
  scalar mass
) const
{
  tmp<scalarField> tfld =
    2.0*sqrt(2.0*physicoChemical::k.value()*temperature/(pi*mass));
  return tfld();
}


template<class ParcelType>
inline mousse::scalar mousse::DSMCCloud<ParcelType>::maxwellianRMSSpeed
(
  scalar temperature,
  scalar mass
) const
{
  return sqrt(3.0*physicoChemical::k.value()*temperature/mass);
}


template<class ParcelType>
inline mousse::scalarField mousse::DSMCCloud<ParcelType>::maxwellianRMSSpeed
(
  scalarField temperature,
  scalar mass
) const
{
  tmp<scalarField> tfld =
    sqrt(3.0*physicoChemical::k.value()*temperature/mass);
  return tfld();
}


template<class ParcelType>
inline mousse::scalar
mousse::DSMCCloud<ParcelType>::maxwellianMostProbableSpeed
(
  scalar temperature,
  scalar mass
) const
{
  return sqrt(2.0*physicoChemical::k.value()*temperature/mass);
}


template<class ParcelType>
inline mousse::scalarField
mousse::DSMCCloud<ParcelType>::maxwellianMostProbableSpeed
(
  scalarField temperature,
  scalar mass
) const
{
  tmp<scalarField> tfld =
    sqrt(2.0*physicoChemical::k.value()*temperature/mass);
  return tfld();
}


template<class ParcelType>
inline const mousse::volScalarField& mousse::DSMCCloud<ParcelType>::q() const
{
  return q_;
}


template<class ParcelType>
inline const mousse::volVectorField& mousse::DSMCCloud<ParcelType>::fD() const
{
  return fD_;
}


template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::rhoN() const
{
  return rhoN_;
}


template<class ParcelType>
inline const mousse::volScalarField& mousse::DSMCCloud<ParcelType>::rhoM() const
{
  return rhoM_;
}


template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::dsmcRhoN() const
{
  return dsmcRhoN_;
}


template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::linearKE() const
{
  return linearKE_;
}


template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::internalE() const
{
  return internalE_;
}


template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::iDof() const
{
  return iDof_;
}


template<class ParcelType>
inline const mousse::volVectorField& mousse::DSMCCloud<ParcelType>::momentum() const
{
  return momentum_;
}


template<class ParcelType>
inline void mousse::DSMCCloud<ParcelType>::clear()
{
  return IDLList<ParcelType>::clear();
}

#include "_dsmc_cloud.ipp"

#endif
