#ifndef CORE_MESHES_PRESERVE_PATCH_TYPES_HPP_
#define CORE_MESHES_PRESERVE_PATCH_TYPES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   preserve_patch_types.cpp
#include "file_name.hpp"
#include "word_list.hpp"
#include "dictionary.hpp"
namespace mousse
{
class objectRegistry;
//- Preserve patch types
void preservePatchTypes
(
  const objectRegistry& obr,
  const word& meshInstance,
  const fileName& meshDir,
  const wordList& patchNames,
  PtrList<dictionary>& patchDicts,
  const word& defaultFacesName,
  word& defaultFacesType
);
}  // namespace mousse
#endif
