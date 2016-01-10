// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SphericalTensor2D
// Description
//   Templated 2D sphericalTensor derived from VectorSpace adding construction
//   from 1 component, element access using ii() member function and the
//   inner-product (dot-product) and outer-product operators.
// SourceFiles
//   _spherical_tensor_2d_i.hpp
#ifndef _spherical_tensor_2d_hpp_
#define _spherical_tensor_2d_hpp_
#include "vector_space.hpp"
namespace mousse
{
template<class Cmpt>
class SphericalTensor2D
:
  public VectorSpace<SphericalTensor2D<Cmpt>, Cmpt, 1>
{
public:
  // Member constants
    enum
    {
      rank = 2 // Rank of SphericalTensor2D is 2
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const SphericalTensor2D zero;
    static const SphericalTensor2D one;
    static const SphericalTensor2D max;
    static const SphericalTensor2D min;
    static const SphericalTensor2D I;
    static const SphericalTensor2D oneThirdI;
    static const SphericalTensor2D twoThirdsI;
  //- Component labeling enumeration
  enum components { II };
  // Constructors
    //- Construct null
    inline SphericalTensor2D();
    //- Construct given VectorSpace
    inline SphericalTensor2D
    (
      const VectorSpace<SphericalTensor2D<Cmpt>, Cmpt, 1>&
    );
    //- Construct given the component
    inline SphericalTensor2D(const Cmpt& tii);
    //- Construct from Istream
    inline SphericalTensor2D(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& ii() const;
      inline Cmpt& ii();
};
}  // namespace mousse
// Include inline implementations
#include "_spherical_tensor_2d_i.hpp"
#endif
