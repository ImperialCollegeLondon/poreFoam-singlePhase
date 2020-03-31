/*-------------------------------------------------------------------------*\

You can redistribute this code and/or modify this code under the 
terms of the GNU General Public License (GPL) as published by the  
Free Software Foundation, either version 3 of the License, or (at 
your option) any later version. see <http://www.gnu.org/licenses/>.

Please see our website for relavant literature making use of this code:
http://www3.imperial.ac.uk/earthscienceandengineering/research/perm/porescalemodelling

For further information please contact us by email:
Ali Q Raeini: a.qaseminejad-raeini09@imperial.ac.uk

\*-------------------------------------------------------------------------*/


//!\brief Converts 3D Image files into openfoam format for computing flow fields. 

#include <sys/stat.h>

#include <fstream>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <array>
#include <valarray>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;


#include "voxelImage.h"
using namespace std;
#include "voxelRegions.h"

int usage()
{
	cout<<"converts micro-CT images to OpenFOAM serial or simple parallel meshes"<<endl;
	cout<<"usage: example:"<<endl;
	cout<<"    voxelToFoamPar imageName.mhd 1 1 1 resetX0 "<<endl;
	cout<<"    voxelToFoamPar imageName.mhd 3 2 2 resetX0 "<<endl;
	return 1;
}


void toFoam(voxelImage& vxlImg, int nVVs, const voxelField<int>& procIsijk, int iProc, int jProc, int kProc);

