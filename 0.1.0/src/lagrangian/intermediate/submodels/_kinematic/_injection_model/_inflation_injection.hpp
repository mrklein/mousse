// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InflationInjection
// Description
//   Inflation injection - creates new particles by splitting existing
//   particles within in a set of generation cells, then inflating them
//   to a target diameter within the generation cells and an additional
//   set of inflation cells.
// SourceFiles
//   _inflation_injection.cpp
#ifndef _inflation_injection_hpp_
#define _inflation_injection_hpp_
#include "_injection_model.hpp"
#include "distribution_model.hpp"
#include "switch.hpp"
namespace mousse
{
// Structure to hold:
// + position = vectorPairScalarPair::first().first()
// + velocity = vectorPairScalarPair::first().second()
// + diameter = vectorPairScalarPair::second().first()
// + target diameter = vectorPairScalarPair::second().second()
// One structure to allow single operation parallel comms
typedef Tuple2<Pair<vector>, Pair<scalar> > vectorPairScalarPair;
template<class CloudType>
class InflationInjection
:
  public InjectionModel<CloudType>
{
  // Private data
    //- Name of cellSet for generating new particles
    word generationSetName_;
    //- Name of cellSet for inflating new particles
    word inflationSetName_;
    //- Set of cells to generate particles in
    labelList generationCells_;
    //- Set of cells to inflate particles in, includes all
    //  generation cells
    labelList inflationCells_;
    //- Injection duration [s]
    scalar duration_;
    //- Flow rate profile relative to SOI [m3/s]
    TimeDataEntry<scalar> flowRateProfile_;
    //- Growth rate of particle diameters towards target [m/s]
    TimeDataEntry<scalar> growthRate_;
    //- Positions, velocities, diameters and target diameters of
    //  new particles after splitting
    DynamicList<vectorPairScalarPair> newParticles_;
    //- Accumulation variable to carry over volume from one injection
    //  to the next
    scalar volumeAccumulator_;
    //- Fraction of injection controlled by this processor
    scalar fraction_;
    //- Switch to control whether or not the injector is allowed
    //  to create new particles in empty cells
    Switch selfSeed_;
    //- Diameter with which to create new seed particles
    scalar dSeed_;
    //- Parcel size distribution model
    const autoPtr<distributionModels::distributionModel> sizeDistribution_;
public:
  //- Runtime type information
  TypeName("inflationInjection");
  // Constructors
    //- Construct from dictionary
    InflationInjection
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    InflationInjection(const InflationInjection<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType> > clone() const
    {
      return autoPtr<InjectionModel<CloudType> >
      (
        new InflationInjection<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~InflationInjection();
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
#   include "_inflation_injection.cpp"
#endif
#endif
