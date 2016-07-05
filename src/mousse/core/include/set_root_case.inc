// set_root_case.inc
  mousse::argList args(argc, argv);
  if (!args.checkRootCase())
  {
    mousse::FatalError.exit();
  }
