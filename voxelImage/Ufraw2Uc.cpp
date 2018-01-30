/*-------------------------------------------------------------------------*\	
You can redistribute this code and/or modify this code under the 
terms of the GNU General Public License (GPL) as published by the  
Free Software Foundation, either version 3 of the License, or (at 
your option) any later version. see <http://www.gnu.org/licenses/>.


The code has been developed by Ali Qaseminejad Raeini as a part his PhD 
at Imperial College London, under the supervision of Branko Bijeljic 
and Martin Blunt. 
* 
Please see our website for relavant literature:
http://www3.imperial.ac.uk/earthscienceandengineering/research/perm/porescalemodelling

For further information please contact us by email:
Ali Q Raeini:    a.qaseminejad-raeini09@imperial.ac.uk
Branko Bijeljic: b.bijeljic@imperial.ac.uk
Martin J Blunt:  m.blunt@imperial.ac.uk
\*-------------------------------------------------------------------------*/

    #include <fstream>
    #include <iostream>
    #include <vector>

    #include <assert.h>

#include "voxelImage.h"
int usage()
{
	std::cout<<"Ufraw2Uc  "<<std::endl;
		std::cout
		<<" converts Uf*** (face centred velocities) to cell centred velocities (Uc***)"
		<<"To continue, type:  \n"
		<<" Ufraw2Uc  raw  vxlImage.mhd"<< std::endl
		<<"or (for ascii output format):  \n"
		<<" Ufraw2Uc  dat  vxlImage.mhd"<< std::endl;
	return 1;
}

int main(int argc, char** argv)
{

	if(argc!=3)		return usage();
	std::string outFormat(argv[1]);
	std::string headerName(argv[2]);
	if(headerName.size()<4 || headerName.compare(0,headerName.size(),"vxlImage.mhd") != 0) return usage();

	voxelImage vimage("vxlImage.mhd");
	if(!vimage.size3()[2]) {std::cout<<"Error: vxlImage.mhd not read"<<std::endl; return 1;}
	int3 n=vimage.sizeu3();
	if(outFormat=="dat") vimage.write("vxlImage.dat");
	vimage.data_.resize(0);

	{
		voxelImageT<float> fField(n[0]+1,n[1],n[2],0.0);
		fField.readBin("Ufx.raw");

		forAllkji(fField)
			fField(i,j,k)=0.5*(fField(i,j,k)+fField(i+1,j,k));
		
		if(outFormat=="dat") fField.writeAscii("Uccx.dat", 0,n[0],0,n[1],0,n[2]);
		else                 fField.writeBin("Uccx.raw", 0,n[0],0,n[1],0,n[2]);
	}
	{
		voxelImageT<float> fField(n[0],n[1]+1,n[2],0.0);
		fField.readBin("Ufy.raw");

		for (int k = 0; k<int(fField.size3()[2]) ; k++ )
		 for ( int j = 0; j<int(fField.size3()[1])-1 ; j++ )
		  for ( int i = 0; i<int(fField.size3()[0]) ; i++ )
			fField(i,j,k)=0.5*(fField(i,j,k)+fField(i,j+1,k));
		if(outFormat=="dat") fField.writeAscii("Uccy.dat", 0,n[0],0,n[1],0,n[2]);
		else                 fField.writeBin("Uccy.raw", 0,n[0],0,n[1],0,n[2]);
	}
	{
		voxelImageT<float> fField(n[0],n[1],n[2]+1,0.0);
		fField.readBin("Ufz.raw");
		for (int k = 0; k<int(fField.size3()[2])-1 ; k++ )
		 for ( int j = 0; j<int(fField.size3()[1]) ; j++ )
		  for ( int i = 0; i<int(fField.size3()[0]) ; i++ )
			fField(i,j,k)=0.5*(fField(i,j,k)+fField(i,j,k+1));
		if(outFormat=="dat") fField.writeAscii("Uccz.dat", 0,n[0],0,n[1],0,n[2]);
		else                 fField.writeBin("Uccz.raw", 0,n[0],0,n[1],0,n[2]);
	}
	if(outFormat=="dat") 
	{
		voxelImageT<float> pField(n[0],n[1],n[2],0.0);
		pField.readBin("p.raw");
		pField.writeAscii("p.dat");
	}

	std::cout<< "end" << std::endl;


	return 0;
}


