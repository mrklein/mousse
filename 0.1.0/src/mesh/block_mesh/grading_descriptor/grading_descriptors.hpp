#ifndef MESH_BLOCK_MESH_GRADING_DESCRIPTOR_GRADING_DESCRIPTORS_HPP_
#define MESH_BLOCK_MESH_GRADING_DESCRIPTOR_GRADING_DESCRIPTORS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::gradingDescriptors
// Description
//   List of gradingDescriptor for the sections of a block with additional IO
//   functionality
// SourceFiles
//   grading_descriptors.cpp
#include "grading_descriptor.hpp"
#include "list.hpp"
namespace mousse
{
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class gradingDescriptors;
Istream& operator>>(Istream&, gradingDescriptors&);
class gradingDescriptors
:
  public List<gradingDescriptor>
{
public:
  // Constructors
    //- Default constructor
    gradingDescriptors();
    //- Construct from a gradingDescriptor
    gradingDescriptors(const gradingDescriptor& gd);
  // Member functions
    //- Return the inverse gradingDescriptors with 1/expansionRatio
    gradingDescriptors inv() const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, gradingDescriptors&);
};
}  // namespace mousse
#endif