int main(int argc, char** argv)
{  //!- reads image, adds a boundary layers around it
	int irg = 0;
	if(argc<5)		return usage();
	std::string headerName(argv[++irg]);
	if(headerName.size()<4 || ( headerName.compare(headerName.size()-4,4,".mhd") != 0 && headerName.compare(headerName.size()-4,4,".tif") != 0) )
		return usage();
	int3 nPar(1,1,1);
	nPar[0] = atoi(argv[++irg]);
	nPar[1] = atoi(argv[++irg]);
	nPar[2] = atoi(argv[++irg]);
	char resetX0 = argc>1+irg ? argv[++irg][0] : 'F';
	//char unit = argc>1+irg ? argv[++irg][0] : 'u';
	if (nPar[2]*nPar[1]*nPar[0]<1) {cout<<"\nError: nPar[2]*nPar[1]*nPar[0]<1\n"; return usage();}
	cout<<"voxelToFoamPar "<<headerName<<"  "<<nPar[0]<<" "<<nPar[0]<<" "<<nPar[0]<<endl;

	voxelImage vimage;  readConvertFromHeader(vimage, headerName);
	int3 n = vimage.size3();


	vimage.printInfo();

	vimage.writeHeader("vxlImage.mhd");

	cout <<"finding connected parts of the image "<<endl;
	vimage.threshold101(0,0);
	//vimage.growBox(1);
	//vimage.FaceMedian06(2,4);
	//vimage.FaceMedian06(2,4);
	//vimage.FaceMedian06(2,4);
	//vimage.FaceMedian06(2,4);
	//vimage.cropD(int3(1,1,1),int3(n[0]+1,n[1]+1,n[2]+1),1,1);	//		 XXXXXXXXXXXXXXXXXXXXXXXXXXXX
	vimage.cropD(int3(0,0,0),n,1,1);	//		 XXXXXXXXXXXXXXXXXXXXXXXXXXXX

	keepLargest0(vimage); //! CtrlF:isolated=254

	vimage.printInfo();
	cout <<"Converting to OpenFOAM format "<<endl;

	if(resetX0=='T' || resetX0=='t')		vimage.X0Ch()=-vimage.dx();


	//! solid phase should have the highest vv TODO: add multilabel
	int nVVs=0;   forAllvv_seq(vimage) if(vv<240) nVVs =max(nVVs,int(vv));
	cout<<" maxVxlvalue:"<<nVVs<<endl;
	forAllvp_(vimage) if(*vp>nVVs) *vp=nVVs; //! set isolateds to last vv, CtrlF:isolated=254
	++nVVs;
	cout<<"nVVs:"<<int(nVVs)<<endl;


const int
	Left  =nVVs+0,
	Right =nVVs+1,
	Bottom=nVVs+2,
	Top   =nVVs+3,
	Back  =nVVs+4,
	Front =nVVs+5;
	vimage.setSlice('i',0,     0+1*Left  );
	vimage.setSlice('i',n[0]+1,0+1*Right );
	vimage.setSlice('j',0,     0+1*Bottom);
	vimage.setSlice('j',n[1]+1,0+1*Top   );
	vimage.setSlice('k',0,     0+1*Back  );
	vimage.setSlice('k',n[2]+1,0+1*Front );


	voxelField<int> procIsijk(nPar[0]+2,nPar[1]+2,nPar[2]+2,-1);
	int iProc=-1;
	if (nPar[2]*nPar[1]*nPar[0]==1)
		toFoam(vimage,nVVs, procIsijk, 1, 1, 1);
	else if (nPar[2]*nPar[1]*nPar[0]>1)
	{
		voxelField<voxelImage>  vimages(nPar[0],nPar[1],nPar[2],voxelImage());

		vector<int> iBs(nPar[0]+1,n[0]);
		vector<int> jBs(nPar[1]+1,n[1]);
		vector<int> kBs(nPar[2]+1,n[2]);
		for (int iz=0;iz<nPar[2];iz++)	kBs[iz]=int(n[2]/nPar[2])*iz;
		for (int iy=0;iy<nPar[1];iy++)	jBs[iy]=int(n[1]/nPar[1])*iy;
		for (int ix=0;ix<nPar[0];ix++)	iBs[ix]=int(n[0]/nPar[0])*ix;


		cout<<"iBs: "<<*iBs.begin()<<" ... "<<*iBs.rbegin()<<endl;
		cout<<"jBs: "<<*jBs.begin()<<" ... "<<*jBs.rbegin()<<endl;
		cout<<"kBs: "<<*kBs.begin()<<" ... "<<*kBs.rbegin()<<endl;

			OMPragma("omp parallel for")
			for (int ixyz=0;ixyz<nPar[0]*nPar[1]*nPar[2];ixyz++)
			{
				int ix= ixyz%nPar[0];
				int iz= ixyz/(nPar[0]*nPar[1]);
				int iy=(ixyz/nPar[0])%nPar[1];
				vimages(ix,iy,iz).reset(iBs[ix+1]-iBs[ix]+2, jBs[iy+1]-jBs[iy]+2, kBs[iz+1]-kBs[iz]+2,0);
				vimages(ix,iy,iz).setFrom(vimage, iBs[ix], jBs[iy], kBs[iz]);
				if(vimages(ix,iy,iz).volFraction(0,0)>1.0e-12)		procIsijk(ix+1,iy+1,iz+1)=++iProc;
				cout<<"poro_"<<ix<<iy<<iz<<":"<<vimages(ix,iy,iz).volFraction(0,0)<<"  iBx"<<iBs[ix+1]-iBs[ix]+2<<endl;
			};

			cout<<"\n************** generating meshes ************"<<endl;
		vimage.reset(0,0,0,0);
			OMPragma("omp parallel for")
			for (int ixyz=0;ixyz<nPar[0]*nPar[1]*nPar[2];ixyz++)
			{
			 int ix= ixyz%nPar[0];
			 int iz= ixyz/(nPar[0]*nPar[1]);
			 int iy=(ixyz/nPar[0])%nPar[1];
			 if(procIsijk(ix+1,iy+1,iz+1)>=0)
			 {
				cout<<"************* processor: "<<ix<<" "<<iy<<" "<<iz<<", Phi="<<100*vimages(ix,iy,iz).volFraction(0,0)<<" *************"<<endl;
				toFoam(vimages(ix,iy,iz),nVVs, procIsijk, ix+1, iy+1, iz+1);
				cout<<endl;
			 }
			 }
	} else cout<<"!!! npx x npy x npz = "<<nPar[2]*nPar[1]*nPar[0]<<endl;
	cout<<":/"<<endl;
   return 0;
}

