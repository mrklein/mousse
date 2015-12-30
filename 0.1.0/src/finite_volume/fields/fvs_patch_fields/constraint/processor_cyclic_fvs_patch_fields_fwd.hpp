// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef processor_cyclic_fvs_patch_fields_fwd_hpp_
#define processor_cyclic_fvs_patch_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type> class processorCyclicFvsPatchField;
makeFvsPatchTypeFieldTypedefs(processor);
}  // namespace mousse
#endif
