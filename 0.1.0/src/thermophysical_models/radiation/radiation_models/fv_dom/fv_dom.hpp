#ifndef THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_FV_DOM_FV_DOM_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_FV_DOM_FV_DOM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::fvDOM
// Description
//   Finite Volume Discrete Ordinates Method. Solves the RTE equation for n
//   directions in a participating media, not including scatter.
//   Available absorption models:
//     constantAbsorptionEmission
//     greyMeanAbsoprtionEmission
//     wideBandAbsorptionEmission
//   i.e. dictionary
//   \verbatim
//     fvDOMCoeffs
//     {
//       nPhi        4;          // azimuthal angles in PI/2 on X-Y.
//                   //(from Y to X)
//       nTheta      0;          // polar angles in PI (from Z to X-Y plane)
//       convergence 1e-3;       // convergence criteria for radiation
//                   //iteration
//       maxIter     4;          // maximum number of iterations
//       cacheDiv    true;       // cache the div of the RTE equation.
//       //NOTE: Caching div is "only" accurate if the upwind scheme is used
//       //in div(Ji,Ii_h)
//     }
//     solverFreq   1; // Number of flow iterations per radiation iteration
//   \endverbatim
//   The total number of solid angles is  4*nPhi*nTheta.
//   In 1D the direction of the rays is X (nPhi and nTheta are ignored)
//   In 2D the direction of the rays is on X-Y plane (only nPhi is considered)
//   In 3D (nPhi and nTheta are considered)

#include "radiative_intensity_ray.hpp"
#include "radiation_model.hpp"
#include "fv_matrices.hpp"


