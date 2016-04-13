#ifndef FINITE_VOLUME_FINITE_VOLUME_D2DT2_SCHEMES_EULER_D2DT2_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_D2DT2_SCHEMES_EULER_D2DT2_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::EulerD2dt2Scheme
// Description
//   First-order Euler implicit d2dt2 using the current and two previous
//   time-step values.

#include "d2dt2_scheme.hpp"


namespace mousse {
namespace fv {

template<class Type>
class EulerD2dt2Scheme
:
  public fv::d2dt2Scheme<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("Euler");
  // Constructors
    //- Construct from mesh
    EulerD2dt2Scheme(const fvMesh& mesh)
    :
      d2dt2Scheme<Type>{mesh}
    {}
    //- Construct from mesh and Istream
    EulerD2dt2Scheme(const fvMesh& mesh, Istream& is)
    :
      d2dt2Scheme<Type>{mesh, is}
    {}
    //- Disallow default bitwise copy construct
    EulerD2dt2Scheme(const EulerD2dt2Scheme&) = delete;
    //- Disallow default bitwise assignment
    EulerD2dt2Scheme& operator=(const EulerD2dt2Scheme&) = delete;
  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return fv::d2dt2Scheme<Type>::mesh();
    }
    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcD2dt2
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcD2dt2
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type>> fvmD2dt2
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type>> fvmD2dt2
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type>> fvmD2dt2
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
};
}  // namespace fv
}  // namespace mousse

#include "euler_d2dt2_scheme.ipp"

#endif
