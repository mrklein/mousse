#ifndef CORE_FIELDS_GEOMETRIC_FIELDS_TRANSFORM_GEOMETRIC_FIELD_HPP_
#define CORE_FIELDS_GEOMETRIC_FIELDS_TRANSFORM_GEOMETRIC_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   transform_geometric_field.cpp
#include "transform.hpp"
#include "geometric_field.hpp"
namespace mousse
{
template<class Type, template<class> class PatchField, class GeoMesh>
void transform
(
  GeometricField<Type, PatchField, GeoMesh>&,
  const GeometricField<tensor, PatchField, GeoMesh>&,
  const GeometricField<Type, PatchField, GeoMesh>&
);
template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
  const GeometricField<tensor, PatchField, GeoMesh>&,
  const GeometricField<Type, PatchField, GeoMesh>&
);
template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
  const GeometricField<tensor, PatchField, GeoMesh>&,
  const tmp<GeometricField<Type, PatchField, GeoMesh> >&
);
template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
  const tmp<GeometricField<tensor, PatchField, GeoMesh> >&,
  const GeometricField<Type, PatchField, GeoMesh>&
);
template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
  const tmp<GeometricField<tensor, PatchField, GeoMesh> >&,
  const tmp<GeometricField<Type, PatchField, GeoMesh> >&
);
template<class Type, template<class> class PatchField, class GeoMesh>
void transform
(
  GeometricField<Type, PatchField, GeoMesh>&,
  const dimensionedTensor&,
  const GeometricField<Type, PatchField, GeoMesh>&
);
template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
  const dimensionedTensor&,
  const GeometricField<Type, PatchField, GeoMesh>&
);
template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<Type, PatchField, GeoMesh> > transform
(
  const dimensionedTensor&,
  const tmp<GeometricField<Type, PatchField, GeoMesh> >&
);
}  // namespace mousse
#ifdef NoRepository
#   include "transform_geometric_field.cpp"
#endif
#endif
