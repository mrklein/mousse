#ifndef FINITE_VOLUME_FINITE_VOLUME_FV_SCHEMES_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FV_SCHEMES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvSchemes
// Description
//   Selector class for finite volume differencing schemes.
//   fvMesh is derived from fvShemes so that all fields have access to the
//   fvSchemes from the mesh reference they hold.
// SourceFiles
//   fv_schemes.cpp
#include "iodictionary.hpp"
namespace mousse
{
class fvSchemes
:
  public IOdictionary
{
  // Private data
    dictionary ddtSchemes_;
    ITstream defaultDdtScheme_;
    dictionary d2dt2Schemes_;
    ITstream defaultD2dt2Scheme_;
    dictionary interpolationSchemes_;
    ITstream defaultInterpolationScheme_;
    dictionary divSchemes_;
    ITstream defaultDivScheme_;
    dictionary gradSchemes_;
    ITstream defaultGradScheme_;
    dictionary snGradSchemes_;
    ITstream defaultSnGradScheme_;
    dictionary laplacianSchemes_;
    ITstream defaultLaplacianScheme_;
    mutable dictionary fluxRequired_;
    bool defaultFluxRequired_;
    //- Steady-state run indicator
    //  Set true if the default ddtScheme is steadyState
    bool steady_;
  // Private Member Functions
    //- Clear the dictionaries and streams before reading
    void clear();
    //- Read settings from the dictionary
    void read(const dictionary&);
public:
  //- Debug switch
  static int debug;
  // Constructors
    //- Construct for objectRegistry
    fvSchemes(const objectRegistry& obr);
    //- Disallow default bitwise copy construct
    fvSchemes(const fvSchemes&) = delete;
    //- Disallow default bitwise assignment
    fvSchemes& operator=(const fvSchemes&) = delete;
  // Member Functions
    // Access
      const dictionary& schemesDict() const;
      ITstream& ddtScheme(const word& name) const;
      ITstream& d2dt2Scheme(const word& name) const;
      ITstream& interpolationScheme(const word& name) const;
      ITstream& divScheme(const word& name) const;
      ITstream& gradScheme(const word& name) const;
      ITstream& snGradScheme(const word& name) const;
      ITstream& laplacianScheme(const word& name) const;
      void setFluxRequired(const word& name) const;
      bool fluxRequired(const word& name) const;
      //- Return true if the default ddtScheme is steadyState
      bool steady() const
      {
        return steady_;
      }
      //- Return true if the default ddtScheme is not steadyState
      bool transient() const
      {
        return !steady_;
      }
    // Read
      //- Read the fvSchemes
      bool read();
};
}  // namespace mousse
#endif
