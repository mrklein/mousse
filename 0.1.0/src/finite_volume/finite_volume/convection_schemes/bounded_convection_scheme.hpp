#ifndef FINITE_VOLUME_FINITE_VOLUME_CONVECTION_SCHEMES_BOUNDED_CONVECTION_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_CONVECTION_SCHEMES_BOUNDED_CONVECTION_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::boundedConvectionScheme
// Description
//   Bounded form of the selected convection scheme.
//   Boundedness is achieved by subtracting div(phi)*vf or Sp(div(phi), vf)
//   which is non-conservative if div(phi) != 0 but conservative otherwise.
//   Can be used for convection of bounded scalar properties in steady-state
//   solvers to improve stability if insufficient convergence of the pressure
//   equation causes temporary divergence of the flux field.

#include "convection_scheme.hpp"


namespace mousse {
namespace fv {

template<class Type>
class boundedConvectionScheme
:
  public fv::convectionScheme<Type>
{
  // Private data
    tmp<fv::convectionScheme<Type>> scheme_;
public:
  //- Runtime type information
  TYPE_NAME("bounded");
  // Constructors
    //- Construct from flux and Istream
    boundedConvectionScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      convectionScheme<Type>{mesh, faceFlux},
      scheme_
      {
        fv::convectionScheme<Type>::New(mesh, faceFlux, is)
      }
    {}
    //- Disallow default bitwise copy construct
    boundedConvectionScheme(const boundedConvectionScheme&) = delete;
    //- Disallow default bitwise assignment
    boundedConvectionScheme& operator=(const boundedConvectionScheme&) = delete;
  // Member Functions
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> interpolate
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> flux
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    tmp<fvMatrix<Type>> fvmDiv
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcDiv
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};
}  // namespace fv
}  // namespace mousse

#include "bounded_convection_scheme.ipp"

#endif
