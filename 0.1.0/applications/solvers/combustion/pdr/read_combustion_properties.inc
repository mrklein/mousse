  Info << "Reading combustion properties\n" << endl;
  IOdictionary combustionProperties
  {
    {
      "combustionProperties",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  ignition ign{combustionProperties, runTime, mesh};
