// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformDiffusivity
// Description
//   Uniform uniform finite volume mesh motion diffusivity.
// SourceFiles
//   uniform_diffusivity.cpp
#ifndef uniform_diffusivity_hpp_
#define uniform_diffusivity_hpp_
#include "motion_diffusivity.hpp"
#include "surface_fields.hpp"
namespace mousse
{
class uniformDiffusivity
:
  public motionDiffusivity
{
protected:
  // Protected data
    surfaceScalarField faceDiffusivity_;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    uniformDiffusivity(const uniformDiffusivity&);
    //- Disallow default bitwise assignment
    void operator=(const uniformDiffusivity&);
public:
  //- Runtime type information
  TypeName("uniform");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    uniformDiffusivity(const fvMesh& mesh, Istream& mdData);
  //- Destructor
  virtual ~uniformDiffusivity();
  // Member Functions
    //- Return diffusivity field
    virtual tmp<surfaceScalarField> operator()() const
    {
      return faceDiffusivity_;
    }
    //- Do not correct the motion diffusivity
    virtual void correct()
    {}
};
}  // namespace mousse
#endif
