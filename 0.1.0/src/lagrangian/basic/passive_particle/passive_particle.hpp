#ifndef LAGRANGIAN_BASIC_PASSIVE_PARTICLE_PASSIVE_PARTICLE_HPP_
#define LAGRANGIAN_BASIC_PASSIVE_PARTICLE_PASSIVE_PARTICLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::passiveParticle
// Description
//   Copy of base particle
// SourceFiles
//   passive_particle.hpp
#include "particle.hpp"
#include "iostream.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class passiveParticle
:
  public particle
{
public:
  // Constructors
    //- Construct from components
    passiveParticle
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI
    )
    :
      particle(mesh, position, cellI, tetFaceI, tetPtI)
    {}
    //- Construct from components, with searching for tetFace and
    //  tetPt unless disabled by doCellFacePt = false.
    passiveParticle
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      bool doCellFacePt = true
    )
    :
      particle(mesh, position, cellI, doCellFacePt)
    {}
    //- Construct from Istream
    passiveParticle
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    )
    :
      particle(mesh, is, readFields)
    {}
    //- Construct as copy
    passiveParticle(const passiveParticle& p)
    :
      particle(p)
    {}
    //- Construct and return a clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>(new passiveParticle(*this));
    }
};
}  // namespace mousse
#endif
