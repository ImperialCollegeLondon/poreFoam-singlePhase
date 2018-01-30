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
//using namespace std;

int usage()
{
	std::cout<<"\nvoxelImageConvert:\n utility to read a 3D image\n run some commands on it (cropD, threshold, ...),"
	<<"\n and write it back to disk,\n in the same or in a different format\n .dat suffix is used for ascii files and .raw implies binary format"
		<<"\nusages: \n"
		<<"    voxelImageConvert inputMetaImage.mhd outPutFile \n"
		<<"    voxelImageConvert inputMetaImage.mhd outPutFile.mhd UChar\n"
		<<"    voxelImageConvert inputMetaImage.mhd outPutFile.dat \n"
		<<"    voxelImageConvert inputMetaImage.mhd outPutFile.raw \n"
		<<"    voxelImageConvert inputMetaImage.mhd NO_WRITE \n"
		<< std::endl;
	return -1;
}


int main(int argc, char *argv[])
{


	if(argc<3) return usage();


	std::string headerName(argv[1]);
	if(headerName.size()<4) return usage();
	std::string outputName(argv[2]);
	std::string outputFormat(argc>3 ?  argv[3] : "XXX" );

	std::cout<<"voxelImageConvert "<<headerName<<"  "<<outputName<<std::endl;

	std::unique_ptr<voxelImageTBase> vxlImage = readImage(headerName);

	if(outputFormat=="UChar")
	{
		voxelImage vImgUChar(vxlImage->sizeu3(), vxlImage->dx(), vxlImage->X0(), 0);
		forAllkji(vImgUChar)			vImgUChar(i,j,k) = vxlImage->getInt(i,j,k);
		vImgUChar.write(outputName);
	}
	else
		vxlImage->write(outputName);


	std::cout<< "end" << std::endl;

	vxlImage->printInfo();


	return 0;
}


