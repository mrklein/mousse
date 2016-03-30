// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tet_indices.hpp"


// Constructors 
mousse::tetIndices::tetIndices()
:
  cellI_{-1},
  faceI_{-1},
  faceBasePtI_{-1},
  facePtAI_{-1},
  facePtBI_{-1},
  tetPtI_{-1}
{}


mousse::tetIndices::tetIndices
(
  label cellI,
  label faceI,
  label faceBasePtI,
  label facePtAI,
  label facePtBI,
  label tetPtI
)
:
  cellI_{cellI},
  faceI_{faceI},
  faceBasePtI_{faceBasePtI},
  facePtAI_{facePtAI},
  facePtBI_{facePtBI},
  tetPtI_{tetPtI}
{}


mousse::tetIndices::tetIndices
(
  label cellI,
  label faceI,
  label tetPtI,
  const polyMesh& mesh
)
:
  cellI_{cellI},
  faceI_{faceI},
  faceBasePtI_{-1},
  facePtAI_{-1},
  facePtBI_{-1},
  tetPtI_{tetPtI}
{
  const faceList& pFaces = mesh.faces();
  const labelList& pOwner = mesh.faceOwner();
  const mousse::face& f = pFaces[faceI_];
  bool own = (pOwner[faceI_] == cellI_);
  faceBasePtI_ = mesh.tetBasePtIs()[faceI_];
  label facePtI = (tetPtI_ + faceBasePtI_) % f.size();
  label otherFacePtI = f.fcIndex(facePtI);
  if (own) {
    facePtAI_ = facePtI;
    facePtBI_ = otherFacePtI;
  } else {
    facePtAI_ = otherFacePtI;
    facePtBI_ = facePtI;
  }
}


// Destructor 
mousse::tetIndices::~tetIndices()
{}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, tetIndices& tI)
{
  is >> tI.cell()
    >> tI.face()
    >> tI.faceBasePt()
    >> tI.facePtA()
    >> tI.facePtB()
    >> tI.tetPt();
  // Check state of Istream
  is.check
  (
    "mousse::Istream& mousse::operator>>(mousse::Istream&, mousse::tetIndices&)"
  );
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const tetIndices& tI)
{
  os << tI.cell() << token::SPACE
    << tI.face() << token::SPACE
    << tI.faceBasePt() << token::SPACE
    << tI.facePtA() << token::SPACE
    << tI.facePtB() << token::SPACE
    << tI.tetPt() << token::SPACE
    << endl;
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<(mousse::Ostream&, "
    "const mousse::tetIndices&)"
  );
  return os;
}
