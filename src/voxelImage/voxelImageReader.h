/*-------------------------------------------------------------------------*\

This file is part of voxelImage library, a C++ template library  
developed by Ali Qaseminejad Raeini for handelling 3D raw images.


Please see our website for relavant literature making use of this code:
http://www3.imperial.ac.uk/earthscienceandengineering/research/perm/porescalemodelling

For further information please contact us by email:
Ali Q Raeini: a.qaseminejad-raeini09@imperial.ac.uk

\*-------------------------------------------------------------------------*/

#include <sstream>
//#include "shapeToVoxel.h"

//#ifndef GLOBALS_SkipH
//#define Dbg_SkipH
//#include "globals.h"
//#undef  Dbg_SkipH
//#endif

namespace MCTProcessing
{
	using std::cout;
	using std::endl;
template<typename T> bool ignore( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	if (ins.good() && vxlImage.size3()[0]==-1) std::cout<<" ";
	
	return true;
}

template<typename T> bool fillHoles( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	unsigned int maxHoleSize;
	ins>>maxHoleSize;

		std::cout<<"fillHoles: eliminating isolated rocks/pores; maxHoleSize:" <<maxHoleSize<<" (default is 2) "<<std::endl;
		vxlImage.fillHoles(maxHoleSize);

		vxlImage.FaceMedian06(1,5);
		//vxlImage.FaceMedian07(2,5);
		//vxlImage.FaceMedian07(2,5);
		return true;
}

template<typename T> bool selectPore( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
		std::cout<<"  converting to binary (0 and 1):"<<std::endl
			 <<"  selecting pore (->0) with values between:";
		unsigned int  thresholdMin=0,thresholdMax=0;
		ins>>thresholdMin;
		ins>>thresholdMax;

		std::cout<<" "<<int(thresholdMin)<<"  and "<<int(thresholdMax)<<"  inclusive."<<std::endl;
		vxlImage.threshold101(thresholdMin,thresholdMax);
		return true;
}

template<typename T> bool rescale( std::stringstream & ins, voxelImageT<T>& vxlImage)
{ //Warning not working for float3 in c++11
	//thresholdImage=true;
	(std::cout<<"  rescaling voxel values to [ ").flush();
	unsigned int  thresholdMin=0,thresholdMax=0;
	ins>>thresholdMin;
	ins>>thresholdMax;

	(std::cout<<thresholdMin<<", "<<thresholdMax<<" ]    ").flush();
	rescale(vxlImage,T(thresholdMin),T(thresholdMax));
	(std::cout<<".").flush();
	return true;
}

template<typename T> bool growPore( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
		std::cout<<"  growing voxels:"<<std::endl;
		int voxelValueTogrow; ins>>voxelValueTogrow;
 		char growingAlgorithm; ins>>growingAlgorithm;

		while (ins.good())		  // loop while extraction from file is possible
		{
			if (growingAlgorithm!='f')
			{
				if(voxelValueTogrow==0)
					vxlImage.growPore();
				else if (voxelValueTogrow==0)
					vxlImage.shrinkPore();
				else
				{
					std::cerr<<"growing is only implemented for binary images: "<<
					"selected voxel value to grow is "<<voxelValueTogrow << ", which is not acceptable"<<std::endl;
					return false;//error occurred
				}
			}
			else
			{
				std::cerr<<"selected growing algorithm: "<<growingAlgorithm<<
				" the only implemented algorithm is f which stands for faceGrowing"<<std::endl;
				return false;//error occurred
			}

			ins>>voxelValueTogrow;
			ins>>growingAlgorithm;
		}
		std::cout<<" done"<<std::endl;
		return true;
}


template<typename T> bool resampleMean( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	double nResample=1;
		ins>>nResample, std::cout<<__FUNCTION__<<" factor: "<<nResample<<std::endl;
		vxlImage = resampleMean(vxlImage,nResample);
		return true;
}


template<typename T> bool resampleMax( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	double nResample=1;
		ins>>nResample, std::cout<<__FUNCTION__<<" factor: "<<nResample<<std::endl;
		vxlImage = resampleMax(vxlImage,nResample);
		return true;
}

template<typename T> bool resliceZ( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	double nResample=1;
		ins>>nResample, std::cout<<__FUNCTION__<<" factor: "<<nResample<<std::endl;
		vxlImage = resliceZ(vxlImage,nResample);
		return true;
}

template<typename T> bool resampleMode( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	double nResample=1;
		ins>>nResample, std::cout<<__FUNCTION__<<" factor: "<<nResample<<std::endl;
		vxlImage = resampleMode(vxlImage,nResample);
		return true;
}

template<typename T> bool redirect( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
		char direction;
		ins>>direction;
		(std::cout<<direction<<", swapping x and "<<direction<<" directions").flush();

		vxlImage.rotate(direction);
		std::cout<<std::endl;
		return true;
}

template<typename T> bool replaceRange( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	int  thresholdMin(0),thresholdMax(0); ///. Warning don't use T, uchar wont work
	ins >> thresholdMin >> thresholdMax;

	int  value=(thresholdMin+thresholdMax)/2; ///. Warning don't use T, uchar wont work
	ins >> value;

	std::cout<<" Replacing range  ["<<thresholdMin<<"  "<<thresholdMax<<"] with "<<value<<";   ";
	replaceRange(vxlImage,T(thresholdMin),T(thresholdMax),T(value));
	(std::cout<<".").flush();
	return true;
}

template<typename T> bool crop( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	int cropBegin[3], cropEnd[3];

	std::cout<<"Crop:   ";
	for (int i=0; i<3;++i)   ins>>cropBegin[i] >>cropEnd[i],  std::cout<<cropBegin[i]<<' '<<cropEnd[i]<<"	";
	std::cout<<' '<<std::endl;

	//cropEnd[0]+=1; cropEnd[1]+=1; cropEnd[2]+=1;
	vxlImage.crop(cropBegin,cropEnd);
	return true;
}


template<typename T> bool cropD( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	int3 cropBegin(0,0,0), cropEnd=vxlImage.size3();
	int nLayers(0); int value(1);
	std::cout<<"cropD:   ";
	ins>>cropBegin[0] >>cropBegin[1] >>cropBegin[2];  std::cout<<" "<<cropBegin[0] <<" "<<cropBegin[1] <<" "<<cropBegin[2]<<" -- ";
	ins>>cropEnd[0] >>cropEnd[1] >>cropEnd[2];		std::cout<<cropEnd[0] <<" "<<cropEnd[1] <<" "<<cropEnd[2]<<"  +  ";;
	ins >> nLayers >> value;
	std::cout<<nLayers<<" layers of "<<value<<std::endl;
	vxlImage.cropD(cropBegin,cropEnd,nLayers,value);
	return true;
}

template<typename T> bool write( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	std::string outName("dump.tif");
	ins >> outName;
	vxlImage.write(outName);
	(std::cout<<".").flush();
	return true;
}

template<typename T> bool writeUchar( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	std::string outName("dump.tif");
	ins >> outName;
	double minv=-0.5, maxv=255.0;
	ins>>minv>>maxv;
	double delv=255.499999999/(maxv-minv);
	(std::cout<<minv<<" "<<maxv).flush();
	voxelImageT<unsigned char> voxels(vxlImage.size3(),vxlImage.dx(),vxlImage.X0(),255);
	forAlliii_(voxels) voxels(iii)=std::max(0,std::min(255,int(delv*(vxlImage(iii)-minv))));
	voxels.write(outName);
	(std::cout<<".").flush();
	return true;
}

template<typename T> bool read( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	int3 nnn = vxlImage.size3();
	std::string fnam;
	ins>>fnam;
	std::cout<<"  reading from  image "<<fnam<<std::endl;
	if(fnam.size()>4)
	{
		if ( hasExt(fnam,4,".tif") || hasExt(fnam,7,".raw.gz") || hasExt(fnam,4,".raw") )
		{
			  vxlImage.reset(nnn,0);
			  vxlImage.readBin(fnam);
		}
		else vxlImage.readFromHeader(fnam,0);
	}
	return true;
}
template<typename T> bool readAtZ( std::stringstream & ins, voxelImageT<T>& vxlImage)
{// used to stitch images
	int3 nnn = vxlImage.size3();
	size_t iSlic=0;
	std::string fnam;
	ins>>fnam>>iSlic;
	std::cout<<"  reading from  image "<<fnam<<", assigning to slices after "<<iSlic<<std::endl;
	voxelImageT<T> img(fnam);
	ensure(img.size3()[0]==nnn[0]);	ensure(img.size3()[1]==nnn[0]);
	std::copy(img.begin(),img.end(),vxlImage.begin()+iSlic*nnn[0]*nnn[1]);
	return true;
}

template<typename T> bool medianFilter( std::stringstream & ins, voxelImageT<T> & vxlImage)
{
	int nIterations(1);
	ins >> nIterations;
	(std::cout<<"  median Filter, nIterations: "<<nIterations).flush();
	vxlImage.growBox(2);
	for (int i=0; i<nIterations; ++i)
	{
		vxlImage=median(vxlImage);
	}
	vxlImage.shrinkBox(2);
	(std::cout<<".").flush();
	return true;
}
template<typename T> bool medianX( std::stringstream & ins, voxelImageT<T> & vxlImage)
{
	int nIterations(1);
	ins >> nIterations;
	(std::cout<<"  median Filter, nIterations: "<<nIterations).flush();
	for (int i=0; i<nIterations; ++i)
	{
		vxlImage=medianx(vxlImage);
	}
	(std::cout<<".").flush();
	return true;
}


template<typename T> bool FaceMedian06( std::stringstream & ins, voxelImageT<T> & vxlImage)
{
	if(ins.peek()=='?') { ins.str("thereshold0(2), thereshold1(4),  nIterations(1)"); return true; }
	int thereshold0(2), thereshold1(4),  nIterations(1);
	ins >> thereshold0>> thereshold1>> nIterations;
	(std::cout<<"  FaceMedian06: "<<thereshold0<<" "<<thereshold1<<" "<<nIterations<<"     ").flush();
	vxlImage.growBox(2);
	for (int i=0; i<nIterations; ++i)
	{
		vxlImage.FaceMedian06(thereshold0,thereshold1);
	}
	vxlImage.shrinkBox(2);
	(std::cout<<".").flush();
	return true;
}



template<typename T> bool PointMedian026( std::stringstream & ins, voxelImageT<T> & vxlImage)
{
	int nIterations(1),  thereshold0(11), thereshold1(15);
	ins >> thereshold0>> thereshold1>> nIterations;
	(std::cout<<"  PointMedian026: "<<thereshold0<<" "<<thereshold1<<" "<<nIterations<<"     ").flush();
	vxlImage.growBox(2);
	for (int i=0; i<nIterations; ++i)
	{
		vxlImage.PointMedian026(thereshold0,thereshold1);
	}
	vxlImage.shrinkBox(2);
	(std::cout<<".").flush();
	return true;
}


template<typename T> bool circleOut( std::stringstream & ins, voxelImageT<T> & vxlImage)
{

	char d='z';
	ins >> d;
	int i = std::max<int>(d-'x',0);
	int X0(vxlImage.size3()[(i+1)%3]/2), Y0(vxlImage.size3()[(i+2)%3]/2);
	int R((X0+Y0)/2);

	ins >> X0 >> Y0 >> R;
	(std::cout<<"  circleOut: dir="<<d<<",  X0="<<X0 <<"  Y0="<<Y0  <<"  R="<<R ).flush();

	circleOut(vxlImage,X0,Y0,R,d);

	(std::cout<<".").flush();
	return true;
}


template<typename T> bool maskWriteFraction( std::stringstream & ins, voxelImageT<T> & vxlImage)
{
	int maskvv(2);
	T minIelm(1), maxIelm=std::numeric_limits<T>::max();
	std::string maskname, outName("maskWriteFraction.txt");
	ins >> maskname >> outName >> maskvv >> minIelm >> maxIelm;
	(std::cout<<"  maskWriteFraction:  mask:"<<maskname <<"  outName:"<<outName<<"  maskvv:"<<maskvv  <<"  minIelm:"<<minIelm<<"  maxIelm:"<<maxIelm ).flush();

	maskWriteFraction(vxlImage,maskname,outName,maskvv,minIelm,maxIelm);

	(std::cout<<".").flush();
	return true;
}


template<typename T> bool Offset( std::stringstream & ins, voxelImageT<T> & vxlImage)
{
	dbl3 offset;
	ins >> offset;
	(std::cout<<"  Offset:"<<offset<<" " ).flush();
	vxlImage.X0Ch()=offset;
	(std::cout<<".").flush();
	return true;
}


template<typename T>  bool growLabel( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	int  vv(255),nIters(0);
	ins >> vv>>nIters;
	(std::cout<<"  growLabel: "<<vv<<" x"<<nIters ).flush();


	for (int i=0; i<=nIters;++i)
	 vxlImage.growLabel(vv);

	(std::cout<<".").flush();
	return true;
}


template<typename T>  bool reset( std::stringstream & ins, voxelImageT<T>& vxlImage)
{
	std::string param;
	ins >>param;
	while(param.size())
	{
		if(param=="N")
		{ 	int3 N{0,0,0}; 	ins>>N; 	vxlImage.reset(N); cout<<"N:"<<N<<" "; 	}
		else if(param=="X0")
		{ 	dbl3 X0; 	ins>>X0; 	vxlImage.X0Ch()=X0; cout<<"X0:"<<X0<<" "; 	}
		else if(param=="dx")
		{ 	dbl3 dx(1.,-1e64,1.); 	ins>>dx; 
			if(dx[1]<0) {ensure(dx[1]>0, "dx[1&2] set to dx[0]"); dx[1]=dx[0]; dx[2]=dx[1]; }
			vxlImage.dxCh()=dx; cout<<"dX:"<<dx<<" "; 	}
		else if(param[0]=='V')
		{ 	int3 N=vxlImage.size3();
			double vv = 0.0; 	ins>>vv>>N;
			vxlImage.reset(N,vv);
		}
		else if(param=="Nd0")
		{ 	dbl3 dx(1.,1.,1.), X0(0.,0.,0.); int3 N=vxlImage.size3(); 	ins>>N>>dx>>X0;
			vxlImage.reset(N); cout<<"N:"<<N<<" ";  vxlImage.dxCh()=dx; cout<<"dX:"<<dx<<" "; 	vxlImage.X0Ch()=X0; cout<<"X0:"<<X0<<" "; 	}
		else cout<<"reset does not support "<<param<<endl;
		param="";
		ins >>param;
	}
	(std::cout<<".").flush();
	return true;
}

template<typename T> std::unordered_map<std::string,bool(*)( std::stringstream&, voxelImageT<T>&)>
 namedProcesses()
{
	typedef bool(*ProcessP)( std::stringstream&  ins, voxelImageT<T>& vxlImage);
	return std::unordered_map<std::string,ProcessP>{
		{  ""             , ProcessP(& ignore)},// ProcessP can be removed if using g++
		{  ";"		      , ProcessP(& ignore )},
		{  "fillHoles"    , ProcessP(& fillHoles )},
		{  "reset"	      , ProcessP(& reset )},
		{  "rescale"	  , ProcessP(& rescale )},
		{  "pore"		  , ProcessP(& selectPore )},
		{  "threshold"    , ProcessP(& selectPore )},
		{  "threshold101" , ProcessP(& selectPore )},
		{  "resampleMean" , ProcessP(& resampleMean )},
		{  "Offset"       , ProcessP(& Offset )},
		{  "direction"    , ProcessP(& redirect )},
		{  "crop"	      , ProcessP(& crop )},
		{  "cropD"	      , ProcessP(& cropD )},
		{  "resampleMax"  , ProcessP(& resampleMax )},
		{  "resampleMode" , ProcessP(& resampleMode )},
		{  "resliceZ"     , ProcessP(& resliceZ )},
		{  "replaceRange" , ProcessP(& replaceRange )},
		{  "write"        , ProcessP(& write )},
		{  "writeUchar"   , ProcessP(& writeUchar )},
		{  "read"         , ProcessP(& read )},
		{  "readAtZ"      , ProcessP(& readAtZ )},
		{  "medianFilter" , ProcessP(& medianFilter )},
		{  "medianX"      , ProcessP(& medianX )},
		{  "FaceMedian06" , ProcessP(& FaceMedian06 )},
		{  "PointMedian026" , ProcessP(& PointMedian026 )},
		{  "circleOut"  , ProcessP(& circleOut )},
		{  "growLabel"  , ProcessP(& growLabel )},
		{  "maskWriteFraction"  ,ProcessP(& maskWriteFraction )}
		//,{  "shapeToVoxel"  ,ProcessP(& shapeToVoxel )},
		//{  "shapeToVoxelAdd"  ,ProcessP(& shapeToVoxelAdd )},
		};
}



}


