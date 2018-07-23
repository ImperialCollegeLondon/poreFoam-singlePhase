# poreFoam/singlePhase

 * This branch is experimental, aiming to simplify the build process 
   by including the prerequisites (+ some customization of foam-extend)




### The codes simulate single-phase flow, by solving 
### incompressible Naview-Stokes equations, on 3D images of porous media

---

### Prerequisites: 

- GNU make, cmake, mpi and a c++11 compiler (available in most Linux distributions)
- voxelImage library (included in apps directory)
- A costomized foam-extend (included in thirdparty dyrectory)
- libtiff nad zlib (optional, both included in thirdparty dyrectory)

### Compiling: 
Run, in a terminal in the top-most directory:

`make` 

Once everything tested and working, you can run the following command to delete temporary files:  

`make clean` 
  
The command `make distclean` removes all compiled libraries and executables.

---

### Please see the doc folder for [installation and usage](src/doc/porefoam_singlePhase.pdf),  and a sample input file -- [Image.mhd](src/doc/Image.mhd) -- for microCT images similar to those on [Imperial College pore-scale modelling website](http://www.imperial.ac.uk/earth-science/research/research-groups/perm/research/pore-scale-modelling/micro-ct-images-and-networks/)
