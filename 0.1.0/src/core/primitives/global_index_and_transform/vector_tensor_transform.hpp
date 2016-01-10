// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::vectorTensorTransform
// Description
//   Vector-tensor class used to perform translations and rotations in
//   3D space.
// SourceFiles
//   vector_tensor_transform_i.hpp
//   vector_tensor_transform.cpp
//   vector_tensor_transform_templates.cpp
#ifndef vector_tensor_transform_hpp_
#define vector_tensor_transform_hpp_
#include "tensor.hpp"
#include "word.hpp"
#include "contiguous.hpp"
#include "point_field.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class vectorTensorTransform;
Istream& operator>>(Istream& is, vectorTensorTransform&);
Ostream& operator<<(Ostream& os, const vectorTensorTransform& C);
class vectorTensorTransform
{
  // private data
    //- Translation vector
    vector t_;
    //- Rotation tensor
    tensor R_;
    //- Recording if the transform has non-identity transform to
    //  allow its calculation to be skipped, which is the majority
    //  of the expected cases
    bool hasR_;
public:
  // Static data members
    static const char* const typeName;
    static const vectorTensorTransform zero;
    static const vectorTensorTransform I;
  // Constructors
    //- Construct null
    inline vectorTensorTransform();
    //- Construct given a translation vector, rotation tensor and
    //  hasR bool
    inline vectorTensorTransform
    (
      const vector& t,
      const tensor& R,
      bool hasR = true
    );
    //- Construct a pure translation vectorTensorTransform given a
    //  translation vector
    inline explicit vectorTensorTransform(const vector& t);
    //- Construct a pure rotation vectorTensorTransform given a
    //  rotation tensor
    inline explicit vectorTensorTransform(const tensor& R);
    //- Construct from Istream
    vectorTensorTransform(Istream&);
  // Member functions
    // Access
      inline const vector& t() const;
      inline const tensor& R() const;
      inline bool hasR() const;
    // Edit
      inline vector& t();
      inline tensor& R();
    // Transform
      //- Transform the given position
      inline vector transformPosition(const vector& v) const;
      //- Transform the given pointField
      inline pointField transformPosition(const pointField& pts) const;
      //- Inverse transform the given position
      inline vector invTransformPosition(const vector& v) const;
      //- Inverse transform the given pointField
      inline pointField invTransformPosition(const pointField& pts) const;
      //- Transform the given field
      template<class Type>
      tmp<Field<Type> > transform(const Field<Type>&) const;
  // Member operators
    inline void operator=(const vectorTensorTransform&);
    inline void operator&=(const vectorTensorTransform&);
    inline void operator=(const vector&);
    inline void operator+=(const vector&);
    inline void operator-=(const vector&);
    inline void operator=(const tensor&);
    inline void operator&=(const tensor&);
  // IOstream operators
    friend Istream& operator>>(Istream& is, vectorTensorTransform&);
    friend Ostream& operator<<(Ostream& os, const vectorTensorTransform&);
};
// Global Functions 
//- Return the inverse of the given vectorTensorTransform
inline vectorTensorTransform inv(const vectorTensorTransform& tr);
//- Return a string representation of a vectorTensorTransform
word name(const vectorTensorTransform&);
//- Data associated with vectorTensorTransform type are contiguous
template<>
inline bool contiguous<vectorTensorTransform>() {return true;}
//- Template specialisations
template<>
tmp<Field<bool> > vectorTensorTransform::transform(const Field<bool>&) const;
template<>
tmp<Field<label> > vectorTensorTransform::transform(const Field<label>&) const;
template<>
tmp<Field<scalar> > vectorTensorTransform::transform(const Field<scalar>&)
const;
// Global Operators 
inline bool operator==
(
  const vectorTensorTransform& tr1,
  const vectorTensorTransform& tr2
);
inline bool operator!=
(
  const vectorTensorTransform& tr1,
  const vectorTensorTransform& tr2
);
inline vectorTensorTransform operator+
(
  const vectorTensorTransform& tr,
  const vector& t
);
inline vectorTensorTransform operator+
(
  const vector& t,
  const vectorTensorTransform& tr
);
inline vectorTensorTransform operator-
(
  const vectorTensorTransform& tr,
  const vector& t
);
inline vectorTensorTransform operator&
(
  const vectorTensorTransform& tr1,
  const vectorTensorTransform& tr2
);
}  // namespace mousse
#include "vector_tensor_transform_i.hpp"
#ifdef NoRepository
#   include "vector_tensor_transform_templates.cpp"
#endif
#endif
