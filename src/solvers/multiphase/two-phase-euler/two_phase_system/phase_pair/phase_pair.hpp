#ifndef SOLVERS_MULTIPHASE_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_PHASE_PAIR_PHASE_PAIR_HPP_
#define SOLVERS_MULTIPHASE_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_PHASE_PAIR_PHASE_PAIR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phasePair
// Description
// SourceFiles
//   phase_pair.cpp
#include "phase_model.hpp"
#include "phase_pair_key.hpp"
namespace mousse
{
class phasePair
:
  public phasePairKey
{
public:
  // Hash table types
    //- Dictionary hash table
    typedef HashTable<dictionary, phasePairKey, phasePairKey::hash>
      dictTable;
    //- Scalar hash table
    typedef HashTable<scalar, phasePairKey, phasePairKey::hash>
      scalarTable;
private:
  // Private data
    //- Phase 1
    const phaseModel& phase1_;
    //- Phase 2
    const phaseModel& phase2_;
    //- Gravitational acceleration
    const dimensionedVector& g_;
    //- Surface tension coefficient
    const dimensionedScalar sigma_;
  // Private member functions
    // Etvos number for given diameter
    tmp<volScalarField> EoH(const volScalarField& d) const;
public:
  // Constructors
    //- Construct from two phases, gravity and surface tension table
    phasePair
    (
      const phaseModel& phase1,
      const phaseModel& phase2,
      const dimensionedVector& g,
      const scalarTable& sigmaTable,
      const bool ordered = false
    );
  //- Destructor
  virtual ~phasePair();
  // Member Functions
    //- Dispersed phase
    virtual const phaseModel& dispersed() const;
    //- Continuous phase
    virtual const phaseModel& continuous() const;
    //- Pair name
    virtual word name() const;
    //- Average density
    tmp<volScalarField> rho() const;
    //- Relative velocity magnitude
    tmp<volScalarField> magUr() const;
    //- Relative velocity
    tmp<volVectorField> Ur() const;
    //- Reynolds number
    tmp<volScalarField> Re() const;
    //- Prandtl number
    tmp<volScalarField> Pr() const;
    //- Eotvos number
    tmp<volScalarField> Eo() const;
    //- Eotvos number based on hydraulic diameter type 1
    tmp<volScalarField> EoH1() const;
    //- Eotvos number based on hydraulic diameter type 2
    tmp<volScalarField> EoH2() const;
    //- Morton Number
    tmp<volScalarField> Mo() const;
    //- Takahashi Number
    tmp<volScalarField> Ta() const;
    //- Aspect ratio
    virtual tmp<volScalarField> E() const;
    // Access
      // Phase 1
      inline const phaseModel& phase1() const;
      // Phase 2
      inline const phaseModel& phase2() const;
      // Gravitational acceleration
      inline const dimensionedVector& g() const;
      // Surface tension coefficient
      inline const dimensionedScalar& sigma() const;
};
}  // namespace mousse

// Member Functions 
inline const mousse::phaseModel& mousse::phasePair::phase1() const
{
  return phase1_;
}
inline const mousse::phaseModel& mousse::phasePair::phase2() const
{
  return phase2_;
}
inline const mousse::dimensionedVector& mousse::phasePair::g() const
{
  return g_;
}
inline const mousse::dimensionedScalar& mousse::phasePair::sigma() const
{
  return sigma_;
}
#endif
