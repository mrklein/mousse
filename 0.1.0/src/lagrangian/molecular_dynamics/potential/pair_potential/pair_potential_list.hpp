// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentialList
// Description
// SourceFiles
//   pair_potential_list_i.hpp
//   pair_potential_list.cpp
#ifndef pair_potential_list_hpp_
#define pair_potential_list_hpp_
#include "ptr_list.hpp"
#include "word.hpp"
#include "pair_potential.hpp"
#include "poly_mesh.hpp"
namespace mousse
{
class pairPotentialList
:
  public PtrList<pairPotential>
{
  // Private data
    label nIds_;
    scalar rCutMax_;
    scalar rCutMaxSqr_;
    autoPtr<pairPotential> electrostaticPotential_;
  // Private Member Functions
    inline label pairPotentialIndex
    (
      const label a,
      const label b
    ) const;
    void readPairPotentialDict
    (
      const List<word>& idList,
      const dictionary& pairPotentialDict,
      const polyMesh& mesh
    );
public:
  // Constructors
    pairPotentialList();
    //- Construct from idList and potental dictionaries
    pairPotentialList
    (
      const List<word>& idList,
      const dictionary& pairPotentialDict,
      const polyMesh& mesh
    );
    //- Disallow default bitwise assignment
    pairPotentialList& operator=(const pairPotentialList&) = delete;
    //- Disallow default bitwise copy construct
    pairPotentialList(const pairPotentialList&) = delete;
  //- Destructor
  ~pairPotentialList();
  // Member Functions
    void buildPotentials
    (
      const List<word>& idList,
      const dictionary& pairPotentialDict,
      const polyMesh& mesh
    );
    // Access
      inline scalar rCutMax() const;
      inline scalar rCutMaxSqr() const;
      bool rCutMaxSqr(scalar rIJMagSqr) const;
      const pairPotential& pairPotentialFunction
      (
        const label a,
        const label b
      ) const;
      // Return true if rIJ is within rCut for this pair.
      bool rCutSqr
      (
        const label a,
        const label b,
        const scalar rIJMagSqr
      ) const;
      scalar rMin(const label a, const label b) const;
      scalar dr(const label a, const label b) const;
      scalar rCutSqr(const label a, const label b) const;
      scalar rCut(const label a, const label b) const;
      scalar force
      (
        const label a,
        const label b,
        const scalar rIJMag
      ) const;
      scalar energy
      (
        const label a,
        const label b,
        const scalar rIJMag
      ) const;
      inline const pairPotential& electrostatic() const;
};
}  // namespace mousse

// Private Member Functions 
inline mousse::label mousse::pairPotentialList::pairPotentialIndex
(
  const label a,
  const label b
) const
{
  label index;
  if (a < b)
  {
    index = a*(2*nIds_ - a - 1)/2 + b;
  }
  else
  {
    index = b*(2*nIds_ - b - 1)/2 + a;
  }
  if (index > size() - 1)
  {
    FATAL_ERROR_IN("mousse::pairPotentialList::pairPotentialIndex ")
      << "Attempting to access a pairPotential with too high an index."
      << nl << "a = " << a << ", b = " << b << ", index = " << index
      << nl << "max index = " << size() - 1
      << nl << abort(FatalError);
  }
  return index;
}
// Member Functions 
inline mousse::scalar mousse::pairPotentialList::rCutMax() const
{
  return rCutMax_;
}
inline mousse::scalar mousse::pairPotentialList::rCutMaxSqr() const
{
  return rCutMaxSqr_;
}
inline const mousse::pairPotential& mousse::pairPotentialList::electrostatic() const
{
  return electrostaticPotential_;
}
#endif
