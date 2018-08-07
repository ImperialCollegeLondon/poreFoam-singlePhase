# poreFoam/singlePhase


 The codes simulate incompressible single-phase flow, by solving Naview-Stokes equations on 3D images of porous media

 * This branch is aims to simplify the build process by including the prerequisites (+ some customization of foam-extend for saving memory)

---

### Prerequisites 

- GNU make, cmake, mpi and a c++11 compiler (available in most Linux distributions or can be installed through their package manager)
- voxelImage library (included in src directory)
- A costomized foam-extend (included in thirdparty directory)
- libtiff and zlib (optional, both included in thirdparty directory)

### Compiling
Run, in a terminal in the top-most directory:  
`make`  
Once everything tested and working, you can run `make clean` to delete temporary files.    
The command `make distclean` removes all compiled libraries and executables.



### Instructions 

Please see the src/doc folder for [installation and usage](src/doc/porefoam_singlePhase.pdf),  and a sample input file -- [Image.mhd](src/doc/Image.mhd) -- for microCT images similar to those on [Imperial College pore-scale modelling website](http://www.imperial.ac.uk/earth-science/research/research-groups/perm/research/pore-scale-modelling/micro-ct-images-and-networks/)


### Contact and References

For contacts and references please visit:  
http://www.imperial.ac.uk/earth-science/research/research-groups/perm/research/pore-scale-modelling  
or contact Ali Q. Raeini, email: a.qaseminejad-raeini09@imperial.ac.uk

