// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::forceSuSp
// Description
//   Helper container for force Su and Sp terms.
//     F = Sp(U - Up) + Su
//   Explicit contribution, Su specified as a force
//   Implicit coefficient, Sp specified as force/velocity
// SourceFiles
//   force_su_sp_i.hpp
#ifndef force_su_sp_hpp_
#define force_su_sp_hpp_
#include "tuple2.hpp"
namespace mousse
{
// Forward declaration of friend functions
class forceSuSp;
inline forceSuSp operator+(const forceSuSp& susp1, const forceSuSp& susp2);
inline forceSuSp operator*(const forceSuSp& susp, const scalar s);
class forceSuSp
:
  public Tuple2<vector, scalar>
{
public:
  // Constructors
    //- Construct null
    inline forceSuSp();
    //- Construct given Tuple2
    inline forceSuSp(const Tuple2<vector, scalar>& susp);
    //- Construct given two components
    inline forceSuSp(const vector& Su, const scalar Sp);
    //- Construct from Istream
    inline forceSuSp(Istream&);
  // Member Functions
    // Access
      //- Return const access to the explicit contribution [kg.m/s2]
      inline const vector& Su() const;
      //- Return const access to the implicit coefficient [kg/s]
      inline scalar Sp() const;
    // Edit
      //- Return reference to the explicit contribution
      inline vector& Su();
      //- Return reference to the implicit coefficient
      inline scalar& Sp();
    // Operators
      //- Assignment
      inline void operator=(const forceSuSp& susp);
      //- Addition
      inline void operator+=(const forceSuSp& susp);
      //- Subtraction
      inline void operator-=(const forceSuSp& susp);
    // Friend operators
      //- Addition
      friend inline forceSuSp operator*
      (
        const forceSuSp& susp1,
        const forceSuSp& susp2
      );
      //- Multiplication
      friend inline forceSuSp operator*
      (
        const scalar s,
        const forceSuSp& susp
      );
};
}  // namespace mousse
#include "force_su_sp_i.hpp"
#endif
