// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_particle.hpp"
#include "iostreams.hpp"
// Static Data Members
const std::size_t mousse::solidParticle::sizeofFields_
(
  sizeof(solidParticle) - sizeof(particle)
);
// Constructors 
mousse::solidParticle::solidParticle
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  particle(mesh, is, readFields)
{
  if (readFields)
  {
    if (is.format() == IOstream::ASCII)
    {
      d_ = readScalar(is);
      is >> U_;
    }
    else
    {
      is.read(reinterpret_cast<char*>(&d_), sizeofFields_);
    }
  }
  // Check state of Istream
  is.check("solidParticle::solidParticle(Istream&)");
}
void mousse::solidParticle::readFields(Cloud<solidParticle>& c)
{
  if (!c.size())
  {
    return;
  }
  particle::readFields(c);
  IOField<scalar> d(c.fieldIOobject("d", IOobject::MUST_READ));
  c.checkFieldIOobject(c, d);
  IOField<vector> U(c.fieldIOobject("U", IOobject::MUST_READ));
  c.checkFieldIOobject(c, U);
  label i = 0;
  forAllIter(Cloud<solidParticle>, c, iter)
  {
    solidParticle& p = iter();
    p.d_ = d[i];
    p.U_ = U[i];
    i++;
  }
}
void mousse::solidParticle::writeFields(const Cloud<solidParticle>& c)
{
  particle::writeFields(c);
  label np = c.size();
  IOField<scalar> d(c.fieldIOobject("d", IOobject::NO_READ), np);
  IOField<vector> U(c.fieldIOobject("U", IOobject::NO_READ), np);
  label i = 0;
  forAllConstIter(Cloud<solidParticle>, c, iter)
  {
    const solidParticle& p = iter();
    d[i] = p.d_;
    U[i] = p.U_;
    i++;
  }
  d.write();
  U.write();
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const solidParticle& p)
{
  if (os.format() == IOstream::ASCII)
  {
    os  << static_cast<const particle&>(p)
      << token::SPACE << p.d_
      << token::SPACE << p.U_;
  }
  else
  {
    os  << static_cast<const particle&>(p);
    os.write
    (
      reinterpret_cast<const char*>(&p.d_),
      solidParticle::sizeofFields_
    );
  }
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const solidParticle&)");
  return os;
}
