// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multiphase_system.hpp"
#include "alpha_contact_angle_fv_patch_scalar_field.hpp"
#include "fixed_value_fvs_patch_fields.hpp"
#include "time.hpp"
#include "sub_cycle.hpp"
#include "mules.hpp"
#include "surface_interpolate.hpp"
#include "fvc_grad.hpp"
#include "fvc_sn_grad.hpp"
#include "fvc_div.hpp"
#include "fvc_flux.hpp"
#include "fvc_average.hpp"
// Static Member Data 
const mousse::scalar mousse::multiphaseSystem::convertToRad =
  mousse::constant::mathematical::pi/180.0;
// Private Member Functions 
void mousse::multiphaseSystem::calcAlphas()
{
  scalar level = 0.0;
  alphas_ == 0.0;
  FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, iter)
  {
    alphas_ += level*iter();
    level += 1.0;
  }
  alphas_.correctBoundaryConditions();
}
void mousse::multiphaseSystem::solveAlphas()
{
  PtrList<surfaceScalarField> alphaPhiCorrs{phases_.size()};
  int phasei = 0;
  FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, iter)
  {
    phaseModel& phase1 = iter();
    volScalarField& alpha1 = phase1;
    phase1.alphaPhi() = dimensionedScalar{"0", {0, 3, -1, 0, 0}, 0};
    alphaPhiCorrs.set
    (
      phasei,
      new surfaceScalarField
      {
        "phi" + alpha1.name() + "Corr",
        fvc::flux
        (
          phi_,
          phase1,
          "div(phi," + alpha1.name() + ')'
        )
      }
    );
    surfaceScalarField& alphaPhiCorr = alphaPhiCorrs[phasei];
    FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, iter2)
    {
      phaseModel& phase2 = iter2();
      volScalarField& alpha2 = phase2;
      if (&phase2 == &phase1) continue;
      surfaceScalarField phir{phase1.phi() - phase2.phi()};
      scalarCoeffSymmTable::const_iterator cAlpha
      {
        cAlphas_.find(interfacePair(phase1, phase2))
      };
      if (cAlpha != cAlphas_.end())
      {
        surfaceScalarField phic
        {
          (mag(phi_) + mag(phir))/mesh_.magSf()
        };
        phir += min(cAlpha()*phic, max(phic))*nHatf(phase1, phase2);
      }
      word phirScheme
      {
        "div(phir," + alpha2.name() + ',' + alpha1.name() + ')'
      };
      alphaPhiCorr += fvc::flux
      (
        -fvc::flux(-phir, phase2, phirScheme),
        phase1,
        phirScheme
      );
    }
    // Ensure that the flux at inflow BCs is preserved
    FOR_ALL(alphaPhiCorr.boundaryField(), patchi)
    {
      fvsPatchScalarField& alphaPhiCorrp = alphaPhiCorr.boundaryField()[patchi];
      if (!alphaPhiCorrp.coupled())
      {
        const scalarField& phi1p = phase1.phi().boundaryField()[patchi];
        const scalarField& alpha1p = alpha1.boundaryField()[patchi];
        FOR_ALL(alphaPhiCorrp, facei)
        {
          if (phi1p[facei] < 0)
          {
            alphaPhiCorrp[facei] = alpha1p[facei]*phi1p[facei];
          }
        }
      }
    }
    MULES::limit
    (
      1.0/mesh_.time().deltaT().value(),
      geometricOneField(),
      phase1,
      phi_,
      alphaPhiCorr,
      zeroField(),
      zeroField(),
      1,
      0,
      true
    );
    phasei++;
  }
  MULES::limitSum(alphaPhiCorrs);
  volScalarField sumAlpha
  {
    {
      "sumAlpha",
      mesh_.time().timeName(),
      mesh_
    },
    mesh_,
    {"sumAlpha", dimless, 0}
  };
  phasei = 0;
  FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, iter)
  {
    phaseModel& phase1 = iter();
    surfaceScalarField& alphaPhi = alphaPhiCorrs[phasei];
    alphaPhi += upwind<scalar>(mesh_, phi_).flux(phase1);
    MULES::explicitSolve
    (
      geometricOneField(),
      phase1,
      alphaPhi,
      zeroField(),
      zeroField()
    );
    phase1.alphaPhi() += alphaPhi;
    Info << phase1.name() << " volume fraction, min, max = "
      << phase1.weightedAverage(mesh_.V()).value()
      << ' ' << min(phase1).value()
      << ' ' << max(phase1).value()
      << endl;
    sumAlpha += phase1;
    phasei++;
  }
  Info << "Phase-sum volume fraction, min, max = "
    << sumAlpha.weightedAverage(mesh_.V()).value()
    << ' ' << min(sumAlpha).value()
    << ' ' << max(sumAlpha).value()
    << endl;
  calcAlphas();
}
mousse::tmp<mousse::surfaceVectorField> mousse::multiphaseSystem::nHatfv
(
  const volScalarField& alpha1,
  const volScalarField& alpha2
) const
{
  surfaceVectorField gradAlphaf
  {
    fvc::interpolate(alpha2)*fvc::interpolate(fvc::grad(alpha1))
    - fvc::interpolate(alpha1)*fvc::interpolate(fvc::grad(alpha2))
  };
  // Face unit interface normal
  return gradAlphaf/(mag(gradAlphaf) + deltaN_);
}
mousse::tmp<mousse::surfaceScalarField> mousse::multiphaseSystem::nHatf
(
  const volScalarField& alpha1,
  const volScalarField& alpha2
) const
{
  // Face unit interface normal flux
  return nHatfv(alpha1, alpha2) & mesh_.Sf();
}
// Correction for the boundary condition on the unit normal nHat on
// walls to produce the correct contact angle.
// The dynamic contact angle is calculated from the component of the
// velocity on the direction of the interface, parallel to the wall.
void mousse::multiphaseSystem::correctContactAngle
(
  const phaseModel& phase1,
  const phaseModel& phase2,
  surfaceVectorField::GeometricBoundaryField& nHatb
) const
{
  const volScalarField::GeometricBoundaryField& gbf
    = phase1.boundaryField();
  const fvBoundaryMesh& boundary = mesh_.boundary();
  FOR_ALL(boundary, patchi)
  {
    if (isA<alphaContactAngleFvPatchScalarField>(gbf[patchi]))
    {
      const alphaContactAngleFvPatchScalarField& acap =
        refCast<const alphaContactAngleFvPatchScalarField>(gbf[patchi]);
      vectorField& nHatPatch = nHatb[patchi];
      vectorField AfHatPatch
      {
        mesh_.Sf().boundaryField()[patchi]
        /mesh_.magSf().boundaryField()[patchi]
      };
      alphaContactAngleFvPatchScalarField::thetaPropsTable::
        const_iterator tp =
        acap.thetaProps().find(interfacePair(phase1, phase2));
      if (tp == acap.thetaProps().end())
      {
        FATAL_ERROR_IN
        (
          "multiphaseSystem::correctContactAngle"
          "(const phaseModel& phase1, const phaseModel& phase2, "
          "fvPatchVectorFieldField& nHatb) const"
        )
        << "Cannot find interface " << interfacePair(phase1, phase2)
        << "\n    in table of theta properties for patch "
        << acap.patch().name()
        << exit(FatalError);
      }
      bool matched = (tp.key().first() == phase1.name());
      scalar theta0 = convertToRad*tp().theta0(matched);
      scalarField theta(boundary[patchi].size(), theta0);
      scalar uTheta = tp().uTheta();
      // Calculate the dynamic contact angle if required
      if (uTheta > SMALL)
      {
        scalar thetaA = convertToRad*tp().thetaA(matched);
        scalar thetaR = convertToRad*tp().thetaR(matched);
        // Calculated the component of the velocity parallel to the wall
        vectorField Uwall
        {
          phase1.U().boundaryField()[patchi].patchInternalField()
          - phase1.U().boundaryField()[patchi]
        };
        Uwall -= (AfHatPatch & Uwall)*AfHatPatch;
        // Find the direction of the interface parallel to the wall
        vectorField nWall
        {
          nHatPatch - (AfHatPatch & nHatPatch)*AfHatPatch
        };
        // Normalise nWall
        nWall /= (mag(nWall) + SMALL);
        // Calculate Uwall resolved normal to the interface parallel to
        // the interface
        scalarField uwall{nWall & Uwall};
        theta += (thetaA - thetaR)*tanh(uwall/uTheta);
      }
      // Reset nHatPatch to correspond to the contact angle
      scalarField a12{nHatPatch & AfHatPatch};
      scalarField b1{cos(theta)};
      scalarField b2{nHatPatch.size()};
      FOR_ALL(b2, facei)
      {
        b2[facei] = cos(acos(a12[facei]) - theta[facei]);
      }
      scalarField det{1.0 - a12*a12};
      scalarField a{(b1 - a12*b2)/det};
      scalarField b{(b2 - a12*b1)/det};
      nHatPatch = a*AfHatPatch + b*nHatPatch;
      nHatPatch /= (mag(nHatPatch) + deltaN_.value());
    }
  }
}
mousse::tmp<mousse::volScalarField> mousse::multiphaseSystem::K
(
  const phaseModel& phase1,
  const phaseModel& phase2
) const
{
  tmp<surfaceVectorField> tnHatfv = nHatfv(phase1, phase2);
  correctContactAngle(phase1, phase2, tnHatfv().boundaryField());
  // Simple expression for curvature
  return -fvc::div(tnHatfv & mesh_.Sf());
}
// Constructors 
mousse::multiphaseSystem::multiphaseSystem
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  IOdictionary
  (
    {
      "transportProperties",
      U.time().constant(),
      U.db(),
      IOobject::MUST_READ_IF_MODIFIED
    }
  ),
  phases_{lookup("phases"), phaseModel::iNew(U.mesh())},
  mesh_{U.mesh()},
  phi_{phi},
  alphas_
  {
    {
      "alphas",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"alphas", dimless, 0.0},
    zeroGradientFvPatchScalarField::typeName
  },
  sigmas_{lookup("sigmas")},
  dimSigma_{1, 0, -2, 0, 0},
  cAlphas_{lookup("interfaceCompression")},
  Cvms_{lookup("virtualMass")},
  deltaN_
  {
    "deltaN",
    1e-8/pow(average(mesh_.V()), 1.0/3.0)
  }
{
  calcAlphas();
  alphas_.write();
  interfaceDictTable dragModelsDict(lookup("drag"));
  FOR_ALL_CONST_ITER(interfaceDictTable, dragModelsDict, iter)
  {
    dragModels_.insert
    (
      iter.key(),
      dragModel::New
      (
        iter(),
        *phases_.lookup(iter.key().first()),
        *phases_.lookup(iter.key().second())
      ).ptr()
    );
  }
  FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, iter1)
  {
    const phaseModel& phase1 = iter1();
    FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, iter2)
    {
      const phaseModel& phase2 = iter2();
      if (&phase2 != &phase1)
      {
        scalarCoeffSymmTable::const_iterator sigma
        (
          sigmas_.find(interfacePair(phase1, phase2))
        );
        if (sigma != sigmas_.end())
        {
          scalarCoeffSymmTable::const_iterator cAlpha
          (
            cAlphas_.find(interfacePair(phase1, phase2))
          );
          if (cAlpha == cAlphas_.end())
          {
            WARNING_IN
            (
              "multiphaseSystem::multiphaseSystem"
              "(const volVectorField& U,"
              "const surfaceScalarField& phi)"
            )
            << "Compression coefficient not specified for phase pair ("
            << phase1.name() << ' ' << phase2.name()
            << ") for which a surface tension coefficient is specified"
            << endl;
          }
        }
      }
    }
  }
}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::multiphaseSystem::rho() const
{
  PtrDictionary<phaseModel>::const_iterator iter = phases_.begin();
  tmp<volScalarField> trho = iter()*iter().rho();
  for (++iter; iter != phases_.end(); ++iter)
  {
    trho() += iter()*iter().rho();
  }
  return trho;
}
mousse::tmp<mousse::scalarField>
mousse::multiphaseSystem::rho(const label patchi) const
{
  PtrDictionary<phaseModel>::const_iterator iter = phases_.begin();
  tmp<scalarField> trho = iter().boundaryField()[patchi]*iter().rho().value();
  for (++iter; iter != phases_.end(); ++iter)
  {
    trho() += iter().boundaryField()[patchi]*iter().rho().value();
  }
  return trho;
}
mousse::tmp<mousse::volScalarField> mousse::multiphaseSystem::nu() const
{
  PtrDictionary<phaseModel>::const_iterator iter = phases_.begin();
  tmp<volScalarField> tmu = iter()*(iter().rho()*iter().nu());
  for (++iter; iter != phases_.end(); ++iter)
  {
    tmu() += iter()*(iter().rho()*iter().nu());
  }
  return tmu/rho();
}
mousse::tmp<mousse::scalarField>
mousse::multiphaseSystem::nu(const label patchi) const
{
  PtrDictionary<phaseModel>::const_iterator iter = phases_.begin();
  tmp<scalarField> tmu =
    iter().boundaryField()[patchi]*(iter().rho().value()*iter().nu().value());
  for (++iter; iter != phases_.end(); ++iter)
  {
    tmu() +=
      iter().boundaryField()[patchi]*(iter().rho().value()*iter().nu().value());
  }
  return tmu/rho(patchi);
}
mousse::tmp<mousse::volScalarField> mousse::multiphaseSystem::Cvm
(
  const phaseModel& phase
) const
{
  tmp<volScalarField> tCvm
  {
    new volScalarField
    {
      {
        "Cvm",
        mesh_.time().timeName(),
        mesh_
      },
      mesh_,
      {"Cvm", {1, -3, 0, 0, 0}, 0}
    }
  };
  FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, iter)
  {
    const phaseModel& phase2 = iter();
    if (&phase2 != &phase)
    {
      scalarCoeffTable::const_iterator Cvm
      {
        Cvms_.find(interfacePair(phase, phase2))
      };
      if (Cvm != Cvms_.end())
      {
        tCvm() += Cvm()*phase2.rho()*phase2;
      }
      else
      {
        Cvm = Cvms_.find(interfacePair(phase2, phase));
        if (Cvm != Cvms_.end())
        {
          tCvm() += Cvm()*phase.rho()*phase2;
        }
      }
    }
  }
  return tCvm;
}
mousse::tmp<mousse::volVectorField> mousse::multiphaseSystem::Svm
(
  const phaseModel& phase
) const
{
  tmp<volVectorField> tSvm
  {
    new volVectorField
    {
      {
        "Svm",
        mesh_.time().timeName(),
        mesh_
      },
      mesh_,
      {"Svm", {1, -2, -2, 0, 0}, vector::zero}
    }
  };
  FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, iter)
  {
    const phaseModel& phase2 = iter();
    if (&phase2 != &phase)
    {
      scalarCoeffTable::const_iterator Cvm
      {
        Cvms_.find(interfacePair(phase, phase2))
      };
      if (Cvm != Cvms_.end())
      {
        tSvm() += Cvm()*phase2.rho()*phase2*phase2.DDtU();
      }
      else
      {
        Cvm = Cvms_.find(interfacePair(phase2, phase));
        if (Cvm != Cvms_.end())
        {
          tSvm() += Cvm()*phase.rho()*phase2*phase2.DDtU();
        }
      }
    }
  }
  // Remove virtual mass at fixed-flux boundaries
  FOR_ALL(phase.phi().boundaryField(), patchi)
  {
    if
    (
      isA<fixedValueFvsPatchScalarField>
      (
        phase.phi().boundaryField()[patchi]
      )
    )
    {
      tSvm().boundaryField()[patchi] = vector::zero;
    }
  }
  return tSvm;
}
mousse::autoPtr<mousse::multiphaseSystem::dragCoeffFields>
mousse::multiphaseSystem::dragCoeffs() const
{
  autoPtr<dragCoeffFields> dragCoeffsPtr{new dragCoeffFields};
  FOR_ALL_CONST_ITER(dragModelTable, dragModels_, iter)
  {
    const dragModel& dm = *iter();
    volScalarField* Kptr =
      (
        max
        (
          //fvc::average(dm.phase1()*dm.phase2()),
          //fvc::average(dm.phase1())*fvc::average(dm.phase2()),
          dm.phase1()*dm.phase2(),
          dm.residualPhaseFraction()
        )
       *dm.K
        (
          max
          (
            mag(dm.phase1().U() - dm.phase2().U()),
            dm.residualSlip()
          )
        )
      ).ptr();
    // Remove drag at fixed-flux boundaries
    FOR_ALL(dm.phase1().phi().boundaryField(), patchi)
    {
      if
      (
        isA<fixedValueFvsPatchScalarField>
        (
          dm.phase1().phi().boundaryField()[patchi]
        )
      )
      {
        Kptr->boundaryField()[patchi] = 0.0;
      }
    }
    dragCoeffsPtr().insert(iter.key(), Kptr);
  }
  return dragCoeffsPtr;
}
mousse::tmp<mousse::volScalarField> mousse::multiphaseSystem::dragCoeff
(
  const phaseModel& phase,
  const dragCoeffFields& dragCoeffs
) const
{
  tmp<volScalarField> tdragCoeff
  {
    new volScalarField
    {
      {
        "dragCoeff",
        mesh_.time().timeName(),
        mesh_
      },
      mesh_,
      {"dragCoeff", {1, -3, -1, 0, 0}, 0}
    }
  };
  dragModelTable::const_iterator dmIter = dragModels_.begin();
  dragCoeffFields::const_iterator dcIter = dragCoeffs.begin();
  for
  (
    ;
    dmIter != dragModels_.end() && dcIter != dragCoeffs.end();
    ++dmIter, ++dcIter
  )
  {
    if (&phase == &dmIter()->phase1() || &phase == &dmIter()->phase2())
    {
      tdragCoeff() += *dcIter();
    }
  }
  return tdragCoeff;
}
mousse::tmp<mousse::surfaceScalarField> mousse::multiphaseSystem::surfaceTension
(
  const phaseModel& phase1
) const
{
  tmp<surfaceScalarField> tSurfaceTension
  {
    new surfaceScalarField
    {
      {
        "surfaceTension",
        mesh_.time().timeName(),
        mesh_
      },
      mesh_,
      {"surfaceTension", {1, -2, -2, 0, 0}, 0}
    }
  };
  FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, iter)
  {
    const phaseModel& phase2 = iter();
    if (&phase2 != &phase1)
    {
      scalarCoeffSymmTable::const_iterator sigma
      (
        sigmas_.find(interfacePair(phase1, phase2))
      );
      if (sigma != sigmas_.end())
      {
        tSurfaceTension() +=
          dimensionedScalar("sigma", dimSigma_, sigma())
          *fvc::interpolate(K(phase1, phase2))*
          (
            fvc::interpolate(phase2)*fvc::snGrad(phase1)
            - fvc::interpolate(phase1)*fvc::snGrad(phase2)
          );
      }
    }
  }
  return tSurfaceTension;
}
mousse::tmp<mousse::volScalarField>
mousse::multiphaseSystem::nearInterface() const
{
  tmp<volScalarField> tnearInt
  {
    new volScalarField
    {
      {
        "nearInterface",
        mesh_.time().timeName(),
        mesh_
      },
      mesh_,
      {"nearInterface", dimless, 0.0}
    }
  };
  FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, iter)
  {
    tnearInt() = max(tnearInt(), pos(iter() - 0.01)*pos(0.99 - iter()));
  }
  return tnearInt;
}
void mousse::multiphaseSystem::solve()
{
  FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, iter)
  {
    iter().correct();
  }
  const Time& runTime = mesh_.time();
  const dictionary& alphaControls = mesh_.solverDict("alpha");
  label nAlphaSubCycles{readLabel(alphaControls.lookup("nAlphaSubCycles"))};
  if (nAlphaSubCycles > 1)
  {
    dimensionedScalar totalDeltaT = runTime.deltaT();
    PtrList<volScalarField> alpha0s{phases_.size()};
    PtrList<surfaceScalarField> alphaPhiSums{phases_.size()};
    int phasei = 0;
    FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, iter)
    {
      phaseModel& phase = iter();
      volScalarField& alpha = phase;
      alpha0s.set
      (
        phasei,
        new volScalarField{alpha.oldTime()}
      );
      alphaPhiSums.set
      (
        phasei,
        new surfaceScalarField
        {
          {
            "phiSum" + alpha.name(),
            runTime.timeName(),
            mesh_
          },
          mesh_,
          {"0", {0, 3, -1, 0, 0}, 0}
        }
      );
      phasei++;
    }
    for
    (
      subCycleTime alphaSubCycle
      (
        const_cast<Time&>(runTime),
        nAlphaSubCycles
      );
      !(++alphaSubCycle).end();
    )
    {
      solveAlphas();
      int phasei = 0;
      FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, iter)
      {
        alphaPhiSums[phasei] += iter().alphaPhi()/nAlphaSubCycles;
        phasei++;
      }
    }
    phasei = 0;
    FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, iter)
    {
      phaseModel& phase = iter();
      volScalarField& alpha = phase;
      phase.alphaPhi() = alphaPhiSums[phasei];
      // Correct the time index of the field
      // to correspond to the global time
      alpha.timeIndex() = runTime.timeIndex();
      // Reset the old-time field value
      alpha.oldTime() = alpha0s[phasei];
      alpha.oldTime().timeIndex() = runTime.timeIndex();
      phasei++;
    }
  }
  else
  {
    solveAlphas();
  }
}
bool mousse::multiphaseSystem::read()
{
  if (regIOobject::read())
  {
    bool readOK = true;
    PtrList<entry> phaseData{lookup("phases")};
    label phasei = 0;
    FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, iter)
    {
      readOK &= iter().read(phaseData[phasei++].dict());
    }
    lookup("sigmas") >> sigmas_;
    lookup("interfaceCompression") >> cAlphas_;
    lookup("virtualMass") >> Cvms_;
    return readOK;
  }
  else
  {
    return false;
  }
}
