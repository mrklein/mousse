// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linearUpwind
// Description
//   linearUpwind interpolation scheme class derived from upwind and returns
//   upwind weighting factors and also applies a gradient-based explicit
//   correction.
// SourceFiles
//   linear_upwind.cpp
#ifndef linear_upwind_hpp_
#define linear_upwind_hpp_
#include "upwind.hpp"
#include "gauss_grad.hpp"
namespace mousse
{
template<class Type>
class linearUpwind
:
  public upwind<Type>
{
  // Private Data
    word gradSchemeName_;
    tmp<fv::gradScheme<Type> > gradScheme_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    linearUpwind(const linearUpwind&);
    //- Disallow default bitwise assignment
    void operator=(const linearUpwind&);
public:
  //- Runtime type information
  TypeName("linearUpwind");
  // Constructors
    //- Construct from faceFlux
    linearUpwind
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux
    )
    :
      upwind<Type>(mesh, faceFlux),
      gradSchemeName_("grad"),
      gradScheme_
      (
        new fv::gaussGrad<Type>(mesh)
      )
    {}
    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    linearUpwind
    (
      const fvMesh& mesh,
      Istream& schemeData
    )
    :
      upwind<Type>(mesh, schemeData),
      gradSchemeName_(schemeData),
      gradScheme_
      (
        fv::gradScheme<Type>::New
        (
          mesh,
          mesh.gradScheme(gradSchemeName_)
        )
      )
    {}
    //- Construct from faceFlux and Istream
    linearUpwind
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    )
    :
      upwind<Type>(mesh, faceFlux, schemeData),
      gradSchemeName_(schemeData),
      gradScheme_
      (
        fv::gradScheme<Type>::New
        (
          mesh,
          mesh.gradScheme(gradSchemeName_)
        )
      )
    {}
  // Member Functions
    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return true;
    }
    //- Return the explicit correction to the face-interpolate
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};
}  // namespace mousse
#endif