template<typename T>
void VxlKeysProcess
(
	std::istream& keyins,
	voxelImageT<T>& vxlImage,
	const std::string& hdrNam
)
{
	typedef bool(*ProcessP)( std::stringstream&  ins, voxelImageT<T>& vxlImage);
	std::unordered_map<std::string,ProcessP> key_funs = MCTProcessing::namedProcesses<T>();


	while (true)
	{
		std::streampos begLine = keyins.tellg();
		std::string tmpStr;
		keyins>>tmpStr;
		//bool validKey=false;
		//cout<<tmpStr<<endl;///. keep me
		if (keyins.fail())
		{std::cout<<" Finished reading "<<hdrNam<<":/  "<<keyins.tellg()<<std::endl;  break; }
		else if (tmpStr[0]=='#' || tmpStr[0]=='\'' || tmpStr[0]=='/' || tmpStr[0]=='%')
		{
			keyins.ignore(10000,'\n');
			//validKey=true;
		}
		else
		{
			auto paer = key_funs.find(tmpStr);
			if (paer!=key_funs.end())	
			{
				(std::cout<<" "<<tmpStr<<": ").flush();
				std::stringstream ss;
				if(keyins.peek()!='\n') keyins.get (*(ss.rdbuf()));
				(*(paer->second))(ss,vxlImage);
				std::cout<<std::endl;
				//validKey=true;
			}
			else
			{	std::cout<<"  read "<<hdrNam<<" util entry \""<<tmpStr<<"\":/ \n"<<std::endl;
				keyins.clear();
				keyins.seekg(begLine);
				break;
			}
		}
	}

}

