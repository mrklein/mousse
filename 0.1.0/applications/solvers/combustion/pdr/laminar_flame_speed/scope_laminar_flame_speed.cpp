// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ifstream.hpp"
#include "scope_laminar_flame_speed.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace laminarFlameSpeedModels {

DEFINE_TYPE_NAME_AND_DEBUG(SCOPE, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  laminarFlameSpeed,
  SCOPE,
  dictionary
);

}
}


// Constructors 
mousse::laminarFlameSpeedModels::SCOPE::polynomial::polynomial
(
  const dictionary& polyDict
)
:
  FixedList<scalar, 7>{polyDict.lookup("coefficients")},
  ll{readScalar(polyDict.lookup("lowerLimit"))},
  ul{readScalar(polyDict.lookup("upperLimit"))},
  llv{polyPhi(ll, *this)},
  ulv{polyPhi(ul, *this)},
  lu{0}
{}


mousse::laminarFlameSpeedModels::SCOPE::SCOPE
(
  const dictionary& dict,
  const psiuReactionThermo& ct
)
:
  laminarFlameSpeed{dict, ct},
  coeffsDict_
  {
    dictionary
    {
      IFstream{fileName{dict.lookup("fuelFile")}}()
    }.subDict(typeName + "Coeffs")
  },
  LFL_{readScalar(coeffsDict_.lookup("lowerFlamabilityLimit"))},
  UFL_{readScalar(coeffsDict_.lookup("upperFlamabilityLimit"))},
  SuPolyL_{coeffsDict_.subDict("lowerSuPolynomial")},
  SuPolyU_{coeffsDict_.subDict("upperSuPolynomial")},
  Texp_{readScalar(coeffsDict_.lookup("Texp"))},
  pexp_{readScalar(coeffsDict_.lookup("pexp"))},
  MaPolyL_{coeffsDict_.subDict("lowerMaPolynomial")},
  MaPolyU_{coeffsDict_.subDict("upperMaPolynomial")}
{
  SuPolyL_.ll = max(SuPolyL_.ll, LFL_) + SMALL;
  SuPolyU_.ul = min(SuPolyU_.ul, UFL_) - SMALL;
  SuPolyL_.lu = 0.5*(SuPolyL_.ul + SuPolyU_.ll);
  SuPolyU_.lu = SuPolyL_.lu - SMALL;
  MaPolyL_.lu = 0.5*(MaPolyL_.ul + MaPolyU_.ll);
  MaPolyU_.lu = MaPolyL_.lu - SMALL;
  if (debug) {
    Info << "phi     Su  (T = Tref, p = pref)" << endl;
    label n = 200;
    for (int i=0; i<n; i++) {
      scalar phi = (2.0*i)/n;
      Info << phi << token::TAB << SuRef(phi) << endl;
    }
  }
}


// Destructor 
mousse::laminarFlameSpeedModels::SCOPE::~SCOPE()
{}


// Member Functions 
inline mousse::scalar mousse::laminarFlameSpeedModels::SCOPE::polyPhi
(
  scalar phi,
  const polynomial& a
)
{
  scalar x = phi - 1.0;
  return
    a[0]*(scalar(1) + x*(a[1] + x*(a[2] + x*(a[3] + x*(a[4] + x*(a[5]
                                                                 + x*a[6]))))));
}


inline mousse::scalar mousse::laminarFlameSpeedModels::SCOPE::SuRef
(
  scalar phi
) const
{
  if (phi < LFL_ || phi > UFL_) {
    // Return 0 beyond the flamibility limits
    return scalar(0);
  } else if (phi < SuPolyL_.ll) {
    // Use linear interpolation between the low end of the
    // lower polynomial and the lower flamibility limit
    return SuPolyL_.llv*(phi - LFL_)/(SuPolyL_.ll - LFL_);
  } else if (phi > SuPolyU_.ul) {
    // Use linear interpolation between the upper end of the
    // upper polynomial and the upper flamibility limit
    return SuPolyU_.ulv*(UFL_ - phi)/(UFL_ - SuPolyU_.ul);
  } else if (phi < SuPolyL_.lu) {
    // Evaluate the lower polynomial
    return polyPhi(phi, SuPolyL_);
  } else if (phi > SuPolyU_.lu) {
    // Evaluate the upper polynomial
    return polyPhi(phi, SuPolyU_);
  } else {
    FATAL_ERROR_IN("laminarFlameSpeedModels::SCOPE::SuRef(scalar phi)")
      << "phi = " << phi
      << " cannot be handled by SCOPE function with the given coefficients"
      << exit(FatalError);
    return scalar(0);
  }
}


inline mousse::scalar mousse::laminarFlameSpeedModels::SCOPE::Ma
(
  scalar phi
) const
{
  if (phi < MaPolyL_.ll) {
    // Beyond the lower limit assume Ma is constant
    return MaPolyL_.llv;
  } else if (phi > MaPolyU_.ul) {
    // Beyond the upper limit assume Ma is constant
    return MaPolyU_.ulv;
  } else if (phi < SuPolyL_.lu) {
    // Evaluate the lower polynomial
    return polyPhi(phi, MaPolyL_);
  } else if (phi > SuPolyU_.lu) {
    // Evaluate the upper polynomial
    return polyPhi(phi, MaPolyU_);
  } else {
    FATAL_ERROR_IN("laminarFlameSpeedModels::SCOPE::Ma(scalar phi)")
      << "phi = " << phi
      << " cannot be handled by SCOPE function with the given coefficients"
      << exit(FatalError);
    return scalar(0);
  }
}


