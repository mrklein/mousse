// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::VectorSpace
// Description
//   Templated vector space.
//   Template arguments are the Form the vector space will be used to create,
//   the type of the elements and the number of elements.
// SourceFiles
//   vector_space_i.hpp
//   vector_space.cpp
#ifndef vector_space_hpp_
#define vector_space_hpp_
#include "direction.hpp"
#include "scalar.hpp"
#include "word.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Form, class Cmpt, int nCmpt> class VectorSpace;
template<class Form, class Cmpt, int nCmpt>
Istream& operator>>
(
  Istream&,
  VectorSpace<Form, Cmpt, nCmpt>&
);
template<class Form, class Cmpt, int nCmpt>
Ostream& operator<<
(
  Ostream&,
  const VectorSpace<Form, Cmpt, nCmpt>&
);
template<class Form, class Cmpt, int nCmpt>
class VectorSpace
{
public:
  //- Component type
  typedef Cmpt cmptType;
  // Member constants
    enum
    {
      dim = 3,            //!< Dimensionality of space
      nComponents = nCmpt //!< Number of components in this vector space
    };
  // Static data members
    //- The components of this vector space
    Cmpt v_[nCmpt];
  // Constructors
    //- Construct null
    inline VectorSpace();
    //- Construct from Istream
    VectorSpace(Istream&);
    //- Construct as copy
    inline VectorSpace(const VectorSpace<Form, Cmpt, nCmpt>&);
    //- Construct as copy of another VectorSpace type of the same rank
    template<class Form2, class Cmpt2>
    inline VectorSpace(const VectorSpace<Form2, Cmpt2, nCmpt>&);
  // Member Functions
    //- Return the number of elements in the VectorSpace = nCmpt.
    inline label size() const;
    inline const Cmpt& component(const direction) const;
    inline Cmpt& component(const direction);
    inline void component(Cmpt&, const direction) const;
    inline void replace(const direction, const Cmpt&);
  // Member Operators
    inline const Cmpt& operator[](const direction) const;
    inline Cmpt& operator[](const direction);
    inline void operator=(const VectorSpace<Form, Cmpt, nCmpt>&);
    inline void operator+=(const VectorSpace<Form, Cmpt, nCmpt>&);
    inline void operator-=(const VectorSpace<Form, Cmpt, nCmpt>&);
    inline void operator*=(const scalar);
    inline void operator/=(const scalar);
  // IOstream Operators
    friend Istream& operator>> <Form, Cmpt, nCmpt>
    (
      Istream&,
      VectorSpace<Form, Cmpt, nCmpt>&
    );
    friend Ostream& operator<< <Form, Cmpt, nCmpt>
    (
      Ostream&,
      const VectorSpace<Form, Cmpt, nCmpt>&
    );
};
// Global functions 
//- Return a string representation of a VectorSpace
template<class Form, class Cmpt, int nCmpt>
word name(const VectorSpace<Form, Cmpt, nCmpt>&);
}  // namespace mousse
#include "vector_space_i.hpp"
#ifdef NoRepository
#   include "vector_space.cpp"
#endif
#endif
