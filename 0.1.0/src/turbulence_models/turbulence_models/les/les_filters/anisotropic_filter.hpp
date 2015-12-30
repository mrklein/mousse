// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::anisotropicFilter
// Description
//   anisotropic filter
//   \verbatim
//   Kernel                 as filter          as Test filter with ratio 2
//   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   Box filter:            g = delta2/24  ->  g = delta2/6
//   Spherical box filter:  g = delta2/64  ->  g = delta2/16
//   Gaussian filter:       g = delta2/24  ->  g = delta2/6
//   \endverbatim
// SourceFiles
//   anisotropic_filter.cpp
#ifndef anisotropic_filter_hpp_
#define anisotropic_filter_hpp_
#include "les_filter.hpp"
namespace mousse
{
class anisotropicFilter
:
  public LESfilter
{
  // Private data
    scalar widthCoeff_;
    volVectorField coeff_;
  // Private Member Functions
    // Disallow default bitwise copy construct and assignment
    anisotropicFilter(const anisotropicFilter&);
    void operator=(const anisotropicFilter&);
public:
  //- Runtime type information
  TypeName("anisotropic");
  // Constructors
    //- Construct from components
    anisotropicFilter(const fvMesh& mesh, scalar widthCoeff);
    //- Construct from IOdictionary
    anisotropicFilter(const fvMesh& mesh, const dictionary&);
  //- Destructor
  virtual ~anisotropicFilter()
  {}
  // Member Functions
    //- Read the LESfilter dictionary
    virtual void read(const dictionary&);
  // Member Operators
    virtual tmp<volScalarField> operator()
    (
      const tmp<volScalarField>&
    ) const;
    virtual tmp<volVectorField> operator()
    (
      const tmp<volVectorField>&
    ) const;
    virtual tmp<volSymmTensorField> operator()
    (
      const tmp<volSymmTensorField>&
    ) const;
    virtual tmp<volTensorField> operator()
    (
      const tmp<volTensorField>&
    ) const;
};
}  // namespace mousse
#endif
