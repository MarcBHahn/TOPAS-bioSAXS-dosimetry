# TOPAS-bioSAXS-dosimetry
1) Description:

Models for TOPAS/Geant4 to estimate the microscopic dose received by biomolecules during bioSAXS experiments.

The C++ classes in this repository extend the functionality of the TOPAS (http://www.topasmc.org/) Monte-Carlo program, which is itself a wrapper of the Geant4 MCS Toolkit (http://geant4.org).


2) Installation:

Installation:

Navigate to the TOPAS extension directory:

  cd ~/topas_extensions/

Clone or download the sourcecode into your TOPAS extension directory:
 
  git clone https://github.com/MarcBHahn/TOPAS-bioSAXS-dosimetry.git
 
Change to your topas directory:

  cd ~/Topas/

Install it:

  cmake ./ -DTOPAS_EXTENSIONS_DIR=~/topas_extensions/TOPAS-bioSAXS-dosimetry &&  make -j4


3) Description:

A simple model to determine microscopic doses in the target volume of biomolecules received during bioSAXS experiments. [1]
Details can be found in 

4) Usage:

Examples can be found in the  "examples/" directory.


 
5) Bugs:

Please report bugs to hahn@physik.fu-berlin.de or on https://github.com/MarcBHahn/TOPAS-bioSAXS-dosimetry/


6) Literature:

If you use this extension please cite the following literature:

[1] Hahn et al. Phys. Rev. E 95, 052419 
https://journals.aps.org/pre/abstract/10.1103/PhysRevE.95.052419



7) Etc:

Tags: topas topasmc topasmcs topas-mc topas-mcs topas-nbio mcs geant4 geant4-dna
