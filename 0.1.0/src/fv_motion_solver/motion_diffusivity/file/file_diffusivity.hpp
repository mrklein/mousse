// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileDiffusivity
// Description
//   Motion diffusivity read from given file name.
// SourceFiles
//   file_diffusivity.cpp
#ifndef file_diffusivity_hpp_
#define file_diffusivity_hpp_
#include "motion_diffusivity.hpp"
#include "surface_fields.hpp"
namespace mousse
{
class fileDiffusivity
:
  public motionDiffusivity
{
protected:
  // Protected data
    surfaceScalarField faceDiffusivity_;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    fileDiffusivity(const fileDiffusivity&);
    //- Disallow default bitwise assignment
    void operator=(const fileDiffusivity&);
public:
  //- Runtime type information
  TypeName("file");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    fileDiffusivity(const fvMesh& mesh, Istream& mdData);
  //- Destructor
  virtual ~fileDiffusivity();
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
