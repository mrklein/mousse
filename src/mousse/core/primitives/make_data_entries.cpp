// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "compatibility_constant.hpp"
#include "constant.hpp"
#include "polynomial_entry.hpp"
#include "csv.hpp"
#include "data_entry.hpp"
#include "table.hpp"
#include "table_file.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "vector.hpp"
#include "spherical_tensor.hpp"
#include "symm_tensor.hpp"
#include "tensor.hpp"


namespace mousse {

MAKE_DATA_ENTRY(label);
MAKE_DATA_ENTRY_TYPE(CompatibilityConstant, label);
MAKE_DATA_ENTRY_TYPE(Constant, label);
//makeDataEntryType(PolynomialEntry, label);
MAKE_DATA_ENTRY_TYPE(CSV, label);
MAKE_DATA_ENTRY_TYPE(Table, label);
MAKE_DATA_ENTRY_TYPE(TableFile, label);
MAKE_DATA_ENTRY(scalar);
MAKE_DATA_ENTRY_TYPE(CompatibilityConstant, scalar);
MAKE_DATA_ENTRY_TYPE(Constant, scalar);
MAKE_DATA_ENTRY_TYPE(PolynomialEntry, scalar);
MAKE_DATA_ENTRY_TYPE(CSV, scalar);
MAKE_DATA_ENTRY_TYPE(Table, scalar);
MAKE_DATA_ENTRY_TYPE(TableFile, scalar);
MAKE_DATA_ENTRY(vector);
MAKE_DATA_ENTRY_TYPE(CompatibilityConstant, vector);
MAKE_DATA_ENTRY_TYPE(Constant, vector);
MAKE_DATA_ENTRY_TYPE(PolynomialEntry, vector);
MAKE_DATA_ENTRY_TYPE(CSV, vector);
MAKE_DATA_ENTRY_TYPE(Table, vector);
MAKE_DATA_ENTRY_TYPE(TableFile, vector);
MAKE_DATA_ENTRY(sphericalTensor);
MAKE_DATA_ENTRY_TYPE(CompatibilityConstant, sphericalTensor);
MAKE_DATA_ENTRY_TYPE(Constant, sphericalTensor);
MAKE_DATA_ENTRY_TYPE(PolynomialEntry, sphericalTensor);
MAKE_DATA_ENTRY_TYPE(CSV, sphericalTensor);
MAKE_DATA_ENTRY_TYPE(Table, sphericalTensor);
MAKE_DATA_ENTRY_TYPE(TableFile, sphericalTensor);
MAKE_DATA_ENTRY(symmTensor);
MAKE_DATA_ENTRY_TYPE(CompatibilityConstant, symmTensor);
MAKE_DATA_ENTRY_TYPE(Constant, symmTensor);
MAKE_DATA_ENTRY_TYPE(PolynomialEntry, symmTensor);
MAKE_DATA_ENTRY_TYPE(CSV, symmTensor);
MAKE_DATA_ENTRY_TYPE(Table, symmTensor);
MAKE_DATA_ENTRY_TYPE(TableFile, symmTensor);
MAKE_DATA_ENTRY(tensor);
MAKE_DATA_ENTRY_TYPE(CompatibilityConstant, tensor);
MAKE_DATA_ENTRY_TYPE(Constant, tensor);
MAKE_DATA_ENTRY_TYPE(PolynomialEntry, tensor);
MAKE_DATA_ENTRY_TYPE(CSV, tensor);
MAKE_DATA_ENTRY_TYPE(Table, tensor);
MAKE_DATA_ENTRY_TYPE(TableFile, tensor);

}
