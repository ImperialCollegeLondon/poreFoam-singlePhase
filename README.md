# poreFoam/singlePhase

 * This branch is experimental, aiming to simplify the build process 
   by including the prerequisites (+ some customization of foam-extend)




### The codes simulate single-phase flow, by solving incompressible Naview-Stokes equations, on 3D images of porous media


### Please see the doc folder for [installation and usage](src/doc/porefoam_singlePhase.pdf),  and a sample input file -- [Image.mhd](src/doc/Image.mhd) -- for microCT images similar to those on [Imperial College pore-scale modelling website](http://www.imperial.ac.uk/earth-science/research/research-groups/perm/research/pore-scale-modelling/micro-ct-images-and-networks/)

---

### Prerequisites: 

- GNU make, cmake, mpi and a c++11 compiler (available in most Linux distributions)
- voxelImage library (included in apps directory)
- A costomized foam-extend (included in thirdparty directory)
- libtiff nad zlib (optional, both included in thirdparty directory)

### Compiling: 
Run, in a terminal in the top-most directory:     
`make`  
* Note: edit and remove the `-j` from the top-most level Makefile if your computer has less than 30GB RAM!

Once everything tested and working, you can run `make clean` to delete temporary files.    
The command `make distclean` removes all compiled libraries and executables. 


### Contact and References

For contacts and references please visit: 

http://www.imperial.ac.uk/earth-science/research/research-groups/perm/research/pore-scale-modelling

or contact Ali Q. Raeini, email: a.qaseminejad-raeini09@imperial.ac.uk

