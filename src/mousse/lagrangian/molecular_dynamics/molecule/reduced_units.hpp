#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_MOLECULE_REDUCED_UNITS_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_MOLECULE_REDUCED_UNITS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reducedUnits

#include "scalar.hpp"
#include "iodictionary.hpp"


namespace mousse {

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
    //- Disallow default bitwise copy construct
    reducedUnits(const reducedUnits&) = delete;
    //- Disallow default bitwise assignment
    reducedUnits& operator=(const reducedUnits&) = delete;
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


// Member Functions 
inline mousse::scalar mousse::reducedUnits::refLength() const
{
  return refLength_;
}


inline mousse::scalar mousse::reducedUnits::refTime() const
{
  return refTime_;
}


inline mousse::scalar mousse::reducedUnits::refMass() const
{
  return refMass_;
}


inline mousse::scalar mousse::reducedUnits::refTemp() const
{
  return refTemp_;
}


inline mousse::scalar mousse::reducedUnits::refEnergy() const
{
  return refEnergy_;
}


inline mousse::scalar mousse::reducedUnits::refForce() const
{
  return refForce_;
}


inline mousse::scalar mousse::reducedUnits::refVelocity() const
{
  return refVelocity_;
}


inline mousse::scalar mousse::reducedUnits::refVolume() const
{
  return refVolume_;
}


inline mousse::scalar mousse::reducedUnits::refPressure() const
{
  return refPressure_;
}


inline mousse::scalar mousse::reducedUnits::refMassDensity() const
{
  return refMassDensity_;
}


inline mousse::scalar mousse::reducedUnits::refNumberDensity() const
{
  return refNumberDensity_;
}

#endif

