// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Tensor2D
// Description
//   Templated 2D tensor derived from VectorSpace adding construction from
//   4 components, element access using xx(), xy(), yx() and yy() member
//   functions and the iner-product (dot-product) and outer-product of two
//   Vector2Ds (tensor-product) operators.
// SourceFiles
//   _tensor_2d_i.hpp
#ifndef _tensor2_d_hpp_
#define _tensor2_d_hpp_
#include "_vector_2d.hpp"
#include "_spherical_tensor_2d.hpp"
namespace mousse
{
template<class Cmpt>
class SymmTensor2D;
template<class Cmpt>
class Tensor2D
:
  public VectorSpace<Tensor2D<Cmpt>, Cmpt, 4>
{
public:
  // Member constants
    enum
    {
      rank = 2 // Rank of Tensor2D is 2
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const Tensor2D zero;
    static const Tensor2D one;
    static const Tensor2D max;
    static const Tensor2D min;
    static const Tensor2D I;
  //- Component labeling enumeration
  enum components { XX, XY, YX, YY };
  // Constructors
    //- Construct null
    inline Tensor2D();
    //- Construct given VectorSpace
    inline Tensor2D(const VectorSpace<Tensor2D<Cmpt>, Cmpt, 4>&);
    //- Construct given SymmTensor2D
    inline Tensor2D(const SymmTensor2D<Cmpt>&);
    //- Construct given SphericalTensor2D
    inline Tensor2D(const SphericalTensor2D<Cmpt>&);
    //- Construct given the two vectors
    inline Tensor2D
    (
      const Vector2D<Cmpt>& x,
      const Vector2D<Cmpt>& y
    );
    //- Construct given the four components
    inline Tensor2D
    (
      const Cmpt txx, const Cmpt txy,
      const Cmpt tyx, const Cmpt tyy
    );
    //- Construct from Istream
    Tensor2D(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& xx() const;
      inline const Cmpt& xy() const;
      inline const Cmpt& yx() const;
      inline const Cmpt& yy() const;
      inline Cmpt& xx();
      inline Cmpt& xy();
      inline Cmpt& yx();
      inline Cmpt& yy();
      // Access vector components.
      inline Vector2D<Cmpt> x() const;
      inline Vector2D<Cmpt> y() const;
    //- Transpose
    inline Tensor2D<Cmpt> T() const;
  // Member Operators
    //- Copy SymmTensor2D
    inline void operator=(const SymmTensor2D<Cmpt>&);
    //- Copy SphericalTensor2D
    inline void operator=(const SphericalTensor2D<Cmpt>&);
};
}  // namespace mousse
// Include inline implementations
#include "_tensor_2d_i.hpp"
#endif
