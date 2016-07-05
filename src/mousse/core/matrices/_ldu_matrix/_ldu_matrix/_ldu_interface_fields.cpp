// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_ldu_interface_field.hpp"
#include "field_types.hpp"

// Static Data Members
namespace mousse {

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(LduInterfaceField<scalar>, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(LduInterfaceField<vector>, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  LduInterfaceField<sphericalTensor>,
  0
);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(LduInterfaceField<symmTensor>, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(LduInterfaceField<tensor>, 0);

}
