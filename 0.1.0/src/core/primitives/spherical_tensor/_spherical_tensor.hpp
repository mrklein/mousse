// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SphericalTensor
// Description
//   Templated 3D SphericalTensor derived from VectorSpace adding construction
//   from 1 component, element access using th ii() member function and the
//   inner-product (dot-product) and outer-product operators.
// SourceFiles
//   _spherical_tensor_i.hpp
#ifndef _spherical_tensor_hpp_
#define _spherical_tensor_hpp_
#include "vector_space.hpp"
namespace mousse
{
template<class Cmpt>
class SphericalTensor
:
  public VectorSpace<SphericalTensor<Cmpt>, Cmpt, 1>
{
public:
  //- Equivalent type of labels used for valid component indexing
  typedef SphericalTensor<label> labelType;
  // Member constants
    enum
    {
      rank = 2 // Rank of SphericalTensor is 2
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const SphericalTensor zero;
    static const SphericalTensor one;
    static const SphericalTensor max;
    static const SphericalTensor min;
    static const SphericalTensor I;
    static const SphericalTensor oneThirdI;
    static const SphericalTensor twoThirdsI;
  //- Component labeling enumeration
  enum components { II };
  // Constructors
    //- Construct null
    inline SphericalTensor();
    //- Construct given VectorSpace
    template<class Cmpt2>
    inline SphericalTensor
    (
      const VectorSpace<SphericalTensor<Cmpt2>, Cmpt2, 1>&
    );
    //- Construct given the component
    inline SphericalTensor(const Cmpt& tii);
    //- Construct from Istream
    inline SphericalTensor(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& ii() const;
      inline Cmpt& ii();
    //- Transpose
    inline const SphericalTensor<Cmpt>& T() const;
};
}  // namespace mousse
// Include inline implementations
#include "_spherical_tensor_i.hpp"
#endif
