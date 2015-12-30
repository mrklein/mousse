// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyModifyFace
// Description
//   Class describing modification of a face.
#ifndef poly_modify_face_hpp_
#define poly_modify_face_hpp_
#include "label.hpp"
#include "face.hpp"
#include "topo_action.hpp"
namespace mousse
{
class polyModifyFace
:
  public topoAction
{
  // Private data
    //- Face
    face face_;
    //- Master face ID
    label faceID_;
    //- Face owner
    label owner_;
    //- Face neighbour
    label neighbour_;
    //- Does the face flux need to be flipped
    bool flipFaceFlux_;
    //- Boundary patch ID
    label patchID_;
    //- Remove from current zone
    bool removeFromZone_;
    //- Face zone ID
    label zoneID_;
    //- Face zone flip
    bool zoneFlip_;
public:
  // Static data members
    //- Runtime type information
    TypeName("modifyFace");
  // Constructors
    //- Construct null.  Used in constructing lists
    polyModifyFace()
    :
      face_(0),
      faceID_(-1),
      owner_(-1),
      neighbour_(-1),
      flipFaceFlux_(false),
      patchID_(-1),
      removeFromZone_(false),
      zoneID_(-1),
      zoneFlip_(false)
    {}
    //- Construct from components
    polyModifyFace
    (
      const face& f,
      const label faceID,
      const label owner,
      const label neighbour,
      const bool flipFaceFlux,
      const label patchID,
      const bool removeFromZone,
      const label zoneID,
      const bool zoneFlip
    )
    :
      face_(f),
      faceID_(faceID),
      owner_(owner),
      neighbour_(neighbour),
      flipFaceFlux_(flipFaceFlux),
      patchID_(patchID),
      removeFromZone_(removeFromZone),
      zoneID_(zoneID),
      zoneFlip_(zoneFlip)
    {
      if (face_.size() < 3)
      {
        FatalErrorIn
        (
          "polyModifyFace::polyModifyFace\n"
          "(\n"
          "    const face& f,\n"
          "    const label faceID,\n"
          "    const label owner,\n"
          "    const label neighbour,\n"
          "    const bool flipFaceFlux,\n"
          "    const label patchID,\n"
          "    const bool removeFromZone,\n"
          "    const label zoneID,\n"
          "    const bool zoneFlip\n"
          ")"
        )   << "Invalid face: less than 3 points. This is not allowed\n"
          << "Face: " << face_
          << " faceID:" << faceID_
          << " owner:" << owner_
          << " neighbour:" << neighbour_
          << abort(FatalError);
      }
      if (min(face_) < 0)
      {
        FatalErrorIn
        (
          "polyModifyFace::polyModifyFace\n"
          "(\n"
          "    const face& f,\n"
          "    const label faceID,\n"
          "    const label owner,\n"
          "    const label neighbour,\n"
          "    const bool flipFaceFlux,\n"
          "    const label patchID,\n"
          "    const bool removeFromZone,\n"
          "    const label zoneID,\n"
          "    const bool zoneFlip\n"
          ")"
        )   << "Face contains invalid vertex ID: " << face_ << ".  "
          << "This is not allowed.\n"
          << " faceID:" << faceID_
          << " owner:" << owner_
          << " neighbour:" << neighbour_
          << abort(FatalError);
      }
      if (min(owner_, neighbour_) >= 0 && owner_ == neighbour_)
      {
        FatalErrorIn
        (
          "polyModifyFace::polyModifyFace\n"
          "(\n"
          "    const face& f,\n"
          "    const label faceID,\n"
          "    const label owner,\n"
          "    const label neighbour,\n"
          "    const bool flipFaceFlux,\n"
          "    const label patchID,\n"
          "    const bool removeFromZone,\n"
          "    const label zoneID,\n"
          "    const bool zoneFlip\n"
          ")"
        )   << "Face owner and neighbour are identical.  "
          << "This is not allowed.\n"
          << "Face: " << face_
          << " faceID:" << faceID_
          << " owner:" << owner_
          << " neighbour:" << neighbour_
          << abort(FatalError);
      }
      if (neighbour_ >= 0 && patchID_ >= 0)
      {
        FatalErrorIn
        (
          "polyModifyFace::polyModifyFace\n"
          "(\n"
          "    const face& f,\n"
          "    const label faceID,\n"
          "    const label owner,\n"
          "    const label neighbour,\n"
          "    const bool flipFaceFlux,\n"
          "    const label patchID,\n"
          "    const bool removeFromZone,\n"
          "    const label zoneID,\n"
          "    const bool zoneFlip\n"
          ")"
        )   << "Patch face has got a neighbour  "
          << "This is not allowed.\n"
          << "Face: " << face_
          << " faceID:" << faceID_
          << " owner:" << owner_
          << " neighbour:" << neighbour_
          << " patchID:" << patchID_
          << abort(FatalError);
      }
      if (zoneID_ < 0 && zoneFlip )
      {
        FatalErrorIn
        (
          "polyModifyFace::polyModifyFace\n"
          "(\n"
          "    const face& f,\n"
          "    const label faceID,\n"
          "    const label owner,\n"
          "    const label neighbour,\n"
          "    const bool flipFaceFlux,\n"
          "    const label patchID,\n"
          "    const bool removeFromZone,\n"
          "    const label zoneID,\n"
          "    const bool zoneFlip\n"
          ")"
        )   << "Specified zone flip for a face that does not  "
          << "belong to zone.  This is not allowed.\n"
          << "Face: " << face_
          << " faceID:" << faceID_
          << " owner:" << owner_
          << " neighbour:" << neighbour_
          << abort(FatalError);
      }
    }
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const
    {
      return autoPtr<topoAction>(new polyModifyFace(*this));
    }
  // Default Destructor
  // Member Functions
    //- Return face
    const face& newFace() const
    {
      return face_;
    }
    //- Return master face ID
    label faceID() const
    {
      return faceID_;
    }
    //- Return owner cell ID
    label owner() const
    {
      return owner_;
    }
    //- Return owner cell ID
    label neighbour() const
    {
      return neighbour_;
    }
    //- Does the face flux need to be flipped
    bool flipFaceFlux() const
    {
      return flipFaceFlux_;
    }
    //- Does the face belong to a boundary patch?
    bool isInPatch() const
    {
      return patchID_ >= 0;
    }
    //- Boundary patch ID
    label patchID() const
    {
      return patchID_;
    }
    //- Does the face belong to a zone?
    bool isInZone() const
    {
      return zoneID_ >= 0;
    }
    //- Is the face only a zone face (i.e. not belonging to a cell)
    bool onlyInZone() const
    {
      return zoneID_ >= 0 && owner_ < 0 && neighbour_ < 0;
    }
    bool removeFromZone() const
    {
      return removeFromZone_;
    }
    //- Face zone ID
    label zoneID() const
    {
      return zoneID_;
    }
    //- Face zone flip
    label zoneFlip() const
    {
      return zoneFlip_;
    }
};
}  // namespace mousse
#endif
