// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotential
// Description
// SourceFiles
//   pair_potential.cpp
//   pair_potential_new.cpp
#ifndef pair_potential_hpp_
#define pair_potential_hpp_
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
    //- Disallow copy construct
    pairPotential(const pairPotential&);
    //- Disallow default bitwise assignment
    void operator=(const pairPotential&);
public:
  //- Runtime type information
    TypeName("pairPotential");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
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
#include "pair_potential_i.hpp"
#endif
