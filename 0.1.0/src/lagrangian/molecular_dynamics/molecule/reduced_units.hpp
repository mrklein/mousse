// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reducedUnits
// Description
// SourceFiles
//   reduced_units_i.hpp
//   reduced_units.cpp
//   reduced_units_io.cpp
#ifndef reduced_units_hpp_
#define reduced_units_hpp_
#include "scalar.hpp"
#include "iodictionary.hpp"
namespace mousse
{
class reducedUnits
{
  // Private data
    // Reduced units
      // Fundamental values
        scalar refLength_;
        scalar refTime_;
        scalar refMass_;
      // Derived values
        scalar refEnergy_;
        scalar refTemp_;
        scalar refForce_;
        scalar refVelocity_;
        scalar refVolume_;
        scalar refPressure_;
        scalar refMassDensity_;
        scalar refNumberDensity_;
  // Private Member Functions
    void calcRefValues();
    //- Disallow default bitwise copy construct
    reducedUnits(const reducedUnits&);
    //- Disallow default bitwise assignment
    void operator=(const reducedUnits&);
public:
  // Static data members
    //- Static data someStaticData
    static const scalar kb;
  // Constructors
    //- Construct with no argument, uses default values:
    //  length  = 1nm
    //  mass = 1.660538782e-27kg (unified atomic mass unit)
    //  temperature = 1K (therefore, energy = 1*kb)
    reducedUnits();
    //- Construct from components
    reducedUnits
    (
      scalar refLength,
      scalar refTime,
      scalar refMass
    );
    //- Construct from dictionary
    reducedUnits(const IOdictionary& reducedUnitsDict);
  //- Destructor
  ~reducedUnits();
  // Member Functions
    void setRefValues
    (
      scalar refLength,
      scalar refTime,
      scalar refMass
    );
    void setRefValues(const IOdictionary& reducedUnitsDict);
    // Access
      inline scalar refLength() const;
      inline scalar refTime() const;
      inline scalar refMass() const;
      inline scalar refTemp() const;
      inline scalar refEnergy() const;
      inline scalar refForce() const;
      inline scalar refVelocity() const;
      inline scalar refVolume() const;
      inline scalar refPressure() const;
      inline scalar refMassDensity() const;
      inline scalar refNumberDensity() const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const reducedUnits&);
};
}  // namespace mousse
#include "reduced_units_i.hpp"
#endif
