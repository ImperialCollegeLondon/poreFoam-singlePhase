# porefoam1f 

 Porefoam1f code can simulate incompressible single-phase (1f) flow on 3D images of porous media using openfoam finite-volume library.
 This repository serves as a git module for my other (mostly private) applications, but you can use it as a standalone package as well.

---

### Prerequisites 

 - A GNU Linux operating system (at least for compilation), tested on Ubuntu 18.04
 - GNU make, cmake, and a c++11 compiler (available in most Linux distributions or can be installed through their package manager)
 - MPI message passing interface.   In Ubuntu (18.04) MPI another foamx3m prerequisites can be installed by typing in a terminal:   
      `sudo apt install mpi-default-dev  flex libscotch`    
 - [libvoxel] library 
 - A costomized foam-extend, [foamx3m]
 - libtiff and zlib (optional, both dependancies of [libvoxel])
 - [script] for convenient compilation


## Instructions
### Download and compile

This repository is part of a larger group of partly independent packages. Please follow the instruction in README.md in the upper-most directory.  
 
### Running simulations

Please see the src/doc folder for [installation and usage](doc/porefoam_singlePhase.pdf),  and a sample input file -- [Image.mhd](src/doc/Image.mhd). For the microCT images and their mhd header files, please see [Imperial College pore-scale modelling website](http://www.imperial.ac.uk/earth-science/research/research-groups/perm/research/pore-scale-modelling/micro-ct-images-and-networks/)


In summary, you need to download a segmented micro-CT image (with a `image.mhd` header) and run in a bash terminal:
```shell

    msApps # only once, to set up path to apps folder, see above
    #cd PATH_TO_image.mhd_DIRECTORY
    # set the number of processors (=$nProcX x $nProcY x $nProcZ) used to run the simulation 
    # based on the size of image and number of processors your machine has:
    export nProcX=2;  export nProcX=2;  export nProcX=2  
    AllRunImagePar image.mhd  "X Y Z"
```
 
### Contact and References

To report any problems, contact Ali Q. Raeini, email: a.qaseminejad-raeini09@imperial.ac.uk

For more contacts and references please visit:  
http://www.imperial.ac.uk/earth-science/research/research-groups/perm/research/pore-scale-modelling  

License:
[GPLv3](https://www.gnu.org/licenses/gpl-3.0.txt)

