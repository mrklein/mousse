// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lineDivide
// Description
//   Divides a line into segments
// SourceFiles
//   line_divide.cpp
#ifndef line_divide_hpp_
#define line_divide_hpp_
#include "point_field.hpp"
#include "scalar_list.hpp"
#include "grading_descriptors.hpp"
namespace mousse
{
class curvedEdge;
class lineDivide
{
  // Private data
    pointField points_;
    scalarList divisions_;
public:
  // Constructors
    //- Construct from components
    lineDivide
    (
      const curvedEdge&,
      const label ndiv,
      const gradingDescriptors& gd = gradingDescriptors()
    );
  // Member Functions
    //- Return the points
    const pointField& points() const;
    //- Return the list of lambda values
    const scalarList& lambdaDivisions() const;
};
}  // namespace mousse
#endif
