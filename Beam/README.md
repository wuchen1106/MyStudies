Beam Study
===

###Introduction
This is the beam study for COMET Phase-I experiment.  

###Stucture

#####``Simulate``
Simulation area using comet_g4. Call ``cometg4_gen_sim`` or ``cometg4_gen_simpbs`` to generate jobs.

#####``Analyse``
Analysation area
* ``Scan``: Scan out a profile from comet_g4 output. Should use ``MeV, mm`` and ``ns`` for ``g4sim`` input.
  ``comet_g4`` is using ``GeV, cm`` and ``ns``
* ``cometg4_input`` Deal with comet_g4 output file directly.
* ``BeamProfile`` Get profiles from ``ts2_0.*.root``. output files will be put to ``Chen/result``
* ``Compare`` Get hisograms from ``Chen/result`` and ``Andy/result`` and compare them. Output files will be put to ``Compare``

#####``result``
Some results
* ``g60cm6mm``: Results for 60cm long 6mm radius Graphite production target.
  * `Andy`: Results from Andy's simulation with 1M initial protons. 
    [Initial file](http://www.hep.ucl.ac.uk/lfv/comet/data/graphite-proton-target/).
    [Ben's analyse](http://www.hep.ph.ic.ac.uk/~bek07/comet_plots/20130702_graphite/).
  * ``Chen``: My results with 15M initial protons.
    * ``result``: results from ``BeamProfile``
    * ``ts2_0.*.txt.bk``: profiles scaned from ``Scan``
    * ``ts2_0.*.txt``: modified profiles for ``BeamProfile``
    * ``ts2_0.*.root``: root files generated from ``ts2_0.*.txt`` using ``txt2root``
  * ``Compare``: Results from ``Analyse/Compare``