inline mousse::scalar mousse::laminarFlameSpeedModels::SCOPE::Su0pTphi
(
  scalar p,
  scalar Tu,
  scalar phi
) const
{
  static const scalar Tref = 300.0;
  static const scalar pRef = 1.013e5;
  return SuRef(phi)*pow((Tu/Tref), Texp_)*pow((p/pRef), pexp_);
}


mousse::tmp<mousse::volScalarField> mousse::laminarFlameSpeedModels::SCOPE::Su0pTphi
(
  const volScalarField& p,
  const volScalarField& Tu,
  scalar phi
) const
{
  tmp<volScalarField> tSu0
  {
    new volScalarField
    {
      IOobject
      {
        "Su0",
        p.time().timeName(),
        p.db()
      },
      p.mesh(),
      {"Su0", dimVelocity, 0.0}
    }
  };
  volScalarField& Su0 = tSu0();
  FOR_ALL(Su0, celli) {
    Su0[celli] = Su0pTphi(p[celli], Tu[celli], phi);
  }
  FOR_ALL(Su0.boundaryField(), patchi) {
    scalarField& Su0p = Su0.boundaryField()[patchi];
    const scalarField& pp = p.boundaryField()[patchi];
    const scalarField& Tup = Tu.boundaryField()[patchi];
    FOR_ALL(Su0p, facei) {
      Su0p[facei] = Su0pTphi(pp[facei], Tup[facei], phi);
    }
  }
  return tSu0;
}


mousse::tmp<mousse::volScalarField> mousse::laminarFlameSpeedModels::SCOPE::Su0pTphi
(
  const volScalarField& p,
  const volScalarField& Tu,
  const volScalarField& phi
) const
{
  tmp<volScalarField> tSu0
  {
    new volScalarField
    {
      IOobject
      {
        "Su0",
        p.time().timeName(),
        p.db()
      },
      p.mesh(),
      {"Su0", dimVelocity, 0.0}
    }
  };
  volScalarField& Su0 = tSu0();
  FOR_ALL(Su0, celli) {
    Su0[celli] = Su0pTphi(p[celli], Tu[celli], phi[celli]);
  }
  FOR_ALL(Su0.boundaryField(), patchi) {
    scalarField& Su0p = Su0.boundaryField()[patchi];
    const scalarField& pp = p.boundaryField()[patchi];
    const scalarField& Tup = Tu.boundaryField()[patchi];
    const scalarField& phip = phi.boundaryField()[patchi];
    FOR_ALL(Su0p, facei) {
      Su0p[facei] = Su0pTphi(pp[facei], Tup[facei], phip[facei]);
    }
  }
  return tSu0;
}


mousse::tmp<mousse::volScalarField> mousse::laminarFlameSpeedModels::SCOPE::Ma
(
  const volScalarField& phi
) const
{
  tmp<volScalarField> tMa
  {
    new volScalarField
    {
      IOobject
      {
        "Ma",
        phi.time().timeName(),
        phi.db()
      },
      phi.mesh(),
      {"Ma", dimless, 0.0}
    }
  };
  volScalarField& ma = tMa();
  FOR_ALL(ma, celli) {
    ma[celli] = Ma(phi[celli]);
  }
  FOR_ALL(ma.boundaryField(), patchi) {
    scalarField& map = ma.boundaryField()[patchi];
    const scalarField& phip = phi.boundaryField()[patchi];
    FOR_ALL(map, facei) {
      map[facei] = Ma(phip[facei]);
    }
  }
  return tMa;
}


mousse::tmp<mousse::volScalarField>
mousse::laminarFlameSpeedModels::SCOPE::Ma() const
{
  if (psiuReactionThermo_.composition().contains("ft")) {
    const volScalarField& ft = psiuReactionThermo_.composition().Y("ft");
    return
      Ma
      (
        dimensionedScalar
        {
          psiuReactionThermo_.lookup("stoichiometricAirFuelMassRatio")
        }*ft/(scalar(1) - ft)
      );
  } else {
    const fvMesh& mesh = psiuReactionThermo_.p().mesh();
    return
      tmp<volScalarField>
      {
        new volScalarField
        {
          IOobject
          {
            "Ma",
            mesh.time().timeName(),
            mesh
          },
          mesh,
          {"Ma", dimless, Ma(equivalenceRatio_)}
        }
      };
  }
}


mousse::tmp<mousse::volScalarField>
mousse::laminarFlameSpeedModels::SCOPE::operator()() const
{
  if (psiuReactionThermo_.composition().contains("ft")) {
    const volScalarField& ft = psiuReactionThermo_.composition().Y("ft");
    return
      Su0pTphi
      (
        psiuReactionThermo_.p(),
        psiuReactionThermo_.Tu(),
        dimensionedScalar
        {
          psiuReactionThermo_.lookup("stoichiometricAirFuelMassRatio")
        }*ft/(scalar(1) - ft)
      );
  } else {
    return
      Su0pTphi
      (
        psiuReactionThermo_.p(),
        psiuReactionThermo_.Tu(),
        equivalenceRatio_
      );
  }
}