inline std::string VxlKeysHelp(std::string keyname="", std::string subkey="")
{
	typedef bool(*ProcessP)( std::stringstream&  ins, voxelImageT<unsigned char>& vxlImage);
	std::unordered_map<std::string,ProcessP> key_funs = MCTProcessing::namedProcesses<unsigned char>();

	std::stringstream keys;
	if(keyname.size())
	{
		auto paer = key_funs.find(keyname);
		if (paer!=key_funs.end())
		{
			voxelImageT<unsigned char> vxlImg;
			std::stringstream ss;
			ss.str(subkey.empty()?  "?" : "? "+subkey);
			(*(paer->second))(ss, vxlImg);
			return ss.str();
		}
		else
			std::cout<<" Error: no such keyword "<<keyname<<std::endl; 
		keys<<"//!-*- C -*- keywords:\n";
		for(const auto& proc:key_funs) 	keys<<proc.first<<"\n";
		keys<<" Error: no such keyword "<<keyname<<"\n\n";
	}
	else
		for(const auto& proc:key_funs) 	keys<<proc.first<<"\n";
	//std::cout<<keys.str();
	return keys.str();
}


template<typename T>
void voxelImageT<T>::readFromHeader
(
	std::istream& headerFile,
	const std::string& hdrNam,
	int procesKeys,
	std::string inputName
)
{
	int3 n(0,0,0);
	std::string BinaryData="XXX";
	bool X0read=false, dxread=false, autoUnit=true; //auto unit only applies to .mhd format
	double unit_=1.0;
	int nSkipBytes(0);
	#ifdef TIFLIB
	if (hasExt(hdrNam,4,".tif"))
	{
		(std::cout<<  " reading tif file "<<hdrNam<<" ").flush();
		readTif(*this, hdrNam);
		std::cout<<  " ."<<std::endl;
		return;
	}
	else
	#endif
	if (hasExt(hdrNam,4,".mhd") || hasExt(hdrNam,3,".py"))
	{
		std::cout<<" mhd:"<<hdrNam<<": "<<std::endl;
		while (true)
		{
			std::string tmpStr;
			std::streampos begLine = headerFile.tellg();
			headerFile>>tmpStr;

			

			//ObjectType = Image
			//NDims = 3
			//Offset = 0 0 0
			//ElementSize = 8 8 8
			//DimSize = 200 225 153
			//ElementType = MET_UCHAR
			//ElementDataFile = Ketton100.raw
			std::stringstream ss;
			if(headerFile.peek()!='\n') headerFile.get (*(ss.rdbuf()));
			if (headerFile.fail()) break;
			std::string tmp;
			if (tmpStr == "ObjectType")
			{
				ss >> tmp; ss >> tmp;
				if (tmp != "Image") std::cout<<" Warning: ObjectType != Image :="<<tmp<<std::endl;
			}
			else if (tmpStr == "NDims")
			{
				ss >> tmp; ss >> tmp;
				if (tmp != "3") std::cout<<" Warning: NDims != 3 :="<<tmp<<std::endl;
			}
			else if (tmpStr == "ElementType")
			{
				ss >> tmp; ss >> tmp;
				if ((tmp != "MET_UCHAR") && (sizeof(T)==1)) std::cout<<" Warning: ElementType != MET_UCHAR :="<<tmp<<std::endl;
			}
			else if (tmpStr == "Offset")
			{
				ss >> tmp; ss>>	X0_[0]>>X0_[1]>>X0_[2] ;
				std::cout<<" X0: "<<  X0_[0]<<"  "<<X0_[1]<<"   "<<X0_[2]<<std::endl ;
				X0read=true;
			}
			else if (tmpStr == "ElementSize" || tmpStr == "ElementSpacing")
			{
				ss >> tmp; ss>>	dx_[0]>>dx_[1]>>dx_[2] ;
				std::cout<<" dX: "<< dx_[0]<<"  "<<dx_[1]<<"  "<<dx_[2]<<"   ";
				dxread=true;
				std::cout<<std::endl;
			}
			else if (tmpStr == "DimSize")
			{
				ss >> tmp; ss>>	n[0]>>n[1]>>n[2];
				std::cout<<" Nxyz: "<<n[0]<<" "<<n[1]<<" "<<n[2]<<"   "<<std::endl;
			}
			else if (tmpStr == "ElementDataFile")
			{
				ss >> tmp; if (inputName.empty()) ss >> inputName;

				size_t islash=hdrNam.find_last_of("\\/");
				if (islash<hdrNam.size() && inputName[0]!='/' &&  inputName[1]!=':') inputName=hdrNam.substr(0,islash+1)+inputName;
				std::cout<<" ElementDataFile = "<<inputName<<"	"<<std::endl;
			}
			else if (tmpStr == "BinaryData")
			{
				ss >> tmp; ss >> BinaryData;
				std::cout<<" BinaryData = "<<BinaryData<<"	"<<std::endl;
			}
			else if (tmpStr == "DefaultImageFormat")
			{
				std::string defSuffix;
				ss >> tmp;
				if(tmp == "=") ss >> defSuffix; else { defSuffix=tmp; std::cerr<<"\nWarning: DefaultImageFormat should have been followed by \"=\" \n"; }
				std::cout<<" OutputFormat = "<<defSuffix<<", suffix:"<<imgExt(defSuffix)<<"	"<<std::endl; ///. sets suffix+format
			}
			else if (tmpStr == "Unit")
			{
				ss >> tmp; ss >> unit_;
				std::cout<<" Unit, OneMeter = "<<unit_<<std::endl;
				autoUnit=false;
			}
			else if (tmpStr == "HeaderSize")
			{
				ss >> tmp; ss >> nSkipBytes;
				std::cout<<"HeaderSize, nSkipBytes = "<<nSkipBytes<<std::endl;
			}
			else if (tmpStr!="BinaryDataByteOrderMSB" && tmpStr!="ElementByteOrderMSB" && tmpStr!="CompressedData" &&  tmpStr!="CompressedDataSize" &&  tmpStr!="TransformMatrix" &&
					 tmpStr!="ElementNumberOfChannels" && tmpStr!="CenterOfRotation" && tmpStr!="AnatomicalOrientation" && tmpStr!="AnatomicalOrientation")
			{
				headerFile.clear();
				headerFile.seekg(begLine);
				(std::cout<<" ; ").flush();
				break;
			}

		}

	}
	else if (hasExt(hdrNam,3,".am"))
	{
		inputName=hdrNam;
		procesKeys=0;
	}
	else
	{
		std::cout<<" (depricated) _header:"<<hdrNam<<","<<std::endl;

		char tmpc;
		for (int i=0; i<8;++i)   headerFile>>tmpc, std::cout<<tmpc;  //ignore the first 8 characters (ascii 3uc)

		if (hasExt(hdrNam,7,"_header"))  inputName=hdrNam.substr(0,hdrNam.size()-7);
		headerFile>>n[0]>>n[1]>>n[2];						// number of variables (dimension of
		std::cout<<"\n Nxyz: "<<n[0]<<" "<<n[1]<<" "<<n[2]<<"   "; std::cout.flush();
		headerFile>>	dx_[0]>>dx_[1]>>dx_[2] ;
		std::cout<<" dX: "<< dx_[0]<<"  "<<dx_[1]<<"  "<<dx_[2]<<"   "; std::cout.flush();
		headerFile>>	X0_[0]>>X0_[1]>>X0_[2] ;
		std::cout<<" X0: "<<  X0_[0]<<"  "<<X0_[1]<<"   "<<X0_[2] <<" um"<< std::endl;
		if (!headerFile)	 { std::cout<<"  Incomplete/bad header name, aborting"<<std::endl; exit(-1);}
		//if (!headerFile)	 { std::cout<<"  Incomplete/bad header name, continuing anyway"<<std::endl; }

	}


	if(std::abs(unit_-1.0)<epsT(float)) std::cout<<"  unit= "<<unit_<<" => dx= "<<dx_<<", X0= "<<X0_<<std::endl;
	this->reset(n);//,1 doesn't work for vector types
	if( !inputName.empty() && inputName!="NO_READ" && procesKeys!=2 )
	{
	  if (hasExt(inputName,4,".tif"))
	  {
			dbl3 dx=dx_, X0=X0_;
			bool readingImage = this->readBin(inputName);
			assert(readingImage);
			if(X0read) X0_=X0;
			if(dxread) dx_=dx;
	  }
	  else if ((hasExt(inputName,4,".raw") && BinaryData!="False") || BinaryData=="True")
	  {
			bool readingImage = this->readBin(inputName, nSkipBytes);
			assert(readingImage);
	  }
	  else if (hasExt(inputName,3,".am"))
	  {
			int RLECompressed;
			dbl3 dx=dx_, X0=X0_;
			getAmiraHeaderSize(inputName, n,dx_,X0_,nSkipBytes,RLECompressed);
			bool readingImage = this->readBin(inputName, nSkipBytes);
			assert(readingImage);
			if(X0read) X0_=X0;
			if(dxread) dx_=dx;
	  }
	  else if (hasExt(inputName,7,".raw.gz"))
	  {
			bool readingImage = this->readBin(inputName);
			assert(readingImage);
	  }
	  else
	  {
		std::ifstream in(inputName.c_str());
		assert(in);
		if(nSkipBytes) in.ignore(nSkipBytes);
		voxelField<T>::readAscii(in);
	  }
	}

	if(autoUnit  && dx_[0]>0.01) //&& dxread
	{
		std::cout<<"Warning: too large dx (="<<dx_[0]<<"), assuming unit is um"<<std::endl;
		unit_ = 1.0e-6;
	}
	dx_*=unit_;
	X0_*=unit_;


	if (procesKeys) VxlKeysProcess(headerFile,*this,hdrNam);

}










