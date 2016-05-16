#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_TETHER_POTENTIAL_TETHER_POTENTIAL_LIST_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_TETHER_POTENTIAL_TETHER_POTENTIAL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetherPotentialList

#include "list_ops.hpp"
#include "ptr_list.hpp"
#include "word.hpp"
#include "tether_potential.hpp"


namespace mousse {

class tetherPotentialList
:
  public PtrList<tetherPotential>
{
  // Private data
    List<label> idMap_;
  // Private Member Functions
    inline label tetherPotentialIndex
    (
      const label a
    ) const;
    void readTetherPotentialDict
    (
      const List<word>& siteIdList,
      const dictionary& tetherPotentialDict,
      const List<word>& tetherSiteIdList
    );
public:
  // Constructors
    tetherPotentialList();
    //- Construct from siteIdList and potental dictionaries
    tetherPotentialList
    (
      const List<word>& siteIdList,
      const dictionary& tetherPotentialDict,
      const List<word>& tetherSiteIdList
    );
    //- Disallow default bitwise assignment
    tetherPotentialList& operator=(const tetherPotentialList&) = delete;
    //- Disallow default bitwise copy construct
    tetherPotentialList(const tetherPotentialList&) = delete;
  //- Destructor
  ~tetherPotentialList();
  // Member Functions
    void buildPotentials
    (
      const List<word>& siteIdList,
      const dictionary& tetherPotentialDict,
      const List<word>& tetherSiteIdList
    );
    // Access
      inline const List<word>& idMap() const;
      const tetherPotential& tetherPotentialFunction(const label a) const;
      vector force(const label a, const vector rIT) const;
      scalar energy (const label a, const vector rIT) const;
};

}  // namespace mousse


// Private Member Functions 
inline mousse::label mousse::tetherPotentialList::tetherPotentialIndex
(
  const label a
) const
{
  label index = idMap_[a];
  if (index == -1 || a >= idMap_.size()) {
    FATAL_ERROR_IN
    (
      "mousse::tetherPotentialList::tetherPotentialIndex(const label a)"
    )
    << "Attempting to access an undefined tetherPotential."
    << abort(FatalError);
    return -1;
  } else {
    return index;
  }
}

#endif

