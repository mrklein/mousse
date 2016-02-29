#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_BLENDED_SCHEME_BASE_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_BLENDED_SCHEME_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::blendedSchemeBase
// Description
//   Base class for blended schemes to provide access to the blending factor
//   surface field


#include "class_name.hpp"
#include "tmp.hpp"
#include "surface_fields_fwd.hpp"
#include "vol_fields_fwd.hpp"

namespace mousse
{
TEMPLATE_NAME(blendedSchemeBase);
template<class Type>
class blendedSchemeBase
:
  public blendedSchemeBaseName
{
public:
  //- Constructor
  blendedSchemeBase()
  {}
  //- Destructor
  virtual ~blendedSchemeBase()
  {}
  // Memeber Functions
    //- Return the face-based blending factor
    virtual tmp<surfaceScalarField> blendingFactor
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const = 0;
};
}  // namespace mousse
#endif
