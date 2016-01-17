// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#pragma GCC diagnostic ignored "-Winvalid-offsetof"

#include "particle.hpp"
#include "iostreams.hpp"
// Static Data Members
mousse::string mousse::particle::propertyList_ = mousse::particle::propertyList();
const std::size_t mousse::particle::sizeofPosition_
(
  offsetof(particle, faceI_) - offsetof(particle, position_)
);
const std::size_t mousse::particle::sizeofFields_
(
  sizeof(particle) - offsetof(particle, position_)
);
// Constructors 
mousse::particle::particle(const polyMesh& mesh, Istream& is, bool readFields)
:
  mesh_{mesh},
  position_{},
  cellI_{-1},
  faceI_{-1},
  stepFraction_{0.0},
  tetFaceI_{-1},
  tetPtI_{-1},
  origProc_{Pstream::myProcNo()},
  origId_{-1}
{
  if (is.format() == IOstream::ASCII)
  {
    is  >> position_ >> cellI_;
    if (readFields)
    {
      is  >> faceI_
        >> stepFraction_
        >> tetFaceI_
        >> tetPtI_
        >> origProc_
        >> origId_;
    }
  }
  else
  {
    if (readFields)
    {
      is.read(reinterpret_cast<char*>(&position_), sizeofFields_);
    }
    else
    {
      is.read(reinterpret_cast<char*>(&position_), sizeofPosition_);
    }
  }
  // Check state of Istream
  is.check("particle::particle(Istream&, bool)");
}
void mousse::particle::writePosition(Ostream& os) const
{
  if (os.format() == IOstream::ASCII)
  {
    os  << position_ << token::SPACE << cellI_;
  }
  else
  {
    os.write(reinterpret_cast<const char*>(&position_), sizeofPosition_);
  }
  // Check state of Ostream
  os.check("particle::writePosition(Ostream& os, bool) const");
}
mousse::Ostream& mousse::operator<<(Ostream& os, const particle& p)
{
  if (os.format() == IOstream::ASCII)
  {
    os  << p.position_
      << token::SPACE << p.cellI_
      << token::SPACE << p.faceI_
      << token::SPACE << p.stepFraction_
      << token::SPACE << p.tetFaceI_
      << token::SPACE << p.tetPtI_
      << token::SPACE << p.origProc_
      << token::SPACE << p.origId_;
  }
  else
  {
    os.write
    (
      reinterpret_cast<const char*>(&p.position_),
      particle::sizeofFields_
    );
  }
  return os;
}
