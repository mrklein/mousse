if (!isA<zeroGradientFvPatchScalarField>(k_.boundaryField()[patchi])) {
  FATAL_ERROR_IN("wall-function evaluation")
    << k_.boundaryField()[patchi].type()
    << " is the wrong k patchField type for wall-functions on patch "
    << curPatch.name() << nl
    << "    should be zeroGradient"
    << exit(FatalError);
}

if (!isA<zeroGradientFvPatchScalarField>(epsilon_.boundaryField()[patchi])) {
  FATAL_ERROR_IN("wall-function evaluation")
    << epsilon_.boundaryField()[patchi].type()
    << " is the wrong epsilon patchField type for wall-functions on patch "
    << curPatch.name() << nl
    << "    should be zeroGradient"
    << exit(FatalError);
}
