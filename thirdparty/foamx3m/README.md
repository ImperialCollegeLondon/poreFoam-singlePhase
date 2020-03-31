###  Foam-miniext-3.4 
This is a minified and intermediate version of foam-extend code derived
from foam-extend-3.1 and 4.0, with minor additions for some compatibility 
with OpenFOAM-1712, prepared by Ali Raeini for use in poreFoam codes.

* See the file Changelog.current for the list of changes.

### Licence: 
See the file COPYING for the GNU-GPLv3 copyright notice.
See also the files ExtendProjectPreamble and ListOfContributors.
The provided code is not endorsed by foam-extend developers, nor by OpenFOAM. 

### Compilling and installation
Run ./Allwmake in the top level directory after running "source ./etc/bashrc".
Once everything compiled run ./Allclean to delete intermediate files. 
An optional install script, Allinstall is also provided, but it should
be edited before running, manually setting and creating the installation paths.
The compiled libraries will have different names compared to that of OpenFOAM, 
or foam-extend, so they can be installed simultanuously, but only after all 
compilations are finished.

