// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "writers.hpp"
namespace mousse
{
// Static Data Members
#define defineSetWriterType(dataType)                                         \
  defineNamedTemplateTypeNameAndDebug(writer<dataType >, 0);                \
  defineTemplatedRunTimeSelectionTable(writer, word, dataType);
defineSetWriterType(scalar);
defineSetWriterType(vector);
defineSetWriterType(sphericalTensor);
defineSetWriterType(symmTensor);
defineSetWriterType(tensor);
}  // namespace mousse
