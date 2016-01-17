// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "referred_wall_face.hpp"
// Constructors 
mousse::referredWallFace::referredWallFace()
:
  face{},
  pts_{},
  patchI_{}
{}
mousse::referredWallFace::referredWallFace
(
  const face& f,
  const pointField& pts,
  label patchI
)
:
  face{f},
  pts_{pts},
  patchI_{patchI}
{
  if (this->size() != pts_.size())
  {
    FATAL_ERROR_IN
    (
      "mousse::referredWallFace::referredWallFace"
      "("
        "const face& f, "
        "const pointField& pts, "
        "label patchI"
      ")"
    )
    << "Face and pointField are not the same size. " << nl << (*this)
    << abort(FatalError);
  }
}
mousse::referredWallFace::referredWallFace(const referredWallFace& rWF)
:
  face{rWF},
  pts_{rWF.pts_},
  patchI_{rWF.patchI_}
{
  if (this->size() != pts_.size())
  {
    FATAL_ERROR_IN
    (
      "mousse::referredWallFace::referredWallFace"
      "("
        "const referredWallFace& rWF"
      ")"
    )
    << "Face and pointField are not the same size. " << nl << (*this)
    << abort(FatalError);
  }
}
// Destructor 
mousse::referredWallFace::~referredWallFace()
{}
// Member Operators 
bool mousse::referredWallFace::operator==(const referredWallFace& rhs) const
{
  return (static_cast<const face&>(rhs) == static_cast<face>(*this)
          && rhs.pts_ == pts_
          && rhs.patchI_ == patchI_);
}
bool mousse::referredWallFace::operator!=(const referredWallFace& rhs) const
{
  return !(*this == rhs);
}
// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, referredWallFace& rWF)
{
  is  >> static_cast<face&>(rWF) >> rWF.pts_ >> rWF.patchI_;
  // Check state of Istream
  is.check
  (
    "mousse::Istream& "
    "mousse::operator>>(mousse::Istream&, mousse::referredWallFace&)"
  );
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const referredWallFace& rWF)
{
  os << static_cast<const face&>(rWF) << token::SPACE
    << rWF.pts_ << token::SPACE
    << rWF.patchI_;
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<(mousse::Ostream&, "
    "const mousse::referredWallFace&)"
  );
  return os;
}
