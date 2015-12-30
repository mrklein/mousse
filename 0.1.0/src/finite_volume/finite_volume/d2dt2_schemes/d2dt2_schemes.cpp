// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "d2dt2_scheme.hpp"
namespace mousse
{
namespace fv
{
// Define the constructor function hash tables
defineTemplateRunTimeSelectionTable(d2dt2Scheme<scalar>, Istream);
defineTemplateRunTimeSelectionTable(d2dt2Scheme<vector>, Istream);
defineTemplateRunTimeSelectionTable(d2dt2Scheme<sphericalTensor>, Istream);
defineTemplateRunTimeSelectionTable(d2dt2Scheme<symmTensor>, Istream);
defineTemplateRunTimeSelectionTable(d2dt2Scheme<tensor>, Istream);
}  // namespace fv
}  // namespace mousse
