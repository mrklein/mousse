// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   integration_scheme.cpp
#ifndef _integration_schemes_fwd_hpp_
#define _integration_schemes_fwd_hpp_
namespace mousse
{
template<class Type>
class IntegrationScheme;
typedef IntegrationScheme<scalar> scalarIntegrationScheme;
typedef IntegrationScheme<vector> vectorIntegrationScheme;
}  // namespace mousse
#endif
