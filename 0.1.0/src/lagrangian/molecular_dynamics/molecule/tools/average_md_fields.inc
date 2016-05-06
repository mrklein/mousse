if (runTime.outputTime()) 
  scalarField totalRhoN_sum{mesh.nCells(), 0.0};
  FOR_ALL(allSpeciesRhoN, rN) {
    allSpeciesRhoN[rN].internalField() =
      (allSpeciesN_RU[rN]/mesh.cellVolumes()/nAveragingSteps);
    totalRhoN_sum += allSpeciesRhoN[rN].internalField();
  }
  totalRhoN.internalField() = totalRhoN_sum;
  scalarField totalRhoM_sum{mesh.nCells(), 0.0};
  FOR_ALL(allSpeciesRhoM, rM) {
    allSpeciesRhoM[rM].internalField() =
      (allSpeciesM_RU[rM]/mesh.cellVolumes()/nAveragingSteps);
    totalRhoM_sum += allSpeciesRhoM[rM].internalField();
  }
  totalRhoM.internalField() = totalRhoM_sum;
  vectorField totalMomentum_sum{mesh.nCells(), vector::zero};
  scalarField totalMass_sum{mesh.nCells(), 0.0};
  FOR_ALL(allSpeciesVelocity, v) {
    // A check for 1/0 molecules is required.
    vectorField& singleSpeciesVelocity = allSpeciesVelocity[v].internalField();
    FOR_ALL(singleSpeciesVelocity, sSV) {
      if (allSpeciesN_RU[v][sSV]) {
        singleSpeciesVelocity[sSV] =
         (allSpeciesVelocitySum_RU[v][sSV]/allSpeciesN_RU[v][sSV]);
        totalMomentum_sum[sSV] += (allSpeciesM_RU[v][sSV]
          /allSpeciesN_RU[v][sSV]*allSpeciesVelocitySum_RU[v][sSV]);
        totalMass_sum[sSV] += allSpeciesM_RU[v][sSV];
      } else {
        singleSpeciesVelocity[sSV] = vector::zero;
      }
    }
  }
  volVectorField::InternalField& itotalVelocity = totalVelocity.internalField();
  FOR_ALL(itotalVelocity, tV) {
    if (totalMass_sum[tV] > VSMALL) {
      itotalVelocity[tV] = totalMomentum_sum[tV]/totalMass_sum[tV];
    } else {
      itotalVelocity[tV] = vector::zero;
    }
  }
  scalarField totalTemperatureVTerms_sum{mesh.nCells(), 0.0};
  scalarField totalN_sum{mesh.nCells(), 0.0};
  FOR_ALL(allSpeciesTemperature, t) {
    // A check for 1/0 molecules is required.
    scalarField& singleSpeciesTemp = allSpeciesTemperature[t].internalField();
    FOR_ALL(singleSpeciesTemp, sST) {
      if (allSpeciesN_RU[t][sST]) {
        singleSpeciesTemp[sST] =
          allSpeciesM_RU[t][sST]/allSpeciesN_RU[t][sST]
          /(3.0*moleculeCloud::kb*allSpeciesN_RU[t][sST])
          *(allSpeciesVelocityMagSquaredSum_RU[t][sST]
            - (allSpeciesVelocitySum_RU[t][sST]
               & allSpeciesVelocitySum_RU[t][sST])
            /allSpeciesN_RU[t][sST]);
        totalTemperatureVTerms_sum[sST] +=
          allSpeciesM_RU[t][sST]
         /allSpeciesN_RU[t][sST]
         *(
            allSpeciesVelocityMagSquaredSum_RU[t][sST]
           -
            (
              allSpeciesVelocitySum_RU[t][sST]
              &
              allSpeciesVelocitySum_RU[t][sST]
            )
            /allSpeciesN_RU[t][sST]
          );
        totalN_sum[sST] += allSpeciesN_RU[t][sST];
      } else {
        singleSpeciesTemp[sST] = 0.0;
      }
    }
  }
  volScalarField::InternalField& itotalTemperature =
    totalTemperature.internalField();
  FOR_ALL(itotalTemperature, tT) {
    if (totalN_sum[tT] > 0) {
      itotalTemperature[tT] =
        totalTemperatureVTerms_sum[tT]/(3.0*moleculeCloud::kb*totalN_sum[tT]);
    } else {
      itotalTemperature[tT] = 0.0;
    }
  }
  scalarField totalKE_sum{mesh.nCells(), 0.0};
  FOR_ALL(allSpeciesMeanKE, mKE) {
    // A check for 1/0 molecules is required.
    scalarField& singleSpeciesMeanKE = allSpeciesMeanKE[mKE].internalField();
    FOR_ALL(singleSpeciesMeanKE, sSMKE) {
      if (allSpeciesN_RU[mKE][sSMKE]) {
        singleSpeciesMeanKE[sSMKE] =
          allSpeciesM_RU[mKE][sSMKE]/allSpeciesN_RU[mKE][sSMKE]
          /(2.0*allSpeciesN_RU[mKE][sSMKE])
          *(allSpeciesVelocityMagSquaredSum_RU[mKE][sSMKE]);
        totalKE_sum[sSMKE] +=
          allSpeciesM_RU[mKE][sSMKE]/allSpeciesN_RU[mKE][sSMKE]/2.0
          *(allSpeciesVelocityMagSquaredSum_RU[mKE][sSMKE]);
      } else {
        singleSpeciesMeanKE[sSMKE] = 0.0;
      }
    }
  }
  volScalarField::InternalField& itotalMeanKE = totalMeanKE.internalField();
  FOR_ALL(itotalMeanKE, tMKE) {
    if (totalN_sum[tMKE] > 0) {
      itotalMeanKE[tMKE] = totalKE_sum[tMKE]/totalN_sum[tMKE];
    } else {
      itotalMeanKE[tMKE] = 0.0;
    }
  }
}
