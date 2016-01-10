// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointConstraint
// Description
//   Accumulates point constraints through successive applications of the
//   applyConstraint function.
//   After all the constraints have been entered the resulting
//   transformation tensor is returned by the constraintTransformation
//   function.
// SourceFiles
//   point_constraint_i.hpp
#ifndef point_constraint_hpp_
#define point_constraint_hpp_
#include "label.hpp"
#include "vector.hpp"
#include "tuple2.hpp"
#include "transform.hpp"
namespace mousse
{
class pointConstraint
:
  public Tuple2<label, vector>
{
public:
  // Constructors
    //- Construct null
    inline pointConstraint();
    //- Construct from components
    inline pointConstraint(const Tuple2<label, vector>&);
    //- Construct from Istream
    inline pointConstraint(Istream&);
  // Member Functions
    //- Apply and accumulate the effect of the given constraint direction
    inline void applyConstraint(const vector& cd);
    //- Combine constraints
    inline void combine(const pointConstraint&);
    //- Return the accumulated constraint transformation tensor
    inline tensor constraintTransformation() const;
    //- Return the accumulated unconstrained directions. Directions
    //  coded as first n rows of tensor.
    inline void unconstrainedDirections(label& n, tensor& vecs) const;
};
//- Reduce operator
class combineConstraintsEqOp
{
public:
  inline void operator()(pointConstraint&, const pointConstraint&) const;
};
//- Transformation function
inline pointConstraint transform(const tensor& tt, const pointConstraint& v);
//- contiguous
template<class T> bool contiguous();
template<>
inline bool contiguous<pointConstraint>()                {return true;}
}  // namespace mousse
#include "point_constraint_i.hpp"
#endif
