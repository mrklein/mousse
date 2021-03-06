// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "grading_descriptors.hpp"


// Constructors 
mousse::gradingDescriptors::gradingDescriptors()
:
  List<gradingDescriptor>{1, gradingDescriptor()}
{}


mousse::gradingDescriptors::gradingDescriptors(const gradingDescriptor& gd)
:
  List<gradingDescriptor>{1, gd}
{}


// Member Functions 
mousse::gradingDescriptors mousse::gradingDescriptors::inv() const
{
  gradingDescriptors ret{*this};
  FOR_ALL(ret, i) {
    ret[i] = operator[](ret.size() - i - 1).inv();
  }
  return ret;
}


// Friend Operators 
mousse::Istream& mousse::operator>>(Istream& is, gradingDescriptors& gds)
{
  // Examine next token
  token t{is};
  if (t.isNumber()) {
    gds = gradingDescriptors(gradingDescriptor(t.number()));
  } else {
    is.putBack(t);
    // Read the list for gradingDescriptors
    is >> static_cast<List<gradingDescriptor>& >(gds);
    // Check state of Istream
    is.check("operator>>(Istream&, gradingDescriptor&)");
    // Normalize the blockFractions and nDivFractions
    // of the list of gradingDescriptors
    scalar sumBlockFraction = 0;
    scalar sumNDivFraction = 0;
    FOR_ALL(gds, i) {
      sumBlockFraction += gds[i].blockFraction_;
      sumNDivFraction += gds[i].nDivFraction_;
    }
    FOR_ALL(gds, i) {
      gds[i].blockFraction_ /= sumBlockFraction;
      gds[i].nDivFraction_ /= sumNDivFraction;
    }
  }
  return is;
}