void toFoam(voxelImage& vxlImg, int nVVs, const voxelField<int>& procIsijk, int iProc, int jProc, int kProc)
{ //!- generate an openfaom (processor) mesh similar to voxelToFoamPar.cpp
	int myprocI=procIsijk(iProc,jProc,kProc);
	int3 n=vxlImg.size3();n[0]-=2;n[1]-=2;n[2]-=2;
	dbl3 X0=vxlImg.X0(); 
	dbl3 dx=vxlImg.dx();
	X0+=dx;



	string Folder = myprocI>=0 ?  "processor"+toStr(myprocI)  :  ".";
	cout<<"procIs, size: "<<procIsijk.size3()<<" "<<",  ijk: "<<iProc<<"  "<<jProc<<"  "<<kProc<<endl;
	cout<<Folder<<"  N: "<<n[0] <<" "<<n[1] <<" "<< n[2]<<endl;
	::mkdir(Folder.c_str(),0777);
	::mkdir((Folder+"/constant").c_str(),0777);
	Folder=Folder+"/constant/polyMesh";
	::mkdir((Folder).c_str(),0777);


//=======================================================================

	voxelField<int> point_mapper(n[0]+1,n[1]+1,n[2]+1,-1);




{

	register int iPoints=-1;
	forAllkji_1(vxlImg)
		if (!vxlImg(i,j,k))
		{
				int* 
				dd=&point_mapper(i-1,j-1,k-1);
				      if (*dd<0) *dd=++iPoints;
				++dd; if (*dd<0) *dd=++iPoints;
				dd+=n[0];
						if (*dd<0) *dd=++iPoints;
				++dd; if (*dd<0) *dd=++iPoints;

				dd=&point_mapper(i-1,j-1,k);
				      if (*dd<0) *dd=++iPoints;
				++dd; if (*dd<0) *dd=++iPoints;
				dd+=n[0];
						if (*dd<0) *dd=++iPoints;
				++dd; if (*dd<0) *dd=++iPoints;
		}

	//for (int iz=0;iz<vxlImg.size3()[2]-1;++iz)
		//for (int iy=0;iy<vxlImg.size3()[1]-1;++iy)
			//for (int ix=0;ix<vxlImg.size3()[0]-1;++ix)
			//{
				//unsigned char* dd=&vxlImg(ix,iy,iz);
				//if (*dd!=0 && *(dd+1)!=0)
				//{ dd+=vxlImg.size3()[0];
					//if(*dd!=0 && *(dd+1)!=0)
					//{	dd=&vxlImg(ix,iy,iz+1);
						//if(*dd!=0 && *(dd+1)!=0)
						//{	dd+=vxlImg.size3()[0];
							//if(*dd!=0 && *(dd+1)!=0) continue;
						//}
					//}
				//}
				//++iPoints;
			//}


	cout<<"nPoints: "<<iPoints+1<<"\nwriting points";cout.flush();


	ofstream pointsf((Folder+"/points").c_str());
	assert(pointsf);


	pointsf<<
	"FoamFile\n"
	"{\n"
	"	version	 2.0;\n"
	"	format	  ascii;\n"
	"	class	   vectorField;\n"
	"	location	\""<<Folder+"\";\n"
	"	object	  points;\n"
	"}\n\n";

	pointsf<<iPoints+1<<endl<<"("<<endl;
	pointsf.precision(8);;
	iPoints=-1;
	for (int iz=0;iz<point_mapper.size3()[2];++iz)
	{	double z=iz*dx[2]+X0[2];
		for (int iy=0;iy<point_mapper.size3()[1];iy++)
		{	double y=iy*dx[1]+X0[1];
			for (int ix=0;ix<point_mapper.size3()[0];ix++)
			{
				if(point_mapper(ix,iy,iz)>=0)
				{	point_mapper(ix,iy,iz)=++iPoints;//. sort point_mapper
					double x=ix*dx[0]+X0[0];
					pointsf<< "("<<x<< ' '<<y<<' '<<z<<")\n";
				}
			}
		}
	}
	//for (int iz=0;iz<vxlImg.size3()[2]-1;++iz)
	//{  double z=iz*dx[2]+X0[2];
		//for (int iy=0;iy<vxlImg.size3()[1]-1;iy++)
		//{  double y=iy*dx[1]+X0[1];
			//for (int ix=0;ix<vxlImg.size3()[0]-1;ix++)
			//{
				//unsigned char* dd=&vxlImg(ix,iy,iz);
				//if (*dd!=0 && *(dd+1)!=0)
				//{ dd+=vxlImg.size3()[0];
					//if(*dd!=0 && *(dd+1)!=0)
					//{	dd=&vxlImg(ix,iy,iz+1);
						//if(*dd!=0 && *(dd+1)!=0)
						//{	dd+=vxlImg.size3()[0];
							//if(*dd!=0 && *(dd+1)!=0) continue;
						//}
					//}
				//}
				//pointsf<< "("<<ix*dx[0]+X0[0]<< ' '<<y<<' '<<z<<")\n";
				//point_mapper(ix,iy,iz)=++iPoints;
			//}
		//}
	//}

	pointsf<<endl<<")"<<endl;

	pointsf.close();
}
	cout <<" :/"<<endl;
//=======================================================================



//______________________________________________________________________________________________________________________



	size_t nCells=0;

const int

	Internal     = 0, 
	Grainwalls   = 1, 
	Left  =nVVs+0,
	Right =nVVs+1,
	Bottom=nVVs+2,
	Top   =nVVs+3,
	Back  =nVVs+4,
	Front =nVVs+5;

	std::array<std::string,255> B_nams;
	B_nams[Internal]="Internal";
	B_nams[Grainwalls]="Grainwalls";
	int nBoundaries=5+nVVs;
	cout<<"ib:Nvvs: ";
	for(int ib=2;ib<nVVs;++ib)  { (cout<<ib<<" ").flush();  B_nams[ib]="VV"+toStr(ib)+"B"; }
	B_nams[Left]="Left";
	B_nams[Right]="Right";
	B_nams[Bottom]="Bottom";
	B_nams[Top]="Top";
	B_nams[Back]="Back";
	B_nams[Front]="Front";

	std::array<size_t,255> nFaces; nFaces.fill(0);


	for (int iz=1;iz<=n[2];++iz)
	 for (int iy=1;iy<=n[1];++iy)
	  for (int ix=1;ix<=n[0];++ix)
	  {
			if (!vxlImg(ix,iy,iz))
			{
				nCells++;

				unsigned char 
				neiv=vxlImg(ix-1,iy,iz);
				if (ix!=1)
				{
				  if (neiv)     ++nFaces[neiv];
				  //else         ++nFaces[Internal];
				}else if (neiv) ++nFaces[neiv];
				else            ++nFaces[Left];

				neiv=vxlImg(ix+1,iy,iz);
				if (ix!=n[0])
				{
				  if (neiv)     ++nFaces[neiv];
				  else          ++nFaces[Internal];
				}else if (neiv) ++nFaces[neiv];
				else            ++nFaces[Right];

				neiv=vxlImg(ix,iy-1,iz);
				if (iy!=1)
				{
				  if (neiv)     ++nFaces[neiv];
				  //else        ++nFaces[Internal];
				}else if (neiv) ++nFaces[neiv];
				else            ++nFaces[Bottom];

				neiv=vxlImg(ix,iy+1,iz);
				if (iy!=n[1])
				{
				  if (neiv)     ++nFaces[neiv];
				  else          ++nFaces[Internal];
				}else if (neiv) ++nFaces[neiv];
				else            ++nFaces[Top];


				neiv=vxlImg(ix,iy,iz-1);
				if (iz!=1)
				{
				  if (neiv)     ++nFaces[neiv];
				  //else        ++nFaces[Internal];
				}else if (neiv) ++nFaces[neiv];
				else            ++nFaces[Back];

				neiv=vxlImg(ix,iy,iz+1);
				if (iz!=n[2])
				{
				  if (neiv)     ++nFaces[neiv];
				  else          ++nFaces[Internal];
				}else if (neiv) ++nFaces[neiv];
				else            ++nFaces[Front];
		}
	  }

	(cout<<",  nCells: "<<nCells<<",    B:nFaces: ").flush();
	for(int ib=0;ib<255;++ib)  if(nFaces[ib])  cout<< " "<<ib<<":"<<nFaces[ib]<<", ";
	cout<<endl;

	nBoundaries+=	int(procIsijk(iProc-1,jProc,kProc)>=0 && nFaces[Left])+ 
						int(procIsijk(iProc+1,jProc,kProc)>=0 && nFaces[Right])+
						int(procIsijk(iProc,jProc-1,kProc)>=0 && nFaces[Bottom])+ 
						int(procIsijk(iProc,jProc+1,kProc)>=0 && nFaces[Top])+
						int(procIsijk(iProc,jProc,kProc-1)>=0 && nFaces[Back])+ 
						int(procIsijk(iProc,jProc,kProc+1)>=0 && nFaces[Front]);
						

	array<size_t,255> iStartFaces; iStartFaces.fill(0);





	{	ofstream boundary((Folder+"/boundary").c_str());
		assert(boundary);
		boundary<<
		"FoamFile\n"
		"{\n"
		"	version	 2.0;\n"
		"	format	  ascii;\n"
		"	class	   polyBoundaryMesh;\n"
		"	location	\""<<Folder+"\";\n"
		"	object	  boundary;\n"
		"}\n\n";


		#define write_boundary(nwrite, name,ptype)          {          \
			iStartFaces[name] = iLastFace;										\
			boundary<<		                                                \
			"	"<<  B_nams[name]							 <<endl<<			   \
			"	{"												 <<endl<<		         \
			"		type			"<<ptype<<";"				  <<endl<<			   \
			"		nFaces		  "<<(nwrite)* nFaces[name]<<';'<<endl<<	\
			"		startFace	   "<<iLastFace<<';'   <<endl<<  			 \
			"	}"												 <<endl; 					\
			iLastFace += (nwrite)* nFaces[name];			}

		#define write_procBoundary(name,myId,neiId)     {              \
			iStartFaces[name] = iLastFace;										\
			if (nFaces[name] >0)                                       \
			boundary<<		                                                \
			"	"<<  "processor"+	toStr(myId)+"to"+toStr(neiId)	<<endl<<	   \
			"	{"												 <<endl<<		         \
			"		type			processor;"				  <<endl<<			      \
			"		inGroups        1(processor);"	  <<endl<<			      \
			"		nFaces		  "<<nFaces[name]<<';'	   <<endl<<	      \
			"		startFace	   "<<iLastFace<<';'   <<endl<<   \
			"		matchTolerance  0.0001;"	  <<endl<<			            \
			"		transform       unknown;"	  <<endl<<			            \
			"		myProcNo        "<<myId<<";"	  <<endl<<			         \
			"		neighbProcNo    "<<neiId<<";"	  <<endl<<			         \
			"	}"												 <<endl;						\
			iLastFace += nFaces[name];					  }
        
        
		boundary<< nBoundaries <<endl	<<'('<<endl;






		int iLastFace = nFaces[Internal];

		//write_boundary(true, Grainwalls,"patch");
		for(int ib=1;ib<nVVs;++ib)  write_boundary(1, ib,"patch");

		write_boundary(procIsijk(iProc-1,jProc,kProc)<0, Left,"patch");
		write_boundary(procIsijk(iProc+1,jProc,kProc)<0, Right,"patch");
		write_boundary(procIsijk(iProc,jProc-1,kProc)<0, Bottom,"patch");
		write_boundary(procIsijk(iProc,jProc+1,kProc)<0, Top,"patch");
	 #ifdef _2D_
		write_boundary(true, Back,"empty");
		write_boundary(true, Front,"empty");
	 #else
		write_boundary(procIsijk(iProc,jProc,kProc-1)<0, Back,"patch");
		write_boundary(procIsijk(iProc,jProc,kProc+1)<0, Front,"patch");
	 #endif


		if (procIsijk(iProc-1,jProc,kProc)>=0) write_procBoundary(Left,myprocI, procIsijk(iProc-1,jProc,kProc));
		if (procIsijk(iProc+1,jProc,kProc)>=0) write_procBoundary(Right,myprocI, procIsijk(iProc+1,jProc,kProc));
		if (procIsijk(iProc,jProc-1,kProc)>=0) write_procBoundary(Bottom,myprocI, procIsijk(iProc,jProc-1,kProc));
		if (procIsijk(iProc,jProc+1,kProc)>=0) write_procBoundary(Top,myprocI, procIsijk(iProc,jProc+1,kProc));
		if (procIsijk(iProc,jProc,kProc-1)>=0) write_procBoundary(Back,myprocI, procIsijk(iProc,jProc,kProc-1));
		if (procIsijk(iProc,jProc,kProc+1)>=0) write_procBoundary(Front,myprocI, procIsijk(iProc,jProc,kProc+1));

		boundary<<")"   <<endl;
		boundary.close();
	}







	cout<<"creating faces"<<endl;


	array<std::vector<array<int,6> >,255> faces_bs;
	size_t sumnFaces=0;
	for(int ib=0;ib<255;++ib)  if(nFaces[ib])
	{
		sumnFaces+=nFaces[ib];
		faces_bs[ib].resize(nFaces[ib]);
		fill(faces_bs[ib].begin(),faces_bs[ib].end(), array<int,6>{{-1,-1,-1,-1,-1,-1}});
	}

	voxelField<int3> ownerMapper(n[0]+1,n[1]+1,n[2]+1,int3(-1,-1,-1));


	cout<<"collecting faces"<<endl;


