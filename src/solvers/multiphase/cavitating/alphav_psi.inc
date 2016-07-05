{
  alphav =
    max
    (
      min
      (
        (rho - rholSat)/(rhovSat - rholSat),
        scalar(1)
      ),
      scalar(0)
    );
  alphal = 1.0 - alphav;
  Info << "max-min alphav: " << max(alphav).value() << " "
    << min(alphav).value() << endl;
  psiModel->correct();
}
