// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::directionalDiffusivity
// Description
//   Directional finite volume mesh motion diffusivity.
// SourceFiles
//   directional_diffusivity.cpp
#ifndef directional_diffusivity_hpp_
#define directional_diffusivity_hpp_
#include "uniform_diffusivity.hpp"
namespace mousse
{
class directionalDiffusivity
:
  public uniformDiffusivity
{
  // Private data
    vector diffusivityVector_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    directionalDiffusivity(const directionalDiffusivity&);
    //- Disallow default bitwise assignment
    void operator=(const directionalDiffusivity&);
public:
  //- Runtime type information
  TypeName("directional");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    directionalDiffusivity(const fvMesh& mesh, Istream& mdData);
  //- Destructor
  virtual ~directionalDiffusivity();
  // Member Functions
    //- Correct the motion diffusivity
    virtual void correct();
};
}  // namespace mousse
#endif
