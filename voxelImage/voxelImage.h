/*-------------------------------------------------------------------------*\


This file is part of voxelImage library, a small c++ template library  
developed by Ali Qaseminejad Raeini for handelling 3D raw images.


Please see our website for relavant literature making use of this code:
http://www3.imperial.ac.uk/earthscienceandengineering/research/perm/porescalemodelling

For further information please contact us by email:
Ali Q Raeini:    a.qaseminejad-raeini09@imperial.ac.uk

\*-------------------------------------------------------------------------*/


#ifndef voxelImageT_H
#define voxelImageT_H

#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>
#include <valarray>
#include <cassert>
#include <sstream>
#include <memory>
#include <algorithm>
#include <numeric>


#include "vec3.h"



template <typename Type> class voxelField
{
 protected:
 public:
	long long nij_;
	int3  nnn_;
	std::vector<Type> data_;
	voxelField(): nij_(0), nnn_{{0,0,0}} {};
	voxelField(int3 n, Type value) {  reset(n, value);  };
	voxelField(int n1, int n2, int n3, Type value) {  reset(int3{{n1,n2,n3}}, value);  };
	voxelField(const voxelField<Type>& vm ): nij_(vm.nij_), nnn_(vm.nnn_), data_(vm.data_) {} ;

	void reset(int3 n);
	void reset(int3 n, Type value);
	const Type& operator()(const int i, const int j, const size_t k) const { return data_[k*nij_+j*nnn_[0]+i]; }
	      Type& operator()(const int i, const int j, const size_t k)       { return data_[k*nij_+j*nnn_[0]+i]; }
	const Type& operator()(const size_t iii) const { return data_[iii]; }
	      Type& operator()(const size_t iii)       { return data_[iii]; }
	const Type& v_i(const int i, const Type* vr) const { return *(vr+i); }
	const Type& v_j(const int j, const Type* vr) const { return *(vr+j*int(nnn_[0])); }
	const Type& v_k(const int k, const Type* vr) const { return *(vr+k* static_cast<long long>(nij_)); }
	void reset(int n1, int n2, int n3, Type value) {  reset(int3{{n1,n2,n3}}, value);  };
	void readMicroCT(std::string);
	void readMicroCTHeader(std::ifstream);
	bool readAscii(std::string);
	void readAscii(std::ifstream& in);
	bool readBin(std::string fileName);
	bool readBin(std::string fileName,int iStart,int iEndp1 , int jStart,int jEndp1 , int kStart,int kEndp1 );
	void writeBin(std::string fileName) const;
	void writeBin(std::string fileName,int iStart,int iEndp1 , int jStart,int jEndp1 , int kStart,int kEndp1 ) const;
	void writeAscii(std::string fileName) const;
	void writeAscii(std::string fileName,int iStart,int iEndp1 , int jStart,int jEndp1 , int kStart,int kEndp1) const;
	void writeRotatedXZ(std::ofstream& of) const;
	int3 size3() const;
	int3 sizeu3() const;
	void getSize(int& n1, int& n2, int& n3) const;
};




class voxelImageTBase
{
public:
	virtual ~voxelImageTBase() {};
	virtual void write(std::string fileName) const = 0;
	virtual void printInfo() const = 0;
	virtual int3 sizeu3() const = 0;
	virtual int getInt(int i, int j, int k) const = 0;
	virtual double getDbl(int i, int j, int k) const = 0;
	virtual double vv(double i, double j, double k) const = 0;
	virtual const vec3& dx() const = 0;
	virtual const vec3& X0() const = 0;

};


template <typename T>
class voxelImageT: public voxelField<T>, public voxelImageTBase
{

	vec3	X0_, dx_;

 public:

	voxelImageT():X0_(0.0,0.0,0.0),dx_(1,1,1) {};


	voxelImageT(int n1, int n2, int n3, T value)
	: voxelField<T>( n1,  n2,  n3,  value),  X0_(0.0,0.0,0.0), dx_(1,1,1) {}


	voxelImageT(int3 n, vec3 dx, vec3 xmin, T value)
	: voxelField<T>( n[0],  n[1],  n[2],  value), X0_(xmin),dx_(dx) {}

	voxelImageT(const voxelImageT & vm)
	:  voxelField<T>(vm), X0_(vm.X0_), dx_(vm.dx_) {}


	voxelImageT(std::string headerName, int processKeys=1, std::string fileName="")
	: X0_(0.0,0.0,0.0),dx_(1,1,1)  {readFromHeader(headerName, processKeys,fileName);}
	void readFromHeader(std::string headerName, int processKeys=1, std::string fileName="")
	{	if (!headerName.empty())
		{	std::cout<<"Openning header file: "<<headerName<<std::endl;
			std::ifstream headerFile(headerName.c_str());
			if(!headerFile)  {std::cout<<"\n\n\nError: can not open header file, "<<headerName<<std::endl<<std::endl; }
			else
				readFromHeader(headerFile,headerName,processKeys,fileName);
			headerFile.close();
		}
	}

	void readFromHeader( std::ifstream& headerFile,	std::string headerName, int processKeys=1, std::string fileName="");



