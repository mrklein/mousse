// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LUST
// Description
//   LUST: Linear-upwind stabilised transport.
//   Interpolation scheme class derived from linearUpwind which returns blended
//   linear/linear-upwind weighting factors and also applies a explicit
//   gradient-based correction obtained from the linearUpwind scheme.  The
//   blending-factor is set to 0.75 linear which optimises the balance between
//   accuracy and stability on a range of LES cases with a range of mesh quality.
// SourceFiles
//   lust.cpp
#ifndef lust_hpp_
#define lust_hpp_
#include "linear_upwind.hpp"
namespace mousse
{
template<class Type>
class LUST
:
  public linearUpwind<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("LUST");
  // Constructors
    //- Construct from mesh and Istream
    LUST
    (
      const fvMesh& mesh,
      Istream& schemeData
    )
    :
      linearUpwind<Type>{mesh, schemeData}
    {}
    //- Construct from mesh, faceFlux and Istream
    LUST
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    )
    :
      linearUpwind<Type>{mesh, faceFlux, schemeData}
    {}
    //- Disallow default bitwise copy construct
    LUST(const LUST&) = delete;
    //- Disallow default bitwise assignment
    LUST& operator=(const LUST&) = delete;
  // Member Functions
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      return 0.75*this->mesh().surfaceInterpolation::weights()
        + 0.25*linearUpwind<Type>::weights();
    }
    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return true;
    }
    //- Return the explicit correction to the face-interpolate
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      return 0.25*linearUpwind<Type>::correction(vf);
    }
};
}  // namespace mousse
#endif
