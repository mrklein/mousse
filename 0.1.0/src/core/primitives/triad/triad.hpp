// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::triad
// Description
//   Representation of a 3D Cartesian coordinate system as a Vector of vectors.
// See Also
//   mousse::quaternion
// SourceFiles
//   triad_i.hpp
//   triad.cpp
#ifndef triad_hpp_
#define triad_hpp_
#include "vector.hpp"
#include "tensor.hpp"
#include "contiguous.hpp"
namespace mousse
{
// Forward declaration of classes
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class triad;
Istream& operator>>(Istream&, triad&);
Ostream& operator<<(Ostream&, const triad&);
class quaternion;
class triad
:
  public Vector<vector>
{
public:
  // Constructors
    //- Construct null
    inline triad();
    //- Construct from components
    inline triad(const Vector<vector>& vv);
    //- Construct from coordinate axes
    inline triad(const vector& x, const vector& y, const vector& z);
    //- Construct from a primary axis with the other two unset
    inline triad(const vector& pa);
    //- Construct from a quaternion
    triad(const quaternion& q);
    //- Construct from a tensor
    triad(const tensor& t);
    //- Construct from Istream
    inline triad(Istream&);
  // Static data members
    static const triad zero;
    static const triad one;
    static const triad max;
    static const triad min;
    static const triad rootMax;
    static const triad rootMin;
    static const triad I;
    static const triad unset;
  // Member Functions
    //- Is the vector in the direction d set
    inline bool set(const direction d) const;
    //- Are all the vector set
    inline bool set() const;
    //- Return the primary direction of the vector v
    static inline direction primaryDirection(const vector& v);
    //- Return the vector orthogonal to the two provided
    static inline vector orthogonal(const vector& v1, const vector& v2);
    //- Orthogonalize this triad so that it is ortho-normal
    void orthogonalize();
    //- Normalize each set axis vector to have a unit magnitude
    void normalize();
    //- Align this triad with the given vector v
    //  by rotating the most aligned axis to be coincident with v
    void align(const vector& v);
    //- Sort the axes such that they are closest to the x, y and z axes
    triad sortxyz() const;
    //- Convert to a quaternion
    operator quaternion() const;
  // Member Operators
    inline void operator=(const Vector<vector>&);
    void operator=(const tensor& t);
    //- Add the triad t2 to this triad
    //  without normalizing or orthogonalizing
    void operator+=(const triad& t2);
  // IOstream Operators
    inline friend Istream& operator>>(Istream&, triad&);
    inline friend Ostream& operator<<(Ostream&, const triad&);
};
// Global Functions 
//- Return a quantity of the difference between two triads
scalar diff(const triad& A, const triad& B);
//- Data associated with quaternion type are contiguous
template<>
inline bool contiguous<triad>() {return true;}
}  // namespace mousse
#include "triad_i.hpp"
#endif
