// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::STLtriangle::STLtriangle()
{}
inline mousse::STLtriangle::STLtriangle
(
  const STLpoint& normal,
  const STLpoint& a,
  const STLpoint& b,
  const STLpoint& c,
  unsigned short attrib
)
:
  normal_(normal),
  a_(a),
  b_(b),
  c_(c),
  attrib_(attrib)
{}
inline mousse::STLtriangle::STLtriangle(istream& is)
{
  read(is);
}
// Member Functions 
inline const mousse::STLpoint& mousse::STLtriangle::normal() const
{
  return normal_;
}
inline const mousse::STLpoint& mousse::STLtriangle::a() const
{
  return a_;
}
inline const mousse::STLpoint& mousse::STLtriangle::b() const
{
  return b_;
}
inline const mousse::STLpoint& mousse::STLtriangle::c() const
{
  return c_;
}
inline unsigned short mousse::STLtriangle::attrib() const
{
  return attrib_;
}
inline void mousse::STLtriangle::read(istream& is)
{
  is.read(reinterpret_cast<char*>(this), 4*sizeof(STLpoint));
  is.read(reinterpret_cast<char*>(&attrib_), sizeof(STLattrib));
}
inline void mousse::STLtriangle::write(ostream& os)
{
  os.write(reinterpret_cast<char*>(this), 4*sizeof(STLpoint));
  os.write(reinterpret_cast<char*>(&attrib_), sizeof(STLattrib));
}
// Ostream Operator 
inline mousse::Ostream& mousse::operator<<(Ostream& os, const STLtriangle& t)
{
  os  << t.normal_ << token::SPACE
    << t.a_ << token::SPACE
    << t.b_ << token::SPACE
    << t.c_ << token::SPACE
    << t.attrib_;
  return os;
}
