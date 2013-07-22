Beam Study
===

#Introduction
This is the beam study for COMET Phase-I experiment.  

##``Simulate``
Simulation area using comet_g4. Call ``cometg4_gen_sim`` or ``cometg4_gen_simpbs`` to generate jobs.

##``Analyse``
Analysation area
* ``Scan``: Scan out a profile from comet_g4 output. Should use ``MeV, mm`` and ``ns`` for ``g4sim`` input.
  ``comet_g4`` is using ``GeV, cm`` and ``ns``
  * ``gen_jobs.sh``: copy this script into ``result/`` and call this script to generate executable scripts.
    After that, use ``mkexec`` and ``submit`` to batch these jobs.
  * ``transform.sh``: read output of previous jobs and get selected particle profile.
* ``BeamProfile``: Get profiles from ``ts2_0.*.root``. output files will be put to ``Chen/result``
  * ``getprofile.sh``: use this to generate profiles (.pdf and .root) from previous result.
* ``Compare``: Get hisograms from ``Chen/result`` and ``Andy/result`` and compare them. Output files will be put to ``Compare``
  * ``forcompare.sh``: use this to generate a input file for ``Compare``.
* ``cometg4_input``: Deal with comet_g4 output file directly.

##``result``
###Data Set
####``g60cm6mm``: Results for 60cm long 6mm radius Graphite production target.
  * `Andy`: Results from Andy's simulation with 1M initial protons.  
    * [Initial file](http://www.hep.ucl.ac.uk/lfv/comet/data/graphite-proton-target/).  
    * [Ben's analyse](http://www.hep.ph.ic.ac.uk/~bek07/comet_plots/20130702_graphite/).  
    * Geant4 Version: Geant4.9.2  
    * Physics List: ``QGSP_BERT``?  
    * Graphite density:  < 1.82g/cm<sup>3</sup>?  
    * Gun position spread: 2mm  
    * Field Map: ``130117/Phase1Field.dat``  
  * ``Chen``: My results with 15M initial protons.  
    * Geant4 Version: Geant4.9.6.p01  
    * Physics List: ``MyPhysicsList`` in ``comet_g4`` plus a little change in &mu; capture setting:  
      ``/process/muonMinus/captureWeight   1.0``  
      ``/process/muonMinus/decayWeight     1.0``  
      ``/process/muonMinus/conversionRatio 0.0``  
      ``/process/muonMinus/minEnergyDIO    95 MeV``  
      which by default is:  
      ``/process/muonMinus/captureWeight   0.0``  
      ``/process/muonMinus/decayWeight     1.0``  
      ``/process/muonMinus/conversionRatio 1.0``  
      ``/process/muonMinus/minEnergyDIO    103.5 MeV``  
    * Graphite density: 1.82g/cm<sup>3</sup>  
    * Gun position spread: 2mm  
    * Field Map: ``130117/Phase1Field.dat``  
  * ``QGSPBERT``: My results with 1.5M initial protons.  
    * Geant4 Version: Geant4.9.6.p01  
    * Physics List: Standard ``QGSP_BERT``  
    * Graphite density: 1.82g/cm<sup>3</sup>  
    * Gun position spread: 2mm  
    * Field Map: ``130117/Phase1Field.dat``  
  * ``QGSPBERTHP``: My results with 0.25M initial protons.  
    * Geant4 Version: Geant4.9.6.p01  
    * Physics List: Standard ``QGSP_BERT_HP``  
    * Graphite density: 1.82g/cm<sup>3</sup>  
    * Gun position spread: 2mm  
    * Field Map: ``130117/Phase1Field.dat``  
  * `Hayashi`: Results from Hayashi's simulation with 0.1M initial protons.  
    * Geant4 Version: Geant4.9.4  
    * Physics List: ``MyPhysicsList`` in ``comet_g4`` with default settings.  
    * Graphite density: 1.82g/cm<sup>3</sup>  
    * Gun position spread: 2mm  
    * Field Map: ``130117/Phase1Field.dat``  

####``t16cm6mm``: Results for 16cm long 6mm radius Tungsten production target.
  * ``Chen``: My results with 15M initial protons.  
    * Geant4 Version: Geant4.9.6.p01  
    * Physics List: ``MyPhysicsList`` in ``comet_g4`` plus a little change in &mu; capture setting:  
      ``/process/muonMinus/captureWeight   1.0``  
      ``/process/muonMinus/decayWeight     1.0``  
      ``/process/muonMinus/conversionRatio 0.0``  
      ``/process/muonMinus/minEnergyDIO    95 MeV``  
      which by default is:  
      ``/process/muonMinus/captureWeight   0.0``  
      ``/process/muonMinus/decayWeight     1.0``  
      ``/process/muonMinus/conversionRatio 1.0``  
      ``/process/muonMinus/minEnergyDIO    103.5 MeV``
    * Gun position spread: 2mm  
    * Field Map: ``130117/Phase1Field.dat``  
  * `Hayashi`: Results from Hayashi's simulation with 0.1M initial protons.  
    * Geant4 Version: Geant4.9.4  
    * Physics List: ``MyPhysicsList`` in ``comet_g4`` with default settings.
    * Gun position spread: 2mm

###Geometry

###Analyzation
####``g60cm6mm``: 
#####e<sup>-</sup>
Overview: (According to ``Chen``)  
* at `ptacs_shielding`:
  * Most of them are of low energy: with momentum smaller than 2 MeV/c.
  * Tail is approximately exponential.
  * Most of them came from the target region.
  * Direction distribution is almost isotopical and shows no significant realtion with momentum or position
* at `blt0`:
  * Most of them are of low energy: with momentum smaller than 2 MeV/c.
  * Tail ends at arround 125 MeV/c.
  * Most of them came from the target region.
  * Energetic electron tends to have a small r position and small polar angle &theta; of mementum direction.  

Comparison:
* at `ptacs_shielding`:
  * p<sub>a</sub>: ``Andy`` and ``Hayashi`` are similar, having a sharper peak arround 400 keV/c than ``Chen``, ``QGSPBERT`` and ``QGSPBERTHP``.  
    This is probably caused by the difference of &mu; Capture Weight.
    Tail shapes of them are all approximately exponential, while ``Chen`` has a strange hump between 20 MeV/c to 50 MeV/c.
  * origin point: ``Andy`` and ``Hayashi`` are similar, while ``Chen``, ``QGSPBERT`` and ``QGSPBERTHP`` appear to have more electrons generated at the downstream end of the target.  
  

####``t16cm6mm``: 
