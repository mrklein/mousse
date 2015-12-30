// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::potential
// Description
// SourceFiles
//   potential_i.hpp
//   potential.cpp
#ifndef potential_hpp_
#define potential_hpp_
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
    //- Disallow default bitwise copy construct
    potential(const potential&);
    //- Disallow default bitwise assignment
    void operator=(const potential&);
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
#include "potential_i.hpp"
#endif
