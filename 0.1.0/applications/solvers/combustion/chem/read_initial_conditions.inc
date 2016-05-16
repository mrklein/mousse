  word constProp{initialConditions.lookup("constantProperty")};
  if ((constProp != "pressure") && (constProp != "volume")) {
    FatalError << "in initialConditions, unknown constantProperty type "
      << constProp << nl << " Valid types are: pressure volume."
      << abort(FatalError);
  }
  word fractionBasis{initialConditions.lookup("fractionBasis")};
  if ((fractionBasis != "mass") && (fractionBasis != "mole")) {
    FatalError << "in initialConditions, unknown fractionBasis type " << nl
      << "Valid types are: mass or mole."
      << fractionBasis << abort(FatalError);
  }
  label nSpecie = Y.size();
  PtrList<gasHThermoPhysics> specieData{Y.size()};
  FOR_ALL(specieData, i) {
    specieData.set
    (
      i,
      new gasHThermoPhysics
      {
        dynamic_cast<const reactingMixture<gasHThermoPhysics>&>
          (thermo).speciesData()[i]
      }
    );
  }
  scalarList Y0{nSpecie, 0.0};
  scalarList X0{nSpecie, 0.0};
  dictionary fractions{initialConditions.subDict("fractions")};
  if (fractionBasis == "mole") {
    FOR_ALL(Y, i) {
      const word& name = Y[i].name();
      if (fractions.found(name)) {
        X0[i] = readScalar(fractions.lookup(name));
      }
    }
    scalar mw = 0.0;
    const scalar mTot = sum(X0);
    FOR_ALL(Y, i) {
      X0[i] /= mTot;
      mw += specieData[i].W()*X0[i];
    }
    FOR_ALL(Y, i) {
      Y0[i] = X0[i]*specieData[i].W()/mw;
    }
  } else {  // mass fraction
    FOR_ALL(Y, i) {
      const word& name = Y[i].name();
      if (fractions.found(name)) {
        Y0[i] = readScalar(fractions.lookup(name));
      }
    }
    scalar invW = 0.0;
    const scalar mTot = sum(Y0);
    FOR_ALL(Y, i) {
      Y0[i] /= mTot;
      invW += Y0[i]/specieData[i].W();
    }
    const scalar mw = 1.0/invW;
    FOR_ALL(Y, i) {
      X0[i] = Y0[i]*mw/specieData[i].W();
    }
  }
  scalar h0 = 0.0;
  FOR_ALL(Y, i) {
    Y[i] = Y0[i];
    h0 += Y0[i]*specieData[i].Hs(p[0], T0);
  }
  thermo.he() = {"h", dimEnergy/dimMass, h0};
  thermo.correct();
  rho = thermo.rho();
  scalar rho0 = rho[0];
  scalar u0 = h0 - p0/rho0;
  scalar R0 = p0/(rho0*T0);
  Rspecific[0] = R0;
  scalar integratedHeat = 0.0;
  Info << constProp << " will be held constant." << nl
    << " p   = " << p[0] << " [Pa]" << nl
    << " T   = " << thermo.T()[0] << " [K] " << nl
    << " rho = " << rho[0] << " [kg/m3]" << nl
    << endl;
