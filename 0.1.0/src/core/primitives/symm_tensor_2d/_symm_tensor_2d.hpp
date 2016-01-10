// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SymmTensor2D
// Description
//   Templated 2D symmetric tensor derived from VectorSpace adding construction
//   from 4 components, element access using xx(), xy() etc. member functions
//   and the inner-product (dot-product) and outer-product of two Vectors
//   (tensor-product) operators.
// SourceFiles
//   _symm_tensor_2d_i.hpp
#ifndef _symm_tensor_2d_hpp_
#define _symm_tensor_2d_hpp_
#include "vector_space.hpp"
#include "_spherical_tensor_2d.hpp"
namespace mousse
{
template<class Cmpt>
class SymmTensor2D
:
  public VectorSpace<SymmTensor2D<Cmpt>, Cmpt, 3>
{
public:
  //- Equivalent type of labels used for valid component indexing
  typedef SymmTensor2D<label> labelType;
  // Member constants
    enum
    {
      rank = 2 // Rank of SymmTensor2D is 2
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const SymmTensor2D zero;
    static const SymmTensor2D one;
    static const SymmTensor2D max;
    static const SymmTensor2D min;
    static const SymmTensor2D I;
  //- Component labeling enumeration
  enum components { XX, XY, YY };
  // Constructors
    //- Construct null
    inline SymmTensor2D();
    //- Construct given VectorSpace
    inline SymmTensor2D(const VectorSpace<SymmTensor2D<Cmpt>, Cmpt, 3>&);
    //- Construct given SphericalTensor
    inline SymmTensor2D(const SphericalTensor2D<Cmpt>&);
    //- Construct given the three components
    inline SymmTensor2D
    (
      const Cmpt txx, const Cmpt txy,
              const Cmpt tyy
    );
    //- Construct from Istream
    SymmTensor2D(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& xx() const;
      inline const Cmpt& xy() const;
      inline const Cmpt& yy() const;
      inline Cmpt& xx();
      inline Cmpt& xy();
      inline Cmpt& yy();
    //- Transpose
    inline const SymmTensor2D<Cmpt>& T() const;
  // Member Operators
    //- Construct given SphericalTensor2D
    inline void operator=(const SphericalTensor2D<Cmpt>&);
};
}  // namespace mousse
// Include inline implementations
#include "_symm_tensor_2d_i.hpp"
#endif
