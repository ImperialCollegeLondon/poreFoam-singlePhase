# poreFoam/singlePhase

 * This branch is experimental, aiming to simplify the build process 
 * (and for some customization of foam-extend)




### the codes simulate single-phase flow, by solving 
incompressible Naview-Stokes equations on 3D images of porous media

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

---

### Please see the doc folder for [installation and usage](porefoam_singlePhase.pdf),  and a sample input file -- [Berea.mhd](Berea.mhd) -- for the [Berea.raw image](http://www.imperial.ac.uk/earth-science/research/research-groups/perm/research/pore-scale-modelling/micro-ct-images-and-networks/berea-sandstone/)
