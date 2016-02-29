#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_BASE_CLASSES_KINEMATIC_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_BASE_CLASSES_KINEMATIC_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kinematicCloud
// Description
//   Virtual abstract base class for templated KinematicCloud
// SourceFiles
//   kinematic_cloud.hpp
#include "type_info.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
class kinematicCloud
{
public:
  //- Runtime type information
  TYPE_NAME("kinematicCloud");
  // Constructors
    //- Null constructor
    kinematicCloud();
    //- Disallow default bitwise copy construct
    kinematicCloud(const kinematicCloud&) = delete;
    //- Disallow default bitwise assignment
    kinematicCloud& operator=(const kinematicCloud&) = delete;
  // Member functions
    // Check
      //-  Number of parcels
      virtual label nParcels() const = 0;
      //- Total mass in system
      virtual scalar massInSystem() const = 0;
      //- Total linear momentum of the system
      virtual vector linearMomentumOfSystem() const = 0;
      //- Total linear kinetic energy in the system
      virtual scalar linearKineticEnergyOfSystem() const = 0;
      //- Total rotational kinetic energy in the system
//            virtual scalar rotationalKineticEnergyOfSystem() const = 0;
      //- Penetration for percentage of the current total mass
//            virtual scalar penetration(const scalar& fraction) const = 0;
      //- Mean diameter Dij
      virtual scalar Dij(const label i, const label j) const = 0;
      //- Max diameter
      virtual scalar Dmax() const = 0;
      // Fields
        //- Volume swept rate of parcels per cell
        virtual const tmp<volScalarField> vDotSweep() const = 0;
        //- Return the particle volume fraction field
        //  Note: for particles belonging to this cloud only
        virtual const tmp<volScalarField> theta() const = 0;
        //- Return the particle mass fraction field
        //  Note: for particles belonging to this cloud only
        virtual const tmp<volScalarField> alpha() const = 0;
        //- Return the particle effective density field
        //  Note: for particles belonging to this cloud only
        virtual const tmp<volScalarField> rhoEff() const = 0;
  //- Destructor
  virtual ~kinematicCloud();
};
}  // namespace mousse
#endif
