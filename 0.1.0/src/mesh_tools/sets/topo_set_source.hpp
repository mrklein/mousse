#ifndef MESH_TOOLS_SETS_TOPO_SET_SOURCE_TOPO_SET_SOURCE_HPP_
#define MESH_TOOLS_SETS_TOPO_SET_SOURCE_TOPO_SET_SOURCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::topoSetSource
// Description
//   Base class of a source for a topoSet.
//   Implementer has to modify the given set (see applyToSet) according to
//   its function and the setAction (one of add/delete/new)

#include "point_field.hpp"
#include "word.hpp"
#include "label_list.hpp"
#include "face_list.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "named_enum.hpp"
#include "hash_table.hpp"
#include "dictionary.hpp"


namespace mousse {

// Forward declaration of classes
class polyMesh;
class topoSet;


class topoSetSource
{
public:
  // Public data types
    //- Enumeration defining the types of sources
    enum sourceType
    {
      CELLSETSOURCE,
      FACESETSOURCE,
      POINTSETSOURCE,
      CELLZONESOURCE,
      FACEZONESOURCE,
      POINTZONESOURCE
    };
    //- Enumeration defining the valid actions
    enum setAction
    {
      CLEAR,
      NEW,
      INVERT,
      ADD,
      DELETE,
      SUBSET,
      LIST,
      REMOVE
    };
protected:
    //- A table of usage strings
    static HashTable<string>* usageTablePtr_;
    //- Class with constructor to add usage string to table
    class addToUsageTable
    {
    public:
      addToUsageTable(const word& name, const string& msg)
      {
        if (!usageTablePtr_) {
          usageTablePtr_ = new HashTable<string>();
        }
        usageTablePtr_->insert(name, msg);
      }
      ~addToUsageTable()
      {
        if (usageTablePtr_) {
          delete usageTablePtr_;
          usageTablePtr_ = NULL;
        }
      }
    };
  // Protected data
    const polyMesh& mesh_;
    //- Add (if bool) cellI to set or delete cellI from set.
    void addOrDelete(topoSet& set, const label cellI, const bool) const;
private:
    static const NamedEnum<setAction, 8> actionNames_;
    static const string illegalSource_;
public:
  //- Runtime type information
  TYPE_NAME("topoSetSource");
  // Static Functions
    //- Convert string to action
    static setAction toAction(const word& actionName)
    {
      return actionNames_[actionName];
    }
    //- Check state of stream.
    static Istream& checkIs(Istream& is);
  // Declare run-time constructor selection table
    // For the dictionary constructor
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      topoSetSource,
      word,
      (
        const polyMesh& mesh,
        const dictionary& dict
      ),
      (mesh, dict)
    );
    // For the Istream constructor
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      topoSetSource,
      istream,
      (
        const polyMesh& mesh,
        Istream& is
      ),
      (mesh, is)
    );
    //- Class used for the read-construction of
    //  PtrLists of topoSetSource
    class iNew
    {
      const polyMesh& mesh_;
    public:
      iNew(const polyMesh& mesh)
      :
        mesh_{mesh}
      {}
      autoPtr<topoSetSource> operator()(Istream& is) const
      {
        word topoSetSourceType{is};
        dictionary dict{is};
        return topoSetSource::New(topoSetSourceType, mesh_, dict);
      }
    };
    static const string& usage(const word& name)
    {
      if (!usageTablePtr_) {
        usageTablePtr_ = new HashTable<string>{};
      }
      const HashTable<string>& usageTable = *usageTablePtr_;
      if (usageTable.found(name)) {
        return usageTable[name];
      } else {
        return illegalSource_;
      }
    }
  // Constructors
    //- Construct from components
    topoSetSource(const polyMesh& mesh);
    //- Clone
    autoPtr<topoSetSource> clone() const
    {
      NOT_IMPLEMENTED("autoPtr<topoSetSource> clone() const");
      return autoPtr<topoSetSource>{NULL};
    }
    //- Disallow default bitwise copy construct
    topoSetSource(const topoSetSource&) = delete;
    //- Disallow default bitwise assignment
    topoSetSource& operator=(const topoSetSource&) = delete;
  // Selectors
    //- Return a reference to the selected topoSetSource
    static autoPtr<topoSetSource> New
    (
      const word& topoSetSourceType,
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Return a reference to the selected topoSetSource
    static autoPtr<topoSetSource> New
    (
      const word& topoSetSourceType,
      const polyMesh& mesh,
      Istream& is
    );
  //- Destructor
  virtual ~topoSetSource();
  // Member Functions
    const polyMesh& mesh() const
    {
      return mesh_;
    }
  // Member Functions
    virtual sourceType setType() const = 0;
    virtual void applyToSet(const setAction action, topoSet&) const = 0;
};
}  // namespace mousse
#endif
