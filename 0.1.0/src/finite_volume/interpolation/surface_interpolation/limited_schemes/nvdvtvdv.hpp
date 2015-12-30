// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NVDVTVDV
// Description
//   mousse::NVDVTVDV
#ifndef nvdvtvdv_hpp_
#define nvdvtvdv_hpp_
namespace mousse
{
class NVDVTVDV
{
public:
  typedef vector phiType;
  typedef tensor gradPhiType;
  // Null Constructor
    NVDVTVDV()
    {}
  // Member Functions
    scalar phict
    (
      const scalar faceFlux,
      const vector& phiP,
      const vector& phiN,
      const tensor& gradcP,
      const tensor& gradcN,
      const vector& d
    ) const
    {
      vector gradfV = phiN - phiP;
      scalar gradf = gradfV & gradfV;
      scalar gradcf;
      if (faceFlux > 0)
      {
        gradcf = gradfV & (d & gradcP);
      }
      else
      {
        gradcf = gradfV & (d & gradcN);
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
      const vector& phiP,
      const vector& phiN,
      const tensor& gradcP,
      const tensor& gradcN,
      const vector& d
    ) const
    {
      vector gradfV = phiN - phiP;
      scalar gradf = gradfV & gradfV;
      scalar gradcf;
      if (faceFlux > 0)
      {
        gradcf = gradfV & (d & gradcP);
      }
      else
      {
        gradcf = gradfV & (d & gradcN);
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