inline std::unique_ptr<voxelImageTBase> readImage
(
	std::string hdrNam,
	int procesKeys = 1
)
{

	(std::cout<<"voxelImage \""<<hdrNam<<"\": ").flush();
	if (hasExt(hdrNam,3,".am"))
	{
		std::string vtype = getAmiraDataType(hdrNam);
		if (vtype=="int," || vtype=="int")
		{
			std::cout<<"reading int .am file: "<<hdrNam<<std::endl;
			return std::unique_ptr<voxelImageTBase>(new voxelImageT<int>(hdrNam,0));
		}
		if (vtype=="short," || vtype=="short")
		{
			std::cout<<"reading short .am file: "<<hdrNam<<std::endl;
			return std::unique_ptr<voxelImageTBase>(new voxelImageT<short>(hdrNam,0));
		}
		else if (vtype=="ushort," || vtype=="ushort")
		{
			std::cout<<"reading ushort .am file: "<<hdrNam<<std::endl;
			return std::unique_ptr<voxelImageTBase>(new voxelImageT<unsigned short>(hdrNam,0));
		}
		else if (vtype=="byte," || vtype=="byte")
		{
			std::cout<<"reading unsigned byte .am file: "<<hdrNam<<std::endl;
			return std::unique_ptr<voxelImageTBase>(new voxelImageT<unsigned char>(hdrNam,0));
		}
		else 
		{
			std::cout<<"data type "<<vtype<<" not supported, when reading "<<hdrNam<<std::endl;
			exit(-1);
		}
	}

	#ifdef TIFLIB
	if (hasExt(hdrNam,4,".tif"))  return readTif(hdrNam);
	#endif

	std::string typ;
	std::ifstream headerFile(hdrNam.c_str());
	if(!headerFile)  
	{
		if (hdrNam.size()>4 && hdrNam[hdrNam.size()-4]=='.') std::cout<<"\n\n\nError: can not open hdrNam file, "<<hdrNam<<std::endl<<std::endl;
		typ = hdrNam; hdrNam="NO_READ";
	}
	else if (hasExt(hdrNam,4,".mhd"))
	{
		while (true)
		{
			std::string tmpStr;  headerFile>>tmpStr;
			std::stringstream ss;
			if(headerFile.peek()!='\n') headerFile.get (*(ss.rdbuf()));
			if (headerFile.fail()) {  std::cout<<"\n\n\nWarning: readImage, 'ElementType =' not set in"<<hdrNam<<std::endl; break; }
			if (tmpStr == "ElementType")  {  ss >> typ >> typ;  break; }
		}
	}
	headerFile.close();

	if (typ=="MET_UCHAR")
	 { headerFile.close();	return std::unique_ptr<voxelImageTBase>(new voxelImageT<unsigned char>(hdrNam, procesKeys)); }
	if (typ=="MET_CHAR")
	 { headerFile.close();	return std::unique_ptr<voxelImageTBase>(new voxelImageT<char>(hdrNam, procesKeys)); }
	if (typ=="MET_USHORT")
	 { headerFile.close();	return std::unique_ptr<voxelImageTBase>(new voxelImageT<unsigned short>(hdrNam, procesKeys)); }
	if (typ=="MET_SHORT")
	 { headerFile.close();	return std::unique_ptr<voxelImageTBase>(new voxelImageT<short>(hdrNam, procesKeys)); }
	if (typ=="MET_UINT")
	 { headerFile.close();	return std::unique_ptr<voxelImageTBase>(new voxelImageT<unsigned int>(hdrNam, procesKeys)); }
	if (typ=="MET_INT")
	 { headerFile.close();	return std::unique_ptr<voxelImageTBase>(new voxelImageT<int>(hdrNam, procesKeys)); }
	if (typ=="MET_FLOAT")
	 { headerFile.close();	return std::unique_ptr<voxelImageTBase>(new voxelImageT<float>(hdrNam, procesKeys)); }
	if (typ=="MET_DOUBLE")
	 { headerFile.close();	return std::unique_ptr<voxelImageTBase>(new voxelImageT<double>(hdrNam, procesKeys)); }


	return std::unique_ptr<voxelImageTBase>(new voxelImage(hdrNam, procesKeys));

}




