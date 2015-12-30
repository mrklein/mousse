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
    //- Disallow default bitwise assignment
    void operator=(const pairPotentialList&);
    //- Disallow default bitwise copy construct
    pairPotentialList(const pairPotentialList&);
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
#include "pair_potential_list_i.hpp"
#endif
