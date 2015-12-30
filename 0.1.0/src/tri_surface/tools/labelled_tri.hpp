// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::labelledTri
// Description
//   Triangle with additional region number.
// SourceFiles
//   labelled_tri_i.hpp
#ifndef labelled_tri_hpp_
#define labelled_tri_hpp_
#include "tri_face.hpp"
#include "list_list_ops.hpp"
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
    forAll(x, xI)
    {
      result[xI] = x[xI] + offset;
    }
    return result;
  }
};
}  // namespace mousse
#include "labelled_tri_i.hpp"
#endif
