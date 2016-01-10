//
// setRootCase.H
// ~~~~~~~~~~~~~
  mousse::argList args(argc, argv);
  if (!args.checkRootCase())
  {
    mousse::FatalError.exit();
  }
