# porefoam1f 

 Porefoam1f code solves for incompressible single-phase (1f) flow on 3D images of porous media using OpenFOAM finite-volume library. The code computes and reports flow properties, such as the absolute permeability and formation factor and velocity distributions, of the pore space.

---

### Prerequisites 

 - A GNU Linux operating system (at least for compilation), tested on Ubuntu 18.04
 - GNU make, cmake, and a c++17 compiler (available in most Linux distributions or can be installed through their package manager)
 - MPI message passing interface.   In Ubuntu (18.04) MPI another foamx3m prerequisites can be installed by typing in a terminal:    
      `sudo apt install mpi-default-dev  flex libscotch-dev`    
 - A costomized foam-extend, [foamx3m] (included)     
 - [libtiff] and [zlib] (optional, both dependencies of libvoxel, included)    
 - [script] for convenient compilation (included)    


## Instructions
### Download and compile

This repository is part of a larger group of partly independent packages. Please follow the instruction in README.md in the upper-most directory.  

### Running simulations

Please see the src/doc folder for [installation and usage](doc/porefoam_singlePhase.pdf),  and a sample input header file -- [Image.mhd](src/doc/Image.mhd), which you can you in conjunction with segmented micro-CT image files in raw, raw.gz, tif, and amira (.am) files. For sample micro-CT images and their mhd header files, see [Imperial College pore-scale modelling website].


In summary, you need to download a segmented micro-CT image (with a `image.mhd` header) and run in a bash terminal:
```shell

    source PATH/TO/src/script/bashrc # only once, change PATH/TO according to your porefoam installation path
    
    #cd PATH/TO/IMAGES/
    
    # Set the number of processors (=$nProcX x $nProcY x $nProcZ) used to run the simulation,
    # based on the size of image and number of processors your machine:
    export nProcX=2;  export nProcY=2;  export nProcZ=2  
    
    AllRunImagePar "$(ls *.mhd)"  "X Y Z" # Run the simulations, 
    # This command runs flow simulations on all available mhd files in the current directory, 
    # in all 3 spatial directions, and will take a while.

```

 ### Simulation results
 
 The 3D simulation results are saved in OpenFOAM format, as well as converted into 3D float32 images, both can be visualized using [Paraview], by opening the .foam or the .xmf file in the results folder named OpenMeInParaview.xmf. You can open the latter in a text viewer to see the sizes of the individual velocity files. The Input and output 3D images can also be viewed using [Fiji-is-ImageJ] software.  A summary of the simulation results, including connected porosity, permeability, formation factor and velocity distributions, are saved in a text file named summary_*.txt, alongside other log files.  These log files should be monitored in case of any anomaly in the produced results.  


The velocities that are saved in 3D image files, are face-centred. Faces are either sides of voxels, and cell/voxel-centred values can be obtained by averaging values of consecutive face pairs in the same direction.   If needed, these can be converted to cell/voxel centred values or their magnitudes using a post-processing code called Ufraw2Uc, install the codes and run `Ufraw2Uc -h` for its usage options. 


### Contact and References

To report any problems, contact Ali Q. Raeini, email: a.q.raeini@imperial.ac.uk

For more contacts and references please visit:  
https://www.imperial.ac.uk/earth-science/research/research-groups/pore-scale-modelling  

License:    
[GPLv3](https://www.gnu.org/licenses/gpl-3.0.txt)


[Imperial College pore-scale modelling website]: https://www.imperial.ac.uk/earth-science/research/research-groups/pore-scale-modelling
[Paraview]: https://www.paraview.org/download/    
[libvoxel]: https://github.com/aliraeini/poreFoam-singlePhase/tree/master/src/libvoxel   
[foamx3m]: https://github.com/aliraeini/poreFoam-singlePhase/tree/master/thirdparty/foamx3m     
[libtiff]: https://github.com/aliraeini/poreFoam-singlePhase/tree/master/thirdparty/libtiff    
[zlib]: https://github.com/aliraeini/poreFoam-singlePhase/tree/master/thirdparty/foamx3m    
[script]: https://github.com/aliraeini/poreFoam-singlePhase/tree/master/thirdparty/foamx3m     
[cmd]: https://github.com/aliraeini/pnextract/wiki/Running-command-line-applications
