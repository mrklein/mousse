#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_PAIR_POTENTIAL_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_PAIR_POTENTIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotential
// Description
// SourceFiles
//   pair_potential.cpp
//   pair_potential_new.cpp
#include "iodictionary.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "list.hpp"
#include "pair.hpp"
#include "switch.hpp"
namespace mousse
{
// Forward declaration of classes
class energyScalingFunction;
class pairPotential
{
protected:
  // Protected data
    word name_;
    dictionary pairPotentialProperties_;
    scalar rCut_;
    scalar rCutSqr_;
    scalar rMin_;
    scalar dr_;
    List<scalar> forceLookup_;
    List<scalar> energyLookup_;
    mutable energyScalingFunction* esfPtr_;
    bool writeTables_;
  // Private Member Functions
    void scaleEnergy(scalar& e, const scalar r) const;
public:
  //- Runtime type information
    TYPE_NAME("pairPotential");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      pairPotential,
      dictionary,
      (
        const word& name,
        const dictionary& pairPotentialProperties
      ),
      (name, pairPotentialProperties)
    );
  // Selectors
    //- Return a reference to the selected viscosity model
    static autoPtr<pairPotential> New
    (
      const word& name,
      const dictionary& pairPotentialProperties
    );
  // Constructors
    //- Construct from components
    pairPotential
    (
      const word& name,
      const dictionary& pairPotentialProperties
    );
    //- Disallow copy construct
    pairPotential(const pairPotential&) = delete;
    //- Disallow default bitwise assignment
    pairPotential& operator=(const pairPotential&) = delete;
  //- Destructor
  virtual ~pairPotential()
  {}
  // Member Functions
    void setLookupTables();
    inline scalar rMin() const;
    inline scalar dr() const;
    inline scalar rCut() const;
    inline scalar rCutSqr() const;
    scalar energy (const scalar r) const;
    scalar force (const scalar r) const;
    List<Pair<scalar> > energyTable() const;
    List<Pair<scalar> > forceTable() const;
    inline bool writeTables() const;
    virtual scalar unscaledEnergy(const scalar r) const = 0;
    scalar scaledEnergy(const scalar r) const;
    scalar energyDerivative
    (
      const scalar r,
      const bool scaledEnergyDerivative = true
    ) const;
    const dictionary& pairPotentialProperties() const
    {
      return pairPotentialProperties_;
    }
    bool writeEnergyAndForceTables(Ostream& os) const;
    //- Read pairPotential dictionary
    virtual bool read(const dictionary& pairPotentialProperties) = 0;
};
}  // namespace mousse

// Member Functions 
inline mousse::scalar mousse::pairPotential::rMin() const
{
  return rMin_;
}
inline mousse::scalar mousse::pairPotential::dr() const
{
  return dr_;
}
inline mousse::scalar mousse::pairPotential::rCut() const
{
  return rCut_;
}
inline mousse::scalar mousse::pairPotential::rCutSqr() const
{
  return rCutSqr_;
}
inline bool mousse::pairPotential::writeTables() const
{
  return writeTables_;
}
#endif