template<typename T>
void readConvertFromHeader
(	voxelImageT<T>& vxlImg,
	std::string hdrNam,
	int procesKeys = 1
)
{
	std::unique_ptr<voxelImageTBase> vxlImgUptr = readImage(hdrNam,procesKeys);
	voxelImageTBase* vxlImgPtr = vxlImgUptr.get();
	bool red = false;
	if(!red) {auto vxlImage = dynamic_cast<voxelImageT<T>* >(vxlImgPtr);	     if(vxlImage) { vxlImg = *vxlImage; red=true; } }
	if(!red) {auto vxlImage = dynamic_cast<voxelImageT<char>* >(vxlImgPtr);	        if(vxlImage) { vxlImg.resetFrom(*vxlImage); red=true; std::cout<<"read into "<<vxlImg.size3()<<" chars "; } }
	if(!red) {auto vxlImage = dynamic_cast<voxelImageT<unsigned char>* >(vxlImgPtr);   if(vxlImage) { vxlImg.resetFrom(*vxlImage); red=true; std::cout<<"read into "<<vxlImg.size3()<<" uchrs "; } }
	if(!red) {auto vxlImage = dynamic_cast<voxelImageT<short>* >(vxlImgPtr);	       if(vxlImage) { vxlImg.resetFrom(*vxlImage); red=true; std::cout<<"read into "<<vxlImg.size3()<<" shrts "; } }
	if(!red) {auto vxlImage = dynamic_cast<voxelImageT<unsigned short>* >(vxlImgPtr);  if(vxlImage) { vxlImg.resetFrom(*vxlImage); red=true; std::cout<<"read into "<<vxlImg.size3()<<" usrts "; } }
	if(!red) {auto vxlImage = dynamic_cast<voxelImageT<int>* >(vxlImgPtr);			 if(vxlImage) { vxlImg.resetFrom(*vxlImage); red=true; std::cout<<"read into "<<vxlImg.size3()<<" intgs "; } }
	if(!red) {auto vxlImage = dynamic_cast<voxelImageT<unsigned int>* >(vxlImgPtr); 	if(vxlImage) { vxlImg.resetFrom(*vxlImage); red=true; std::cout<<"read into "<<vxlImg.size3()<<" uints "; } }
	if(!red) {auto vxlImage = dynamic_cast<voxelImageT<float>* >(vxlImgPtr);		   if(vxlImage) { vxlImg.resetFrom(*vxlImage); red=true; std::cout<<"read into "<<vxlImg.size3()<<" flots "; } }
	if(!red) {auto vxlImage = dynamic_cast<voxelImageT<double>* >(vxlImgPtr);		  if(vxlImage) { vxlImg.resetFrom(*vxlImage); red=true; std::cout<<"read into "<<vxlImg.size3()<<" dobls "; } }
	if(!red) std::cout<<"\n\ncan not convert image\n\n"<<std::endl;
}

