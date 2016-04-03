#ifndef MESH_TOOLS_SETS_FACE_SOURCES_POINT_TO_FACE_POINT_TO_FACE_HPP_
#define MESH_TOOLS_SETS_FACE_SOURCES_POINT_TO_FACE_POINT_TO_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointToFace
// Description
//   A topoSetSource to select faces based on use of points.

#include "topo_set_source.hpp"
#include "named_enum.hpp"


namespace mousse {

class pointToFace
:
  public topoSetSource
{
public:
    //- Enumeration defining the valid options
    enum pointAction
    {
      ANY,
      ALL,
      EDGE
    };
private:
    //- Add usage string
    static addToUsageTable usage_;
    static const NamedEnum<pointAction, 3> pointActionNames_;
    //- Name of set to use
    word setName_;
    //- Option
    pointAction option_;
  // Private Member Functions
    //- Depending on face to cell option add to or delete from cellSet.
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("pointToFace");
  // Constructors
    //- Construct from components
    pointToFace
    (
      const polyMesh& mesh,
      const word& setName,
      const pointAction option
    );
    //- Construct from dictionary
    pointToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    pointToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~pointToFace();
  // Member Functions
    virtual sourceType setType() const
    {
      return FACESETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
