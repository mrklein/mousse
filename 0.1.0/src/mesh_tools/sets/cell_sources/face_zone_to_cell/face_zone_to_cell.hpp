// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceZoneToCell
// Description
//   A topoSetSource to select cells based on side of faceZone.
// SourceFiles
//   face_zone_to_cell.cpp
#ifndef face_zone_to_cell_hpp_
#define face_zone_to_cell_hpp_
#include "topo_set_source.hpp"
#include "word_re.hpp"
namespace mousse
{
class faceZoneToCell
:
  public topoSetSource
{
public:
    //- Enumeration defining the valid options
    enum faceAction
    {
      MASTER,
      SLAVE
    };
private:
  // Private data
    static const NamedEnum<faceAction, 2> faceActionNames_;
    //- Add usage string
    static addToUsageTable usage_;
    //- Name/regular expression of faceZone
    wordRe zoneName_;
    //- Option
    faceAction option_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("faceZoneToCell");
  // Constructors
    //- Construct from components
    faceZoneToCell
    (
      const polyMesh& mesh,
      const word& zoneName,
      const faceAction option
    );
    //- Construct from dictionary
    faceZoneToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    faceZoneToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~faceZoneToCell();
  // Member Functions
    virtual sourceType setType() const
    {
      return CELLSETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