namespace mousse {
namespace radiation {

class fvDOM
:
  public radiationModel
{
  // Private data
    //- Incident radiation  [W/m2]
    volScalarField G_;
    //- Total radiative heat flux [W/m2]
    volScalarField Qr_;
    //- Emmited radiative heat flux [W/m2]
    volScalarField Qem_;
    //- Incidet radiative heat flux [W/m2]
    volScalarField Qin_;
    //- Total absorption coefficient [1/m]
    volScalarField a_;
    //- Number of solid angles in theta
    label nTheta_;
    //- Number of solid angles in phi
    label nPhi_ ;
    //- Total number of rays (1 per direction)
    label nRay_;
    //- Number of wavelength bands
    label nLambda_;
    //- Wavelength total absorption coefficient [1/m]
    PtrList<volScalarField> aLambda_;
    //- Black body
    blackBodyEmission blackBody_;
    //- List of pointers to radiative intensity rays
    PtrList<radiativeIntensityRay> IRay_;
    //- Convergence criterion
    scalar convergence_;
    //- Maximum number of iterations
    label maxIter_;
    //- List of cached fvMatrices for rays
    List<PtrList<fvScalarMatrix>> fvRayDiv_;
    //- Cache convection div matrix
    bool cacheDiv_;
    //- Maximum omega weight
    scalar omegaMax_;
  // Private Member Functions
    //- Initialise
    void initialise();
    //- Update nlack body emission
    void updateBlackBodyEmission();
public:
  //- Runtime type information
  TYPE_NAME("fvDOM");
  // Constructors
    //- Construct from components
    fvDOM(const volScalarField& T);
    //- Construct from components
    fvDOM(const dictionary& dict, const volScalarField& T);
    //- Disallow default bitwise copy construct
    fvDOM(const fvDOM&) = delete;
    //- Disallow default bitwise assignment
    fvDOM& operator=(const fvDOM&) = delete;
  //- Destructor
  virtual ~fvDOM();
  // Member functions
    // Edit
      //- Solve radiation equation(s)
      void calculate();
      //- Read radiation properties dictionary
      bool read();
      //- Update G and calculate total heat flux on boundary
      void updateG();
      //- Set the rayId and lambdaId from by decomposing an intensity
      //  field name
      void setRayIdLambdaId
      (
        const word& name,
        label& rayId,
        label& lambdaId
      ) const;
      //- Source term component (for power of T^4)
      virtual tmp<volScalarField> Rp() const;
      //- Source term component (constant)
      virtual tmp<DimensionedField<scalar, volMesh>> Ru() const;
    // Access
      //- Ray intensity for rayI
      inline const radiativeIntensityRay& IRay(const label rayI) const;
      //- Ray intensity for rayI and lambda bandwidth
      inline const volScalarField& IRayLambda
      (
        const label rayI,
        const label lambdaI
      ) const;
      //- Number of angles in theta
      inline label nTheta() const;
      //- Number of angles in phi
      inline label nPhi() const;
      //- Number of rays
      inline label nRay() const;
      //- Number of wavelengths
      inline label nLambda() const;
      //- Const access to total absorption coefficient
      inline const volScalarField& a() const;
      //- Const access to wavelength total absorption coefficient
      inline const volScalarField& aLambda(const label lambdaI) const;
      //- Const access to incident radiation field
      inline const volScalarField& G() const;
      //- Const access to total radiative heat flux field
      inline const volScalarField& Qr() const;
      //- Const access to incident radiative heat flux field
      inline const volScalarField& Qin() const;
      //- Const access to emitted radiative heat flux field
      inline const volScalarField& Qem() const;
      //- Const access to black body
      inline const blackBodyEmission& blackBody() const;
      //- Const access to cached fvMatrix
      inline const fvScalarMatrix& fvRayDiv
      (
        const label lambdaI,
        const label rayId
      ) const;
      //- Caching div(Ji, Ilamda)
      inline bool cacheDiv() const;
      //- Return omegaMax
      inline scalar omegaMax() const;
};


inline const mousse::radiation::radiativeIntensityRay&
mousse::radiation::fvDOM::IRay(const label rayI) const
{
  return  IRay_[rayI];
}


inline const mousse::volScalarField&
mousse::radiation::fvDOM::IRayLambda
(
  const label rayI,
  const label lambdaI
) const
{
  return IRay_[rayI].ILambda(lambdaI);
}


inline mousse::label mousse::radiation::fvDOM::nTheta() const
{
  return nTheta_;
}


inline mousse::label mousse::radiation::fvDOM::nPhi() const
{
  return nPhi_;
}


inline mousse::label mousse::radiation::fvDOM::nRay() const
{
  return nRay_;
}


inline mousse::label mousse::radiation::fvDOM::nLambda() const
{
  return nLambda_;
}


inline const mousse::volScalarField& mousse::radiation::fvDOM::a() const
{
  return a_;
}


inline const mousse::volScalarField& mousse::radiation::fvDOM::aLambda
(
  const label lambdaI
) const
{
  return aLambda_[lambdaI];
}


inline const mousse::volScalarField& mousse::radiation::fvDOM::G() const
{
  return G_;
}


inline const mousse::volScalarField& mousse::radiation::fvDOM::Qr() const
{
  return Qr_;
}


inline const mousse::volScalarField& mousse::radiation::fvDOM::Qin() const
{
  return Qin_;
}


inline const mousse::volScalarField& mousse::radiation::fvDOM::Qem() const
{
  return Qem_;
}


inline const mousse::radiation::blackBodyEmission&
mousse::radiation::fvDOM::blackBody() const
{
  return blackBody_;
}


inline const mousse::fvScalarMatrix& mousse::radiation::fvDOM::fvRayDiv
(
  const label rayId,
  const label lambdaI
) const
{
  return fvRayDiv_[lambdaI][rayId];
}


inline bool mousse::radiation::fvDOM::cacheDiv() const
{
  return cacheDiv_;
}


inline mousse::scalar mousse::radiation::fvDOM::omegaMax() const
{
  return omegaMax_;
}

}  // namespace radiation
}  // namespace mousse

#endif

