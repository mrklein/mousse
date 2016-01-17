// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   preserve_patch_types.cpp
#ifndef preserve_patch_types_hpp_
#define preserve_patch_types_hpp_
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
