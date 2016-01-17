// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::labelledTri
// Description
//   Triangle with additional region number.
#ifndef labelled_tri_hpp_
#define labelled_tri_hpp_
#include "tri_face.hpp"
#include "list_list_ops.hpp"
#include "iostreams.hpp"
namespace mousse
{
class labelledTri
:
  public triFace
{
  // Private data
    label region_;
public:
  // Constructors
    //- Construct null
    inline labelledTri();
    //- Construct from triFace and a region label
    inline labelledTri
    (
      const triFace&,
      const label region
    );
    //- Construct from three point labels and a region label
    inline labelledTri
    (
      const label a,
      const label b,
      const label c,
      const label region
    );
    //- Construct from Istream
    inline labelledTri(Istream&);
  // Member Functions
    // Access
      //- Return region label
      inline label region() const;
      //- Return region label
      inline label& region();
    // Check
    // Edit
    // Write
  // Friend Functions
  // Friend Operators
  // IOstream Operators
    inline friend Istream& operator>>(Istream&, labelledTri&);
    inline friend Ostream& operator<<(Ostream&, const labelledTri&);
};
template<>
inline bool contiguous<labelledTri>()  {return true;}
//- Hash specialization to offset faces in ListListOps::combineOffset
template<>
class offsetOp<labelledTri>
{
public:
  inline labelledTri operator()
  (
    const labelledTri& x,
    const label offset
  ) const
  {
    labelledTri result(x);
    FOR_ALL(x, xI)
    {
      result[xI] = x[xI] + offset;
    }
    return result;
  }
};
}  // namespace mousse

// Constructors 
inline mousse::labelledTri::labelledTri()
:
  region_{-1}
{}
inline mousse::labelledTri::labelledTri
(
  const triFace& tri,
  const label region
)
:
  triFace{tri},
  region_{region}
{}
inline mousse::labelledTri::labelledTri
(
  const label a,
  const label b,
  const label c,
  const label region
)
:
  triFace{a, b, c},
  region_{region}
{}
inline mousse::labelledTri::labelledTri(Istream& is)
{
  operator>>(is, *this);
}
// Member Functions 
inline mousse::label mousse::labelledTri::region() const
{
  return region_;
}
inline mousse::label& mousse::labelledTri::region()
{
  return region_;
}
// Ostream Operator 
inline mousse::Istream& mousse::operator>>(Istream& is, labelledTri& t)
{
  if (is.format() == IOstream::ASCII)
  {
    // Read beginning of labelledTri point pair
    is.readBegin("labelledTri");
    is  >> static_cast<triFace&>(t) >> t.region_;
    // Read end of labelledTri point pair
    is.readEnd("labelledTri");
  }
  else
  {
    is.read(reinterpret_cast<char*>(&t), sizeof(labelledTri));
  }
  // Check state of Ostream
  is.check("Istream& operator>>(Istream&, labelledTri&)");
  return is;
}
inline mousse::Ostream& mousse::operator<<(Ostream& os, const labelledTri& t)
{
  if (os.format() == IOstream::ASCII)
  {
    os << token::BEGIN_LIST
      << static_cast<const triFace&>(t) << token::SPACE << t.region_
      << token::END_LIST;
  }
  else
  {
    os.write
    (
      reinterpret_cast<const char*>(&t),
      sizeof(labelledTri)
    );
  }
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const labelledTri&)");
  return os;
}
#endif
