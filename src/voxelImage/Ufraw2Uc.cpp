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
\*-------------------------------------------------------------------------*/

    #include <fstream>
    #include <iostream>
    #include <vector>

    #include <assert.h>
#include "voxelImage.h"

using namespace std;

int usage()
{
	std::cout<<"Ufraw2Uc  "<<std::endl;
		std::cout
		<<" Converts face centred velocities (Uf*s) to cell centred velocities (Uc*s)\n"
		<<" Writes velocity magnitide if third argument (optional) is Umag or UmagOnly\n\n"
		<<"Usage examples, type:\n"
		<<" #cd PATH/TO/Ufx.*etc first"<< std::endl
		<<" Ufraw2Uc raw  vxlImage.mhd          # generate Uc*s"<< std::endl
		<<" Ufraw2Uc raw  vxlImage.mhd Umag     # write mag(U) as well"<< std::endl
		<<" Ufraw2Uc raw  vxlImage.mhd UmagOnly # write mag(U) only"<< std::endl
		<<" Ufraw2Uc dat  vxlImage.mhd          # write Uc*s in ascii format"<< std::endl;
	return 1;
}

int main(int argc, char** argv)
{

	if(argc<3)		return usage();
	std::string outFormat(argv[1]);
	suffix(outFormat);
	std::string headerName(argv[2]);
	if(headerName.size()<4 || headerName.compare(0,headerName.size(),"vxlImage.mhd") != 0) return usage();
	
	string wUmag;
	if(argc>3)	
	{
		wUmag=string(argv[1]);
		if(wUmag[0]=='U') std::cout<<"Writing Umag"<<wUmag<<endl;
		else	 std::cout<<"Warning third argument can only be wUmag or UmagOnly, ignoring it:"<<wUmag<<endl;
	}else wUmag="ignor";

	voxelImage vimage("vxlImage.mhd");
	if(!vimage.size3()[2]) {std::cout<<"Error: vxlImage.mhd not read"<<std::endl; return 1;}
	int3 n=vimage.sizeu3();
	if(outFormat=="dat") vimage.write("vxlImage.dat");
	vimage.data_.resize(0);

	voxelImageT<float> Umg;
	if (wUmag[0]=='U') Umg.reset(n[0],n[1],n[2],0.0);

	{
		voxelImageT<float> fField(n[0]+1,n[1],n[2],0.0);
		fField.readBin("Ufx"+suffix());

		for (int k = 0; k<int(fField.size3()[2]) ; k++ )
		 for ( int j = 0; j<int(fField.size3()[1]) ; j++ )
		  for ( int i = 0; i<int(fField.size3()[0])-1 ; i++ )
				fField(i,j,k)=0.5*(fField(i,j,k)+fField(i+1,j,k));
		if(wUmag!="UmagOnly")
			fField.writeBin("Uccx"+suffix(), 0,n[0],0,n[1],0,n[2]);
		if(wUmag[0]=='U')
			forAllkji(Umg) Umg(i,j,k) += fField(i,j,k)*fField(i,j,k);
	}
	{
		voxelImageT<float> fField(n[0],n[1]+1,n[2],0.0);
		fField.readBin("Ufy"+suffix());

		for (int k = 0; k<int(fField.size3()[2]) ; k++ )
		 for ( int j = 0; j<int(fField.size3()[1])-1 ; j++ )
		  for ( int i = 0; i<int(fField.size3()[0]) ; i++ )
			fField(i,j,k)=0.5*(fField(i,j,k)+fField(i,j+1,k));
		if(wUmag!="UmagOnly")
			fField.writeBin("Uccy"+suffix(), 0,n[0],0,n[1],0,n[2]);
		if(wUmag[0]=='U')
			forAllkji(Umg) Umg(i,j,k) += fField(i,j,k)*fField(i,j,k);
	}
	{
		voxelImageT<float> fField(n[0],n[1],n[2]+1,0.0);
		fField.readBin("Ufz"+suffix());
		for (int k = 0; k<int(fField.size3()[2])-1 ; k++ )
		 for ( int j = 0; j<int(fField.size3()[1]) ; j++ )
		  for ( int i = 0; i<int(fField.size3()[0]) ; i++ )
			fField(i,j,k)=0.5*(fField(i,j,k)+fField(i,j,k+1));
		if(wUmag!="UmagOnly")
			fField.writeBin("Uccz"+suffix(), 0,n[0],0,n[1],0,n[2]);
		if(wUmag[0]=='U')
			forAllkji(Umg) Umg(i,j,k) += fField(i,j,k)*fField(i,j,k);
	}
	if(wUmag[0]=='U')
	{
		forAlliii(Umg) Umg(iii) = sqrt(Umg(iii));
		Umg.writeBin("Umag"+suffix());
	}

	if(outFormat=="dat") 
	{
		//voxelImageT<float> pField(n[0],n[1],n[2],0.0);
		
		//else pField.readBin("p.tif");
		//pField.writeAscii("p.dat");
	}

	std::cout<< "end" << std::endl;


	return 0;
}