#define recordF_m( l10,l11,l20,l21,l30,l31,dir,ii,jj,kk,type )				   \
  {																	 \
	if (ownerMapper(ii,jj,kk)[dir]<0)										 \
	{		   ++iFaces[type] ;																		 \
			ownerMapper(ii,jj,kk)[dir]=iFaces[type] + iStartFaces[type];	              \
			faces_bs[type][iFaces[type]][4]=iCells;	/* cell number (for owners) */         \
			faces_bs[type][iFaces[type]][0]=point_mapper(ii,jj,kk);								  \
			faces_bs[type][iFaces[type]][1]=point_mapper(ii+l11,jj+l21,kk+l31);					\
			faces_bs[type][iFaces[type]][2]=point_mapper(ii+l10+l11,jj+l20+l21,kk+l30+l31);  \
			faces_bs[type][iFaces[type]][3]=point_mapper(ii+l10,jj+l20,kk+l30);					\
	}																			   \
	else																			\
	{																			   \
			faces_bs[type][ownerMapper(ii,jj,kk)[dir]][5]=iCells; /* cell number (for neighbours) */ \
	}																			   \
  }


#define  kclockwiserecordF(type) recordF_m( 1,0,0,1,0,0, 2, ix-1,iy-1,iz-1, type)
#define kuclockwiserecordF(type) recordF_m( 0,1,1,0,0,0, 2, ix-1,iy-1,iz  , type)
#define  jclockwiserecordF(type) recordF_m( 0,1,0,0,1,0, 1, ix-1,iy-1,iz-1, type)
#define juclockwiserecordF(type) recordF_m( 1,0,0,0,0,1, 1, ix-1,iy  ,iz-1, type)
#define  iclockwiserecordF(type) recordF_m( 0,0,1,0,0,1, 0, ix-1,iy-1,iz-1, type)
#define iuclockwiserecordF(type) recordF_m( 0,0,0,1,1,0, 0, ix  ,iy-1,iz-1, type)




	int iCells=-1;


	array<int,255> iFaces; iFaces.fill(-1);

	for (int iz=1;iz<=n[2];iz++)
	{  cout<<(iz%80 ? '.' : '\n');cout.flush();
		for (int iy=1;iy<=n[1];iy++)
		 for (int ix=1;ix<=n[0];ix++)
		  if (!vxlImg(ix,iy,iz))
		  {

				iCells++;

				unsigned char 
				neiv=vxlImg(ix-1,iy,iz);
				if (ix!=1)
				{
				  if (neiv)    {iclockwiserecordF( neiv)}
				  else                         {iclockwiserecordF( Internal);}
				}else if (neiv) {iclockwiserecordF( neiv)}
				else                           {iclockwiserecordF( Left);}

				neiv=vxlImg(ix+1,iy,iz);
				if (ix!=n[0])
				{
				  if (neiv)    {iuclockwiserecordF( neiv)}
				  else                         {iuclockwiserecordF( Internal);}
				}else if (neiv) {iuclockwiserecordF( neiv)}
				else                           {iuclockwiserecordF( Right); }


				neiv=vxlImg(ix,iy-1,iz);
				if (iy!=1)
				{
				  if (neiv)    {jclockwiserecordF( neiv)}
				  else                         {jclockwiserecordF( Internal);}
				}else if (neiv) {jclockwiserecordF( neiv)}
				else                           {jclockwiserecordF( Bottom)}

				neiv=vxlImg(ix,iy+1,iz);
				if (iy!=n[1])
				{
				  if (neiv)    {juclockwiserecordF( neiv)} 
				  else                         {juclockwiserecordF( Internal);}
				}else if (neiv) {juclockwiserecordF( neiv)} 
				else                           {juclockwiserecordF( Top)}
				

				neiv=vxlImg(ix,iy,iz-1);
				if (iz!=1)
				{
				  if (neiv)    {kclockwiserecordF(neiv)}
				  else                         {kclockwiserecordF( Internal);}
				}else if (neiv) {kclockwiserecordF( neiv)}
				else                           {kclockwiserecordF( Back)}

				neiv=vxlImg(ix,iy,iz+1);
				if (iz!=n[2])
				{
				  if (neiv)    {kuclockwiserecordF( neiv)}
				  else                         {kuclockwiserecordF( Internal);}
				}else if (neiv) {kuclockwiserecordF( neiv)}
				else                           {kuclockwiserecordF( Front)}

		  }
	}




	point_mapper.reset(0,0,0,0);




	ofstream faces((Folder+"/faces").c_str());
	assert(faces);
	faces<<
	"FoamFile\n"
	"{\n"
	"	version	 2.0;\n"
	"	format	  ascii;\n"
	"	class	   faceList;\n"
	"	location	\""<<Folder+"\";\n"
	"	object	  faces;\n"
	"}\n\n";


	ofstream owner((Folder+"/owner").c_str());
	assert(owner);
	owner<<
	"FoamFile\n"
	"{\n"
	"	version	 2.0;\n"
	"	format	  ascii;\n"
	"	class	   labelList;\n"
	"	location	\""<<Folder+"\";\n"
	"	object	  owner;\n"
	"}\n\n";
 
	ofstream neighbour((Folder+"/neighbour").c_str());
	assert(neighbour);
	neighbour<<
	"FoamFile\n"
	"{\n"
	"	version	 2.0;\n"
	"	format	  ascii;\n"
	"	class	   labelList;\n"
	"	location	\""<<Folder+"\";\n"
	"	object	  neighbour;\n"
	"}\n\n";

