// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DiagTensor
// Description
//   Templated 3D DiagTensor derived from VectorSpace.
//   Adding construction from 3 components, element access using xx(), yy()
//   and zz() member functions and the inner-product (dot-product) and
//   outer-product operators.
// SourceFiles
//   _diag_tensor_i.hpp
#ifndef _diag_tensor_hpp_
#define _diag_tensor_hpp_
#include "_tensor.hpp"
namespace mousse
{
template<class Cmpt>
class DiagTensor
:
  public VectorSpace<DiagTensor<Cmpt>, Cmpt, 3>
{
public:
  // Member constants
    enum
    {
      rank = 2 // Rank of DiagTensor is 2
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const DiagTensor zero;
    static const DiagTensor one;
    static const DiagTensor max;
    static const DiagTensor min;
  //- Component labeling enumeration
  enum components { XX, YY, ZZ };
  // Constructors
    //- Construct null
    inline DiagTensor();
    //- Construct given VectorSpace
    template<class Cmpt2>
    inline DiagTensor(const VectorSpace<DiagTensor<Cmpt2>, Cmpt2, 3>&);
    //- Construct given three components
    inline DiagTensor(const Cmpt& txx, const Cmpt& tyy, const Cmpt& tzz);
    //- Construct from Istream
    inline DiagTensor(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& xx() const;
      inline const Cmpt& yy() const;
      inline const Cmpt& zz() const;
      inline Cmpt& xx();
      inline Cmpt& yy();
      inline Cmpt& zz();
};
}  // namespace mousse
// Include inline implementations
#include "_diag_tensor_i.hpp"
#endif
