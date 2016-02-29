#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_POTENTIAL_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_POTENTIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::potential
// Description
// SourceFiles
//   potential.cpp
#include "poly_mesh.hpp"
#include "iodictionary.hpp"
#include "time.hpp"
#include "pair_potential_list.hpp"
#include "electrostatic_potential.hpp"
#include "tether_potential_list.hpp"
namespace mousse
{
class potential
{
  // Private data
    const polyMesh& mesh_;
    List<word> idList_;
    List<word> siteIdList_;
    label nPairPotIds_;
    scalar potentialEnergyLimit_;
    labelList removalOrder_;
    pairPotentialList pairPotentials_;
    tetherPotentialList tetherPotentials_;
    vector gravity_;
  // Private Member Functions
    void setSiteIdList(const dictionary& moleculePropertiesDict);
    void readPotentialDict();
    void readMdInitialiseDict
    (
      const IOdictionary& mdInitialiseDict,
      IOdictionary& idListDict
    );
public:
  // Constructors
    //- Construct from mesh reference
    potential(const polyMesh& mesh);
    //- Construct from mdInitialiseDict
    potential
    (
      const polyMesh& mesh,
      const IOdictionary& mdInitialiseDict,
      IOdictionary& idListDict
    );
    //- Disallow default bitwise copy construct
    potential(const potential&) = delete;
    //- Disallow default bitwise assignment
    potential& operator=(const potential&) = delete;
  //- Destructor
  ~potential();
  // Member Functions
    // Access
      inline label nIds() const;
      inline const List<word>& idList() const;
      inline const List<word>& siteIdList() const;
      inline scalar potentialEnergyLimit() const;
      inline label nPairPotentials() const;
      inline const labelList& removalOrder() const;
      inline const pairPotentialList& pairPotentials() const;
      inline const tetherPotentialList& tetherPotentials() const;
      inline const vector& gravity() const;
};
}  // namespace mousse

// Member Functions 
inline mousse::label mousse::potential::nIds() const
{
  return idList_.size();
}
inline const mousse::List<mousse::word>& mousse::potential::idList() const
{
  return idList_;
}
inline const mousse::List<mousse::word>& mousse::potential::siteIdList() const
{
  return siteIdList_;
}
inline mousse::scalar mousse::potential::potentialEnergyLimit() const
{
  return potentialEnergyLimit_;
}
inline mousse::label mousse::potential::nPairPotentials() const
{
  return pairPotentials_.size();
}
inline const mousse::labelList& mousse::potential::removalOrder() const
{
  return removalOrder_;
}
inline const mousse::pairPotentialList& mousse::potential::pairPotentials() const
{
  return pairPotentials_;
}
inline const mousse::tetherPotentialList&
mousse::potential::tetherPotentials() const
{
  return tetherPotentials_;
}
inline const mousse::vector& mousse::potential::gravity() const
{
  return gravity_;
}
#endif
