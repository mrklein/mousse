// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvs_patch_fields.hpp"
namespace mousse
{
// Static Data Members
#define makeFvsPatchField(fvsPatchTypeField)                                  \
                                       \
defineNamedTemplateTypeNameAndDebug(fvsPatchTypeField, 0);                    \
template<>                                                                    \
int fvsPatchTypeField::disallowGenericFvsPatchField                           \
(                                                                             \
  debug::debugSwitch("disallowGenericFvsPatchField", 0)                     \
);                                                                            \
defineTemplateRunTimeSelectionTable(fvsPatchTypeField, patch);                \
defineTemplateRunTimeSelectionTable(fvsPatchTypeField, patchMapper);          \
defineTemplateRunTimeSelectionTable(fvsPatchTypeField, dictionary);
makeFvsPatchField(fvsPatchScalarField)
makeFvsPatchField(fvsPatchVectorField)
makeFvsPatchField(fvsPatchSphericalTensorField)
makeFvsPatchField(fvsPatchSymmTensorField)
makeFvsPatchField(fvsPatchTensorField)
}  // namespace mousse
