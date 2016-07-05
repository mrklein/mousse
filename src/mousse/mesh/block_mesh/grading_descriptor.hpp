#ifndef MESH_BLOCK_MESH_GRADING_DESCRIPTOR_GRADING_DESCRIPTOR_HPP_
#define MESH_BLOCK_MESH_GRADING_DESCRIPTOR_GRADING_DESCRIPTOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::gradingDescriptor
// Description
//   Handles the specification for grading within a section of a block
//   blockFraction: the fraction of the block the section occupies
//   nDivFraction: the fraction of the divisions of the block allocated to
//     the section
//   expansionRatio: the expansions ratio for the grading with the section of
//     block defined as the ratio of the size of the division at either and
//     of the section.

#include "scalar.hpp"


namespace mousse {

class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class gradingDescriptor;
class gradingDescriptors;
Istream& operator>>(Istream&, gradingDescriptor&);
Ostream& operator<<(Ostream&, const gradingDescriptor&);


class gradingDescriptor
{
  // Private data
    scalar blockFraction_;
    scalar nDivFraction_;
    scalar expansionRatio_;
public:
  friend class gradingDescriptors;
  // Constructors
    //- Default constructor
    gradingDescriptor();
    //- Construct from components
    gradingDescriptor
    (
      const scalar blockFraction,
      const scalar nDivFraction,
      const scalar expansionRatio
    );
    //- Construct from expansionRatio
    gradingDescriptor
    (
      const scalar expansionRatio
    );
    //- Construct from Istream
    gradingDescriptor(Istream&);
  //- Destructor
  ~gradingDescriptor();
  // Member Functions
    // Access
      scalar blockFraction() const { return blockFraction_; }
      scalar nDivFraction() const { return nDivFraction_; }
      scalar expansionRatio() const { return expansionRatio_; }
  // Member functions
    //- Return the inverse gradingDescriptor with 1/expansionRatio
    gradingDescriptor inv() const;
  // Member operators
    bool operator==(const gradingDescriptor&) const;
    bool operator!=(const gradingDescriptor&) const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, gradingDescriptor&);
    friend Ostream& operator<<(Ostream&, const gradingDescriptor&);
    friend Istream& operator>>(Istream&, gradingDescriptors&);
};

}  // namespace mousse

#endif

