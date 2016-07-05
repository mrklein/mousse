#ifndef CONVERSION_ENSIGHT_PART_ENSIGHT_PART_FACES_HPP_
#define CONVERSION_ENSIGHT_PART_ENSIGHT_PART_FACES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightPartFaces
// Description
//   An implementation of ensightPart to hold volume mesh faces.

#include "ensight_part.hpp"


namespace mousse {

class ensightPartFaces
:
  public ensightPart
{
  // Private Member Functions
    //- Track points used
    virtual localPoints calcLocalPoints() const;
    //- Element connectivity
    virtual void writeConnectivity
    (
      ensightGeoFile&,
      const word& key,
      const labelUList& idList,
      const labelUList& pointMap
    ) const;
protected:
    //- Addressable ensight element types
    enum elemType
    {
      tria3Elements,
      quad4Elements,
      nsidedElements
    };
  // Static data members
    static const List<word> elemTypes_;
  // Protected data
    //- Faces referenced
    const faceList& faces_;
    //- Can skip local point renumbering when points are contiguous
    const bool contiguousPoints_;
  // Protected Member Functions
    //- Classify the face shapes, set elemLists.
    void classify(const faceList&);
    //- Helper: write connectivity
    void writeConnectivity
    (
      ensightGeoFile&,
      const word& key,
      const faceList&,
      const labelUList& idList,
      const labelUList& pointMap
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("ensightFaces");
  // Constructors
    //- Construct empty part with number and description
    ensightPartFaces(label partNumber, const string& partDescription);
    //- Construct part with number, description, points and faces
    //  Can skip local point renumbering when points are contiguous
    ensightPartFaces
    (
      label partNumber,
      const string& partDescription,
      const pointField&,
      const faceList&,
      const bool contiguousPoints = false
    );
    //- Construct from polyMesh and polyPatch
    ensightPartFaces
    (
      label partNumber,
      const polyMesh&,
      const polyPatch&
    );
    //- Construct as copy
    ensightPartFaces(const ensightPartFaces&);
    //- Reconstruct part characteristics (eg, element types) from Istream
    //  A part reconstructed in this manner can be used when writing fields,
    //  but cannot be used to write a new geometry
    //  \sa mousse::ensightPart::reconstruct
    ensightPartFaces(Istream&);
    //- Reconstruct part characteristics on freestore from Istream
    static autoPtr<ensightPartFaces> New(Istream& is)
    {
      return autoPtr<ensightPartFaces>{new ensightPartFaces{is}};
    }
    //- Disallow default bitwise assignment
    ensightPartFaces& operator=(const ensightPartFaces&) = delete;
  //- Destructor
  virtual ~ensightPartFaces();
  // Member Functions
    //- Write geometry
    virtual void writeGeometry(ensightGeoFile&) const;
    //- Static listing of the element types
    virtual const List<word>& elementTypes() const
    {
      return elemTypes_;
    }
};
}  // namespace mousse
#endif
