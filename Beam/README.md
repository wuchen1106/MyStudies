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
    * Graphite density:  1.7g/cm<sup>3</sup>  
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
    * Graphite density: 1.829g/cm<sup>3</sup>  
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
Overview: (According to ``QGSPBERT``)  
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
  * momentum amplitude p<sub>a</sub>: ``Andy`` and ``Hayashi`` are similar, having a sharper peak arround 400 keV/c than ``Chen``, ``QGSPBERT`` and ``QGSPBERTHP``, while ``QGSPBERT`` has more electrons than ``Chen`` and ``QGSPBERTHP``.
    Geant4 version or &mu; Capture Weight?  
    Tail shapes of them are all approximately exponential, while ``Chen`` and ``Hayashi`` have a strange bump between 20 MeV/c to 50 MeV/c.
    This must be caused by the customised physics list in ``comet_g4``.
  * momentum direction &theta;: ``Andy`` has less electrons with &theta; lager than 0.6 rad, ``QGSPBERT`` has more electrons with &theta; between 0.3 rad and 1 rad, while ``QGSPBERTHP``, ``Hayashi`` and ``Chen`` are the same.
  * origin point: ``Chen``, ``QGSPBERT`` and ``QGSPBERTHP`` appear to have more electrons generated at the downstream end of the target than ``Andy``. ``Hayashi`` seems to be a mixture of ``Chen`` and ``Andy``.  
* at `blt0`:
  * momentum amplitude p<sub>a</sub>: ``Hayashi`` is limited by statistics while ``Andy`` is distinctively different from others in the region below 0.3 MeV/c.
    Geant4 version &mu; Capture Weight or Physics list?  
    Tails all end at arround 125 MeV/c, while ``Chen`` and ``Hayashi`` have a strange bump between 20 MeV/c to 50 MeV/c.
    This must be caused by the customised physics list in ``comet_g4``.
  * momentum direction &theta;: ``Andy`` has less electrons with &theta; lager than 0.6 rad, while ``Hayashi`` and ``Chen`` have more electrons in that region than others.
  * origin point: ``Chen``, ``QGSPBERT`` and ``QGSPBERTHP`` appear to have more electrons generated at the downstream end of the target than ``Andy``. ``Hayashi`` seems to be a mixture of ``Chen`` and ``Andy``.  
  
#####&mu;<sup>-</sup>
Overview: (According to ``QGSPBERT``)  
* at `ptacs_shielding`:
  * momentum amplitude peaks at arround 80 MeV/c.
  * Most of them were generated outside of the target region but not far away.
  * Tend to have a large polar angle &theta; of mementum direction (peak at 1.4 rad).
* at `blt0`:
  * momentum amplitude peaks at arround 60 MeV/c.
  * Most of them were generated outside of the target region but not far away.
  * Tend to have a large polar angle &theta; of mementum direction (peak at 1.4 rad).

Comparison:
* at `ptacs_shielding`:
  * momentum amplitude p<sub>a</sub>: ``Andy`` and ``Hayashi`` are similar, having a flat top at the peak.  
  * momentum direction &theta;: Same shape while ``Andy`` has less &mu;s. Probably because of Graphite density difference.  
  * origin point: ``Andy`` has less &mu;s in near target region. Probably because of Graphite density difference.  
* at `blt0`:
  * momentum amplitude p<sub>a</sub>: Same shape while ``Andy`` has less &mu;s. Probably because of Graphite density difference.
  * momentum direction &theta;: Same shape while ``Andy`` has less &mu;s. Probably because of Graphite density difference.  
  * origin point: `Same shape while ``Andy`` has less &mu;s. Probably because of Graphite density difference.

  
#####&pi;<sup>-</sup>
Overview: (According to ``QGSPBERT``)  
* at `ptacs_shielding`:
  * momentum amplitude peaks at arround 140 MeV/c.
  * Most of them were generated inside the target.
  * Direction distribution is almost isotopical.
* at `blt0`:
  * momentum amplitude peaks at arround 90 MeV/c.
  * Most of them were generated inside the target.
  * Tend to have a small polar angle &theta; of mementum direction (peak at 0.4 rad).

Comparison:
* at `ptacs_shielding`:
  * momentum amplitude p<sub>a</sub>: Same shape while ``Andy`` and ``Hayashi`` have less &pi;s. Reason?  
  * momentum direction &theta;: Same shape while ``Andy`` and ``Hayashi`` have less &pi;s. Reason?  
  * origin point: ``Andy`` and ``Hayashi`` have less &pi;s generated at the downstream end of the target. Reason?  
* at `blt0`: (``Hayashi`` and ``QGSPBERTHP`` are limited by statistics)
  * momentum amplitude p<sub>a</sub>: Same shape while ``Andy`` and ``Hayashi`` have less &pi;s. Reason?  
  * momentum direction &theta;: Same shape while ``Andy`` and ``Hayashi`` have less &pi;s. Reason?  
  * origin point: ``Andy`` and ``Hayashi`` have less &pi;s generated at the downstream end of the target. Reason?  

####``t16cm6mm``: 
