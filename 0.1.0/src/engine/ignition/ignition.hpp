// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ignition
// Description
//   mousse::ignition
// SourceFiles
//   ignition.cpp
//   ignition_io.cpp
#ifndef ignition_hpp_
#define ignition_hpp_
#include "ignition_site.hpp"
#include "switch.hpp"
namespace mousse
{
class fvMesh;
class ignition
{
  // Private data
    const fvMesh& mesh_;
    Switch ignite_;
    PtrList<ignitionSite> ignSites_;
public:
  // Constructors
    //- Construct from Istream and database
    ignition(const dictionary&, const Time&, const fvMesh&);
    //- Construct from Istream and engineTime
    ignition(const dictionary&, const engineTime&, const fvMesh&);
    //- Disallow default bitwise copy construct
    ignition(const ignition&) = delete;
    //- Disallow default bitwise assignment
    ignition& operator=(const ignition&) = delete;
  // Member Functions
    //- Return the set of ignition sites
    const PtrList<ignitionSite>& sites()
    {
      return ignSites_;
    }
    // Check
      bool ignite() const
      {
        return ignite_;
      }
      //- Are any of the ignition site currently igniting
      bool igniting() const;
      //- Has the mixture been ignited?
      bool ignited() const;
};
}  // namespace mousse
#endif
