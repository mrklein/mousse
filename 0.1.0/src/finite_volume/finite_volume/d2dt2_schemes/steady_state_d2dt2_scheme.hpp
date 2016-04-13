#ifndef FINITE_VOLUME_FINITE_VOLUME_D2DT2_SCHEMES_STEADY_STATE_D2DT2_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_D2DT2_SCHEMES_STEADY_STATE_D2DT2_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::steadyStateD2dt2Scheme
// Description
//   SteadyState d2dt2 which returns 0.

#include "d2dt2_scheme.hpp"


namespace mousse {
namespace fv {

template<class Type>
class steadyStateD2dt2Scheme
:
  public fv::d2dt2Scheme<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("steadyState");
  // Constructors
    //- Construct from mesh
    steadyStateD2dt2Scheme(const fvMesh& mesh)
    :
      d2dt2Scheme<Type>{mesh}
    {}
    //- Construct from mesh and Istream
    steadyStateD2dt2Scheme(const fvMesh& mesh, Istream& is)
    :
      d2dt2Scheme<Type>{mesh, is}
    {}
    //- Disallow default bitwise copy construct
    steadyStateD2dt2Scheme(const steadyStateD2dt2Scheme&) = delete;
    //- Disallow default bitwise assignment
    steadyStateD2dt2Scheme& operator=(const steadyStateD2dt2Scheme&) = delete;
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

#include "steady_state_d2dt2_scheme.ipp"

#endif
