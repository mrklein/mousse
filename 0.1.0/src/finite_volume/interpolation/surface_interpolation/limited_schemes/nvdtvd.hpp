#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_NVDTVD_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_NVDTVD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NVDTVD
// Description
//   mousse::NVDTVD
namespace mousse
{
class NVDTVD
{
public:
  typedef scalar phiType;
  typedef vector gradPhiType;
  // Null Constructor
    NVDTVD()
    {}
  // Member Functions
    scalar phict
    (
      const scalar faceFlux,
      const scalar phiP,
      const scalar phiN,
      const vector& gradcP,
      const vector& gradcN,
      const vector& d
    ) const
    {
      scalar gradf = phiN - phiP;
      scalar gradcf;
      if (faceFlux > 0)
      {
        gradcf = d & gradcP;
      }
      else
      {
        gradcf = d & gradcN;
      }
      if (mag(gradf) >= 1000*mag(gradcf))
      {
        return 1 - 0.5*1000*sign(gradcf)*sign(gradf);
      }
      else
      {
        return 1 - 0.5*gradf/gradcf;
      }
    }
    scalar r
    (
      const scalar faceFlux,
      const scalar phiP,
      const scalar phiN,
      const vector& gradcP,
      const vector& gradcN,
      const vector& d
    ) const
    {
      scalar gradf = phiN - phiP;
      scalar gradcf;
      if (faceFlux > 0)
      {
        gradcf = d & gradcP;
      }
      else
      {
        gradcf = d & gradcN;
      }
      if (mag(gradcf) >= 1000*mag(gradf))
      {
        return 2*1000*sign(gradcf)*sign(gradf) - 1;
      }
      else
      {
        return 2*(gradcf/gradf) - 1;
      }
    }
};
}  // namespace mousse
#endif