	bool readAscii(std::string fileName)
	{	///  overwrite as the parent -voxelField<T>- interprets
		///  numerical values as characters not integers

		std::cout<<  " reading "<<fileName<<std::endl;

		//if ( (fileName.compare(fileName.size()-4,4,".dat")==0) || (fileName.compare(fileName.size()-4,4,".txt") == 0) )
		//{
			std::ifstream in(fileName.c_str());
			assert(in);

			char tmpc[8];
			for ( int i=0; i<8;i++)   in>>tmpc[i];
			if (std::string(tmpc).compare(0,4,"ascii") == 0) //ignore first lines
			{
				int n[3];
				in>>n[2]>>n[0]>>n[1];//ignore first lines
				double  xmin[3],xmax[3];
				in>> xmin[0]>>xmax[0]>>xmin[1]>>xmax[1]>>xmin[2]>>xmax[2] ;
				std::cout<<"Warning: ignoring the header of file "<<fileName<<std::endl;
			}
			else
				in.seekg(0, in.beg);
			readAscii(in);
			in.close();
			return !in.fail();
		//}
		//else
			//this->readBin(fileName);

	}


	void  readAscii(std::ifstream& in)
	{	///  overwrite as the parent -voxelField<T>- interprets
		///  numerical values as characters not integers
		int tmp=0;
		for (auto& vv : voxelField<T>::data_)
		{
			in>>tmp;
			vv=tmp;
		}
	}



	void cropD( int3 cropBegin,  int3 cropEnd,int emptylayers=0, T emptylayersValue=1) ;
	void crop( int cropBegin[3],  int cropEnd[3],int emptylayers=0, T emptylayersValue=1) ;
	void crop(int iStart, int iEnd ,
				 int jStart, int jEnd ,
				 int kStart, int kEnd ,
				 int emptylayers=0,T emptylayersValue=1);

	void writeHeader(std::string fileName, std::string extrasufix="") const;
	void writeHeader(std::string fileName, int3 iStart, int3 iEnd, std::string  extrasufix="") const;


	void erodeLayer(int i);
	void resample(double i);
	void resampleMax(double i);
	void rotate(char direction);
	void PointMedian026(int thereshold0,int thereshold1);
	void FaceMedian06(int thereshold0,int thereshold1);
	void mode(short nNeist);

	void AND(const voxelImageT& data2);
	void NOT(const voxelImageT& data2);
	void OR(const voxelImageT& data2);
	void XOR(const voxelImageT& data2);
	void maxEq(const voxelImageT& data2);
	void minEq(const voxelImageT& data2);


	void fillHoles(int maxHoleRadius);

	void shrinkPore();
	void growPore();


   void threshold101(T theresholdMin,T theresholdMax);


	void writeAConnectedPoreVoxel(std::string fileName) const;
	void setLayer(int k, const T* Values);
	void setSlice(char dir, int ijk, T vv);
	void replaceyLayer(int j, int fromj);
	void replacexLayer(int i, int fromi);
	void setBlock(int n1, int n2, int n3, const voxelField<T>&Values);
	template<typename T2> void resetFrom(const voxelImageT<T2>&Values);
	void setFrom(const voxelImageT<T>&Values, int n1, int n2, int n3);
	void growBox(int nlyr);
	void shrinkBox(int nlyr)
		{int3 beg={{nlyr,nlyr,nlyr}},
		end={{((*this).size3()[0]-nlyr),((*this).size3()[1]-nlyr),((*this).size3()[2]-nlyr)}};
		cropD(beg,end);};

	void write(std::string fileName) const;
	double volFraction(T vv1,T vv2) const;
	void printInfo() const;

	const vec3& X0() const {return X0_;};
	vec3& X0Ch()    {return X0_;};
	const vec3& dx() const {return dx_;};
	vec3& dxCh()    {return dx_;};
	int3 sizeu3() const {return voxelField<T>::sizeu3();};
	int getInt(int i, int j, int k) const { return (*this)(i,j,k); };
	double getDbl(int i, int j, int k) const { return (*this)(i,j,k); };
	double vv(double i, double j, double k) const
	{
		const int i0=i, j0=j, k0=k; const double di=i-i0, dj=j-j0, dk=k-k0;
		const int i1=i0+1, j1=j0+1, k1=k0+1;
		const double v00=(*this)(i0,j0,k0)*(1.0-di)+(*this)(i1,j0,k0)*(di);
		const double v10=(*this)(i0,j1,k0)*(1.0-di)+(*this)(i1,j1,k0)*(di);
		const double v01=(*this)(i0,j0,k1)*(1.0-di)+(*this)(i1,j0,k1)*(di);
		const double v11=(*this)(i0,j1,k1)*(1.0-di)+(*this)(i1,j1,k1)*(di);
		return ( v00*(1.0-dj)+v10*(dj) )*(1.0-dk)  + ( v01*(1.0-dj)+v11*(dj) )*(dk);
	};

 };





typedef voxelImageT<unsigned char> voxelImage;

#include "voxelImageI.h"
#include "voxelImageReader.h"


#endif
