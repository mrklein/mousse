// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "coupled_fv_patch_field.hpp"


namespace mousse {

// Selectors
template<class Type>
tmp<limitedSurfaceInterpolationScheme<Type> >
limitedSurfaceInterpolationScheme<Type>::New
(
  const fvMesh& mesh,
  Istream& schemeData
)
{
  if (surfaceInterpolation::debug) {
    Info << "limitedSurfaceInterpolationScheme<Type>::"
            "New(const fvMesh&, Istream&)"
            " : constructing limitedSurfaceInterpolationScheme<Type>"
      << endl;
  }
  if (schemeData.eof()) {
    FATAL_IO_ERROR_IN
    (
      "limitedSurfaceInterpolationScheme<Type>::"
      "New(const fvMesh&, Istream&)",
      schemeData
    )
    << "Discretisation scheme not specified"
    << endl << endl
    << "Valid schemes are :" << endl
    << MeshConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  const word schemeName{schemeData};
  typename MeshConstructorTable::iterator constructorIter =
    MeshConstructorTablePtr_->find(schemeName);
  if (constructorIter == MeshConstructorTablePtr_->end()) {
    FATAL_IO_ERROR_IN
    (
      "limitedSurfaceInterpolationScheme<Type>::"
      "New(const fvMesh&, Istream&)",
      schemeData
    )
    << "Unknown discretisation scheme "
    << schemeName << nl << nl
    << "Valid schemes are :" << endl
    << MeshConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  return constructorIter()(mesh, schemeData);
}


// Return weighting factors for scheme given by name in dictionary
template<class Type>
tmp<limitedSurfaceInterpolationScheme<Type> >
limitedSurfaceInterpolationScheme<Type>::New
(
  const fvMesh& mesh,
  const surfaceScalarField& faceFlux,
  Istream& schemeData
)
{
  if (surfaceInterpolation::debug) {
    Info << "limitedSurfaceInterpolationScheme<Type>::New"
            "(const fvMesh&, const surfaceScalarField&, Istream&) : "
            "constructing limitedSurfaceInterpolationScheme<Type>"
      << endl;
  }
  if (schemeData.eof()) {
    FATAL_IO_ERROR_IN
    (
      "limitedSurfaceInterpolationScheme<Type>::New"
      "(const fvMesh&, const surfaceScalarField&, Istream&)",
      schemeData
    )
    << "Discretisation scheme not specified"
    << endl << endl
    << "Valid schemes are :" << endl
    << MeshConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  const word schemeName{schemeData};
  typename MeshFluxConstructorTable::iterator constructorIter =
    MeshFluxConstructorTablePtr_->find(schemeName);
  if (constructorIter == MeshFluxConstructorTablePtr_->end()) {
    FATAL_IO_ERROR_IN
    (
      "limitedSurfaceInterpolationScheme<Type>::New"
      "(const fvMesh&, const surfaceScalarField&, Istream&)",
      schemeData
    )
    << "Unknown discretisation scheme "
    << schemeName << nl << nl
    << "Valid schemes are :" << endl
    << MeshFluxConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  return constructorIter()(mesh, faceFlux, schemeData);
}


// Destructor 
template<class Type>
limitedSurfaceInterpolationScheme<Type>::~limitedSurfaceInterpolationScheme()
{}


// Member Functions 
template<class Type>
tmp<surfaceScalarField> limitedSurfaceInterpolationScheme<Type>::weights
(
  const GeometricField<Type, fvPatchField, volMesh>& /*phi*/,
  const surfaceScalarField& CDweights,
  tmp<surfaceScalarField> tLimiter
) const
{
  // Note that here the weights field is initialised as the limiter
  // from which the weight is calculated using the limiter value
  surfaceScalarField& Weights = tLimiter();
  scalarField& pWeights = Weights.internalField();
  FOR_ALL(pWeights, face) {
    pWeights[face] = pWeights[face]*CDweights[face]
      + (1.0 - pWeights[face])*pos(faceFlux_[face]);
  }
  surfaceScalarField::GeometricBoundaryField& bWeights =
    Weights.boundaryField();
  FOR_ALL(bWeights, patchI) {
    scalarField& pWeights = bWeights[patchI];
    const scalarField& pCDweights = CDweights.boundaryField()[patchI];
    const scalarField& pFaceFlux = faceFlux_.boundaryField()[patchI];
    FOR_ALL(pWeights, face) {
      pWeights[face] = pWeights[face]*pCDweights[face]
        + (1.0 - pWeights[face])*pos(pFaceFlux[face]);
    }
  }
  return tLimiter;
}


template<class Type>
tmp<surfaceScalarField> limitedSurfaceInterpolationScheme<Type>::weights
(
  const GeometricField<Type, fvPatchField, volMesh>& phi
) const
{
  return this->weights
  (
    phi,
    this->mesh().surfaceInterpolation::weights(),
    this->limiter(phi)
  );
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
limitedSurfaceInterpolationScheme<Type>::flux
(
  const GeometricField<Type, fvPatchField, volMesh>& phi
) const
{
  return faceFlux_*this->interpolate(phi);
}

}  // namespace mousse

