// mousse: CFD toolbox

FoamFile
{
  version     2.0;
  format      ascii;
  class       volScalarField;
  location    "0";
  object      nut.k;
}

dimensions      [0 2 -1 0 0 0 0];
internalField   uniform 0;

boundaryField
{
  lowerWall
  {
    type            nutkWallFunction;
    value           $internalField;
  }
  upperWall
  {
    type            nutkWallFunction;
    value           $internalField;
  }
  front
  {
    type            cyclic;
  }
  back
  {
    type            cyclic;
  }
  defaultFaces
  {
    type            empty;
  }
}

// vim: set ft=foam et sw=2 ts=2 sts=2:
