// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ignitionSite
// Description
//   mousse::ignitionSite
// SourceFiles
//   ignition_site_i.hpp
//   ignition_site.cpp
//   ignition_site_io.cpp
#ifndef ignition_site_hpp_
#define ignition_site_hpp_
#include "vector.hpp"
#include "label_list.hpp"
#include "scalar_list.hpp"
#include "auto_ptr.hpp"
#include "dictionary.hpp"
namespace mousse
{
class Time;
class engineTime;
class fvMesh;
class ignitionSite
{
  // Private data
    const Time& db_;
    const fvMesh& mesh_;
    dictionary ignitionSiteDict_;
    vector location_;
    scalar diameter_;
    scalar time_;
    scalar duration_;
    scalar strength_;
    labelList cells_;
    scalarList cellVolumes_;
    //- Current time index.
    //  Used during the update for moving meshes
    mutable label timeIndex_;
  // Private Member Functions
    void findIgnitionCells(const fvMesh&);
public:
  // Public classes
    //- Class used for the read-construction of
    //  PtrLists of ignitionSite
    class iNew
    {
      const Time& db_;
      const fvMesh& mesh_;
    public:
      iNew(const Time& db, const fvMesh& mesh)
      :
        db_(db),
        mesh_(mesh)
      {}
      autoPtr<ignitionSite> operator()(Istream& is) const
      {
        return autoPtr<ignitionSite>(new ignitionSite(is, db_, mesh_));
      }
    };
  // Constructors
    //- Construct from Istream and database
    ignitionSite(Istream&, const Time&, const fvMesh&);
    //- Construct from Istream and engineTime
    ignitionSite(Istream&, const engineTime&, const fvMesh&);
    //- Clone
    autoPtr<ignitionSite> clone() const
    {
      return autoPtr<ignitionSite>(new ignitionSite(*this));
    }
  // Member Functions
    // Access
      const vector& location() const
      {
        return location_;
      }
      scalar diameter() const
      {
        return diameter_;
      }
      scalar time() const
      {
        return time_;
      }
      scalar duration() const
      {
        return duration_;
      }
      scalar strength() const
      {
        return strength_;
      }
      //- Return the ignition cells updated if the mesh moved
      const labelList& cells() const;
      const scalarList& cellVolumes() const
      {
        return cellVolumes_;
      }
    // Check
      bool igniting() const;
      bool ignited() const;
  // Member Operators
    void operator=(const ignitionSite&);
};
}  // namespace mousse
#endif
