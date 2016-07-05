#ifndef LAGRANGIAN_BASIC_INDEXED_PARTICLE_INDEXED_PARTICLE_HPP_
#define LAGRANGIAN_BASIC_INDEXED_PARTICLE_INDEXED_PARTICLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::indexedParticle
// Description
//   Adds label index to base particle

#include "particle.hpp"
#include "iostream.hpp"
#include "auto_ptr.hpp"


namespace mousse {

class indexedParticle
:
  public particle
{
  // Private data
    label index_;
public:
  // Constructors
    //- Construct from components
    indexedParticle
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI,
      const label index = 0
    )
    :
      particle{mesh, position, cellI, tetFaceI, tetPtI},
      index_{index}
    {}
    //- Construct from components, with searching for tetFace and tetPt
    indexedParticle
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label index = 0
    )
    :
      particle{mesh, position, cellI},
      index_{index}
    {}
    //- Construct from Istream
    indexedParticle
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    )
    :
      particle{mesh, is, readFields}
    {}
    //- Construct as a copy
    indexedParticle(const indexedParticle& p)
    :
      particle{p}
    {}
    //- Construct and return a clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>(new indexedParticle(*this));
    }
  // Member functions
    label index() const
    {
      return index_;
    }
    label& index()
    {
      return index_;
    }
};

}  // namespace mousse

#endif

