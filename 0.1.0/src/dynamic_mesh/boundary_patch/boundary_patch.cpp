// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "boundary_patch.hpp"
#include "dictionary.hpp"
#include "ostream.hpp"
// Constructors 
mousse::boundaryPatch::boundaryPatch
(
  const word& name,
  const label index,
  const label size,
  const label start,
  const word& physicalType
)
:
  patchIdentifier(name, index, physicalType),
  size_(size),
  start_(start)
{}
mousse::boundaryPatch::boundaryPatch
(
  const word& name,
  const dictionary& dict,
  const label index
)
:
  patchIdentifier(name, dict, index),
  size_(readLabel(dict.lookup("nFaces"))),
  start_(readLabel(dict.lookup("startFace")))
{}
mousse::boundaryPatch::boundaryPatch(const boundaryPatch& p)
:
  patchIdentifier(p.name(), p.index(), p.physicalType()),
  size_(p.size()),
  start_(p.start())
{}
mousse::boundaryPatch::boundaryPatch(const boundaryPatch& p, const label index)
:
  patchIdentifier(p.name(), index, p.physicalType()),
  size_(p.size()),
  start_(p.start())
{}
mousse::autoPtr<mousse::boundaryPatch> mousse::boundaryPatch::clone() const
{
  return autoPtr<boundaryPatch>(new boundaryPatch(*this));
}
// Destructor 
mousse::boundaryPatch::~boundaryPatch()
{}
// Member Functions 
void mousse::boundaryPatch::write(Ostream& os) const
{
  patchIdentifier::write(os);
  os.writeKeyword("nFaces") << size_ << token::END_STATEMENT << nl;
  os.writeKeyword("startFace") << start_ << token::END_STATEMENT << nl;
}
// Friend Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const boundaryPatch& p)
{
  p.write(os);
  os.check("Ostream& operator<<(Ostream& f, const boundaryPatch&)");
  return os;
}
