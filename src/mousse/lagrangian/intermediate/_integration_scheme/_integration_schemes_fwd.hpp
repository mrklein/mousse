#ifndef LAGRANGIAN_INTERMEDIATE_TINTEGRATION_SCHEME_TINTEGRATION_SCHEMES_FWD_HPP_
#define LAGRANGIAN_INTERMEDIATE_TINTEGRATION_SCHEME_TINTEGRATION_SCHEMES_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse {

template<class Type> class IntegrationScheme;
typedef IntegrationScheme<scalar> scalarIntegrationScheme;
typedef IntegrationScheme<vector> vectorIntegrationScheme;

}  // namespace mousse

#endif

