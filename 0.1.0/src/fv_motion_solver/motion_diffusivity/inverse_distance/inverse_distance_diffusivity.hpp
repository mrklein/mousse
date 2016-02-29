#ifndef FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_INVERSE_DISTANCE_INVERSE_DISTANCE_DIFFUSIVITY_HPP_
#define FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_INVERSE_DISTANCE_INVERSE_DISTANCE_DIFFUSIVITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::inverseDistanceDiffusivity
// Description
//   Inverse distance to the given patches motion diffusivity.
// SourceFiles
//   inverse_distance_diffusivity.cpp
#include "uniform_diffusivity.hpp"
#include "word_re_list.hpp"
namespace mousse
{
class inverseDistanceDiffusivity
:
  public uniformDiffusivity
{
  // Private data
    //- Patches selected to base the distance on
    //  These can contain patch names or regular expressions to search for.
    wordReList patchNames_;
  // Private Member Functions
    //- Return patch-cell-centre distance field
    tmp<scalarField> y() const;
public:
  //- Runtime type information
  TYPE_NAME("inverseDistance");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    inverseDistanceDiffusivity(const fvMesh& mesh, Istream& mdData);
    //- Disallow default bitwise copy construct
    inverseDistanceDiffusivity(const inverseDistanceDiffusivity&) = delete;
    //- Disallow default bitwise assignment
    inverseDistanceDiffusivity& operator=
    (
      const inverseDistanceDiffusivity&
    ) = delete;
  //- Destructor
  virtual ~inverseDistanceDiffusivity();
  // Member Functions
    //- Correct the motion diffusivity
    virtual void correct();
};
}  // namespace mousse
#endif
