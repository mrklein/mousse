// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_field.hpp"
#include "vol_mesh.hpp"
#include "fv_patch_field.hpp"
template<class Type>
void mousse::solutionControl::storePrevIter() const
{
  typedef GeometricField<Type, fvPatchField, volMesh> GeoField;
  HashTable<GeoField*>
    flds(mesh_.objectRegistry::lookupClass<GeoField>());
  FOR_ALL_ITER(typename HashTable<GeoField*>, flds, iter)
  {
    GeoField& fld = *iter();
    const word& fName = fld.name();
    size_t prevIterField = fName.find("PrevIter");
    if ((prevIterField == word::npos) && mesh_.relaxField(fName))
    {
      if (debug)
      {
        Info<< algorithmName_ << ": storing previous iter for "
          << fName << endl;
      }
      fld.storePrevIter();
    }
  }
}
