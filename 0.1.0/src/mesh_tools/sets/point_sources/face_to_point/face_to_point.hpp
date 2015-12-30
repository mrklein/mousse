// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceToPoint
// Description
//   A topoSetSource to select points based on usage in faces.
// SourceFiles
//   face_to_point.cpp
#ifndef face_to_point_hpp_
#define face_to_point_hpp_
#include "topo_set_source.hpp"
#include "named_enum.hpp"
namespace mousse
{
class faceToPoint
:
  public topoSetSource
{
public:
    //- Enumeration defining the valid options
    enum faceAction
    {
      ALL
    };
private:
    //- Add usage string
    static addToUsageTable usage_;
    static const NamedEnum<faceAction, 1> faceActionNames_;
    //- Name of set to use
    word setName_;
    //- Option
    faceAction option_;
  // Private Member Functions
    //- Depending on face to cell option add to or delete from cellSet.
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TypeName("faceToPoint");
  // Constructors
    //- Construct from components
    faceToPoint
    (
      const polyMesh& mesh,
      const word& setName,
      const faceAction option
    );
    //- Construct from dictionary
    faceToPoint
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    faceToPoint
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~faceToPoint();
  // Member Functions
    virtual sourceType setType() const
    {
      return POINTSETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
