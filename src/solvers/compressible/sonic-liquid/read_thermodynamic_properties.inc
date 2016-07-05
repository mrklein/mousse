  Info << "Reading thermodynamicProperties\n" << endl;
  IOdictionary thermodynamicProperties
  {
    {
      "thermodynamicProperties",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  };
  dimensionedScalar rho0{"rho0", dimDensity, thermodynamicProperties};
  dimensionedScalar p0{"p0", dimPressure, thermodynamicProperties};
  dimensionedScalar psi{"psi", dimCompressibility, thermodynamicProperties};
  // Density offset, i.e. the constant part of the density
  dimensionedScalar rhoO{"rhoO", rho0 - psi*p0};
