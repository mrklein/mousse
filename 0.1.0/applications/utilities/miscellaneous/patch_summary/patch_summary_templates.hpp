#ifndef UTILITIES_MISCELLANEOUS_PATCH_SUMMARY_PATCH_SUMMARY_TEMPLATES_HPP_
#define UTILITIES_MISCELLANEOUS_PATCH_SUMMARY_PATCH_SUMMARY_TEMPLATES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "vol_fields.hpp"
namespace mousse
{
  template<class GeoField>
  void addToFieldList
  (
    PtrList<GeoField>& fieldList,
    const IOobject& obj,
    const label fieldI,
    const typename GeoField::Mesh& mesh
  );
  template<class GeoField>
  void outputFieldList
  (
    const PtrList<GeoField>& fieldList,
    const label patchI
  );
  template<class GeoField>
  void collectFieldList
  (
    const PtrList<GeoField>& fieldList,
    const label patchI,
    HashTable<word>& fieldToType
  );
}  // namespace mousse
#ifdef NoRepository
  #include "patch_summary_templates.cpp"
#endif
#endif
