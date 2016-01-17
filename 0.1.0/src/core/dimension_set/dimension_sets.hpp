// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   dimension_sets.cpp
#ifndef dimension_sets_hpp_
#define dimension_sets_hpp_
#include "scalar_matrices.hpp"
#include "dimensioned_scalar_fwd.hpp"
#include "ptr_list.hpp"
namespace mousse
{
extern const dimensionSet dimless;
extern const dimensionSet dimMass;
extern const dimensionSet dimLength;
extern const dimensionSet dimTime;
extern const dimensionSet dimTemperature;
extern const dimensionSet dimMoles;
extern const dimensionSet dimCurrent;
extern const dimensionSet dimLuminousIntensity;
extern const dimensionSet dimArea;
extern const dimensionSet dimVolume;
extern const dimensionSet dimVol;
extern const dimensionSet dimDensity;
extern const dimensionSet dimForce;
extern const dimensionSet dimEnergy;
extern const dimensionSet dimPower;
extern const dimensionSet dimVelocity;
extern const dimensionSet dimAcceleration;
extern const dimensionSet dimPressure;
extern const dimensionSet dimCompressibility;
extern const dimensionSet dimGasConstant;
extern const dimensionSet dimSpecificHeatCapacity;
extern const dimensionSet dimViscosity;
extern const dimensionSet dimDynamicViscosity;
class dimensionSets
{
  // Private data
    //- Set of dimensions
    PtrList<dimensionedScalar> units_;
    //- LU decomposition of dimensions
    scalarSquareMatrix conversion_;
    //- See above
    labelList conversionPivots_;
    //- Is LU decomposition valid
    bool valid_;
public:
  // Constructors
    //- Construct from all units and set of units to use for inversion
    //  (writing)
    dimensionSets
    (
      const HashTable<dimensionedScalar>&,
      const wordList& unitNames
    );
  // Member functions
    //- Return the units
    const PtrList<dimensionedScalar>& units() const
    {
      return units_;
    }
    //- Is there a valid inverse of the selected unit
    bool valid() const
    {
      return valid_;
    }
    //- (if valid) obtain set of coefficients of unitNames
    void coefficients(scalarField&) const;
};
//- Top level dictionary
dictionary& dimensionSystems();
//- Set of all dimensions
const HashTable<dimensionedScalar>& unitSet();
//- Set of units
const dimensionSets& writeUnitSet();
}  // namespace mousse
#endif
