#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_CONSTRAINT_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_CONSTRAINT_HPP_

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

#include "label.hpp"
#include "vector.hpp"
#include "tuple2.hpp"
#include "transform.hpp"
#include "fixed_list.hpp"


namespace mousse {

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

template<> inline bool contiguous<pointConstraint>() {return true;}

}  // namespace mousse


// Constructors 

inline mousse::pointConstraint::pointConstraint()
:
  Tuple2<label, vector>{0, vector::zero}
{}


inline mousse::pointConstraint::pointConstraint(const Tuple2<label, vector>& pc)
:
  Tuple2<label, vector>{pc}
{}


inline mousse::pointConstraint::pointConstraint(Istream& is)
:
  Tuple2<label, vector>{is}
{}


// Member Functions 
void mousse::pointConstraint::applyConstraint(const vector& cd)
{
  if (first() == 0) {
    first() = 1;
    second() = cd;
  } else if (first() == 1) {
    vector planeNormal = cd ^ second();
    scalar magPlaneNormal = mag(planeNormal);
    if (magPlaneNormal > 1e-3) {
      first() = 2;
      second() = planeNormal/magPlaneNormal;
    }
  } else if (first() == 2) {
    if (mag(cd & second()) > 1e-3) {
      first() = 3;
      second() = vector::zero;
    }
  }
}


void mousse::pointConstraint::combine(const pointConstraint& pc)
{
  if (first() == 0) {
    operator=(pc);
  } else if (first() == 1) {
    // Save single normal
    vector n = second();
    // Apply to supplied point constaint
    operator=(pc);
    applyConstraint(n);
  } else if (first() == 2) {
    if (pc.first() == 0)
    {}
    else if (pc.first() == 1) {
      applyConstraint(pc.second());
    } else if (pc.first() == 2) {
      // Both constrained to line. Same (+-)direction?
      if (mag(second() & pc.second()) <= (1.0-1e-3)) {
        // Different directions
        first() = 3;
        second() = vector::zero;
      }
    } else {
      first() = 3;
      second() = vector::zero;
    }
  }
}


mousse::tensor mousse::pointConstraint::constraintTransformation() const
{
  if (first() == 0) {
    return I;
  } else if (first() == 1) {
    return I - sqr(second());
  } else if (first() == 2) {
    return sqr(second());
  } else {
    return tensor::zero;
  }
}


void mousse::pointConstraint::unconstrainedDirections(label& n, tensor& tt)
const
{
  n = 3-first();
  FixedList<vector, 3> vecs;
  if (first() == 0) {
    vecs[0] = {1, 0, 0};
    vecs[1] = {0, 1, 0};
    vecs[2] = {0, 0, 1};
  } else if (first() == 1) {
    const vector& planeDir = second();
    vecs[0] = vector{1, 0, 0} - planeDir.x()*planeDir;
    if (mag(vecs[0].x()) < 1e-3) {
      vecs[0] = vector{0, 1, 0} - planeDir.y()*planeDir;
    }
    vecs[0] /= mag(vecs[0]);
    vecs[1] = vecs[0] ^ planeDir;
    vecs[1] /= mag(vecs[1]);
  } else if (first() == 2) {
    vecs[0] = second();
  }
  // Knock out remaining vectors
  for (direction dir = n; dir < vecs.size(); dir++) {
    vecs[dir] = vector::zero;
  }
  tt = tensor(vecs[0], vecs[1], vecs[2]);
}


void mousse::combineConstraintsEqOp::operator()
(
  pointConstraint& x,
  const pointConstraint& y
) const
{
  x.combine(y);
}


mousse::pointConstraint mousse::transform
(
  const tensor& tt,
  const pointConstraint& v
)
{
  return pointConstraint
  (
    Tuple2<label, vector>{v.first(), transform(tt, v.second())}
  );
}

#endif
