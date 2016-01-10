// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pTraits
// Description
//   Traits class for primitives.
//   All primitives need a specialised version of this class. The
//   specialised version will normally also require a conversion
//   method.
#ifndef ptraits_hpp_
#define ptraits_hpp_
namespace mousse
{
class Istream;
template<class PrimitiveType>
class pTraits
:
  public PrimitiveType
{
public:
  // Constructors
    //- Construct from primitive
    pTraits(const PrimitiveType& p)
    :
      PrimitiveType(p)
    {}
    //- Construct from Istream
    pTraits(Istream& is)
    :
      PrimitiveType(is)
    {}
};
}  // namespace mousse
#endif