//______________________________________________________________________________________________________________________
#define write_faces_owners(ipp) \
  for (std::vector<array<int,6> >::iterator ff=faces_bs[ipp].begin();ff<faces_bs[ipp].end();ff++)	\
	{ \
		faces<<'('<<((*ff))[0]<<' '<<(*ff)[1]<<' '<<(*ff)[2]<<' '<<(*ff)[3]<<")\n"; \
		owner<<(*ff)[4]<<"\n";  \
	}


	owner<<sumnFaces<<endl;
	owner<<"("<<endl;
	faces<<sumnFaces<<endl
		 <<"("<<endl;

		//write_faces_owners(Internal)
		for(int ib=0;ib<nVVs;++ib)  	write_faces_owners(ib)
		if (procIsijk(iProc-1,jProc,kProc)<0) write_faces_owners(Left)
		if (procIsijk(iProc+1,jProc,kProc)<0) write_faces_owners(Right)
		if (procIsijk(iProc,jProc-1,kProc)<0) write_faces_owners(Bottom)
		if (procIsijk(iProc,jProc+1,kProc)<0) write_faces_owners(Top)
		if (procIsijk(iProc,jProc,kProc-1)<0) write_faces_owners(Back)
		if (procIsijk(iProc,jProc,kProc+1)<0) write_faces_owners(Front)

		if (procIsijk(iProc-1,jProc,kProc)>=0) write_faces_owners(Left)
		if (procIsijk(iProc+1,jProc,kProc)>=0) write_faces_owners(Right)
		if (procIsijk(iProc,jProc-1,kProc)>=0) write_faces_owners(Bottom)
		if (procIsijk(iProc,jProc+1,kProc)>=0) write_faces_owners(Top)
		if (procIsijk(iProc,jProc,kProc-1)>=0) write_faces_owners(Back)
		if (procIsijk(iProc,jProc,kProc+1)>=0) write_faces_owners(Front)


	faces<<")"<<endl;
	faces.close();
	owner<<")"<<endl;
	owner.close();


	neighbour<<nFaces[Internal]<<endl;
	neighbour<<"("<<endl;
	  for (std::vector<array<int,6> >::iterator ff=faces_bs[Internal].begin();ff<faces_bs[Internal].end();ff++)
		{
			neighbour<<(*ff)[5]<<"\n";
		}

	neighbour<<")"<<endl;
	neighbour.close();
	std::cout<<" :/ "<<endl;

}



