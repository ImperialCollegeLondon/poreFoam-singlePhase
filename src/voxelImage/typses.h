#ifndef TYPSES_H
#define TYPSES_H

#include <iomanip>
#include <sstream>
#include <fstream>
#include <array>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <map>
#include <iostream>
#include <assert.h>
#ifndef verySmall
	#define verySmall  1.0e-31
#endif


//typedef  int Int;
typedef  std::array<int,3> int3;
typedef  std::array<int3,3> int3x3;


class dbl3
{
public:

	double	x;
	double	y;
	double	z;

	dbl3() {}
	dbl3(double r, double s, double t)  { x = r;  y = s;  z = t; }
	dbl3(int3 n)  { x = n[0];  y = n[1];  z = n[2]; }
	dbl3(const double* d)  { x = d[0];  y = d[1];  z = d[2]; }

	double&       operator [](long k) {	return ((&x)[k]); }
	const double& operator [](long k) const  { return ((&x)[k]); }

	dbl3& operator +=(const dbl3& v)  { x += v.x;  y += v.y;  z += v.z;  return (*this); }
	dbl3& operator -=(const dbl3& v)  { x -= v.x;  y -= v.y;  z -= v.z;  return (*this); }
	dbl3& operator *=(double t)  { x *= t;  y *= t;  z *= t;  return (*this); }
	dbl3& operator /=(double t)  {  double f = 1.0 / t;  x *= f;  y *= f;  z *= f;  return (*this); }
	dbl3& operator ^=(const dbl3& v)  { double r, s;  r=y*v.z-z*v.y;  s=z*v.x-x*v.z;  z=x*v.y-y*v.x;  x=r; y=s; 	return (*this); }
	dbl3& operator *=(const dbl3& v)  { x *= v.x;  y *= v.y;  z *= v.z;  return (*this); }
	dbl3  operator -(void) const  { return (dbl3(-x, -y, -z)); }
	dbl3  operator +(const dbl3& v) const  { return (dbl3(x+v.x, y+v.y, z+v.z)); }
	dbl3  operator -(const dbl3& v) const  { return (dbl3(x-v.x, y-v.y, z-v.z)); }
	dbl3  operator *(double t) const  { return (dbl3(x*t, y*t, z*t)); }
	dbl3  operator /(double t) const  { double f = 1.0 / t;  return (dbl3(x*f, y*f, z*f)); }
	double operator &(const dbl3& v) const  { return (x*v.x+y*v.y+z*v.z); }
	dbl3  operator ^(const dbl3& v) const  { return (dbl3(y*v.z-z*v.y,  z*v.x-x*v.z,  x*v.y-y*v.x)); }
	dbl3  operator *(const dbl3& v) const  { return (dbl3(x*v.x, y*v.y, z*v.z)); }
	dbl3  operator /(const dbl3& v) const  { return (dbl3(x/v.x, y/v.y, z/v.z)); }
	bool operator ==(const dbl3& v) const  { return ((x-v.x)*(x-v.x) < verySmall) && ((y-v.y)*(y-v.y) < verySmall) && ((z-v.z)*(z-v.z) < verySmall); }
	bool operator !=(const dbl3& v) const  { return ((x-v.x)*(x-v.x) >= verySmall) || ((y-v.y)*(y-v.y) >= verySmall) || ((z-v.z)*(z-v.z) >= verySmall); }

};

inline dbl3 rotateAroundLine(dbl3 y, double gamma,  dbl3 n, dbl3 x)
{///. rotate y around line passing through x, in the direction of n, http://inside.mines.edu/~gmurray/ArbitraryAxisRotation
	double s = sinf(gamma),   c = cosf(gamma);
	double k = 1.0 - c;
	return dbl3(
	 	( x.x*(n.y*n.y+n.z*n.z) - n.x*( x.y*n.y+x.z*n.z-n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.x*c + (-x.z*n.y+x.y*n.z-n.z*y.y+n.y*y.z )*s,
		( x.y*(n.x*n.x+n.z*n.z) - n.y*( x.x*n.x+x.z*n.z-n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.y*c + ( x.z*n.x-x.x*n.z+n.z*y.x-n.x*y.z )*s,
		( x.z*(n.x*n.x+n.y*n.y) - n.z*( x.x*n.x+x.y*n.y-n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.z*c + (-x.y*n.x+x.x*n.y-n.y*y.x+n.x*y.y )*s );
}
inline dbl3 rotateAroundVec(const dbl3 y, double gamma, dbl3 n)
{///. rotate y around n (line passing through centre, in the direction of n) http://inside.mines.edu/~gmurray/ArbitraryAxisRotation
	double s = sinf(gamma),   c = cosf(gamma);
	double k = 1.0 - c;
	return dbl3(
		(  - n.x*( -n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.x*c + (n.y*y.z-n.z*y.y)*s,
		(  - n.y*( -n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.y*c + (n.z*y.x-n.x*y.z)*s,
		(  - n.z*( -n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.z*c + (n.x*y.y-n.y*y.x)*s );
}



inline dbl3  operator*(double t, const dbl3& v) { return dbl3(t*v.x, t*v.y, t*v.z); }
inline double  mag(const dbl3& v) { return std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z); }
inline double  magSqr(const dbl3& v) { return (v.x*v.x+v.y*v.y+v.z*v.z); }
inline dbl3  norm(const dbl3& v)  { return  v/(mag(v)+1.0e-300); }

inline dbl3  operator*( int3 n, dbl3 x) { return dbl3(n[0]*x[0], n[1]*x[1], n[2]*x[2]);}
inline int3  operator-( int3 n, int3 x) { return int3{{n[0]-x[0], n[1]-x[1], n[2]-x[2]}};}
inline int3  operator*( int s, int3 n) { return int3{{s*n[0], s*n[1], s*n[2]}};}
inline int3  operator*( double s, int3 n) { return int3{{int(s*n[0]), int(s*n[1]), int(s*n[2])}};}
inline int3  operator/( int3 n, int s) { return int3{{n[0]/s, n[1]/s, n[2]/s}};}
inline int3& operator+=( int3& n, int3 x) { n[0]+=x[0]; n[1]+=x[1]; n[2]+=x[2]; return n;}



template<class T>
class piece
{
  protected:
	piece(): d(0), dn(0) {};
  public:

	piece(T* dd, int n): d(dd), dn(d+n)     {};
	piece(T* dd, T* de): d(dd), dn(de)      {};
	piece(const piece& p): d(p.d), dn(p.dn) {};//! note data hold by piece are not const unless piece is const itself
	piece(const std::vector<T>& vs): d(&vs[0]), dn(d+vs.size()) {};

	T* begin() const {return d;};
	T* end()   const {return dn;};
	const T& back()   const {return *(dn-1);};
	const T* cbegin() const {return d;};
	const T* cend()   const {return dn;};
	T* operator()()   const {return d;};
	//const T& operator[](int i) const {return d[i];};
	T& operator[](int i) const {return d[i];};
	//const T* operator()() const {return d;};
	size_t size() const {return dn-d;};




	piece& operator +=(const piece& v)  { for(auto& a:*this){ a += v[&a-d];};  return (*this); }
	piece& operator -=(const piece& v)  { for(auto& a:*this){ a -= v[&a-d];};  return (*this); }
	piece& operator *=(const piece& v)  { for(auto& a:*this){ a *= v[&a-d];};  return (*this); }
	piece& operator /=(const piece& v)  { for(auto& a:*this){ a /= v[&a-d];};  return (*this); }
	piece& operator +=(T v)             { for(auto& a:*this){ a += v;};  return (*this); }
	piece& operator -=(T v)             { for(auto& a:*this){ a -= v;};  return (*this); }
	piece& operator *=(double t)        { for(auto& a:*this){ a *= t;};  return (*this); }
	piece& operator *=(int t)           { for(auto& a:*this){ a *= t;};  return (*this); }
	piece& operator /=(double t)        { return (*this)*=(1.0/t); }
	T sum() const                       { T sm=0; for(auto a:*this){ sm += a;}  return sm; }


  //protected:
	T*  d;
	T*  dn;
};



template<class T>
class lazyvec: public piece<T>
{

 public:
	using piece<T>::d;
	using piece<T>::dn;

	lazyvec(): piece<T>(0,0) {};
	lazyvec(int siz): piece<T>(new T[siz],siz) {};
	lazyvec(size_t siz, T val): piece<T>(new T[siz],siz) {  std::fill(d, dn, val);}
	lazyvec(const lazyvec& v): piece<T>(new T[v.size()],v.size())  {  std::copy(v.d, v.dn, d); }
	lazyvec(const std::vector<T>& v): piece<T>(new T[v.size()],v.size())  {  std::copy(&v[0], &*v.end(), d); }
	lazyvec(lazyvec&& v): piece<T>(v.d,v.size())  {  v.d=0; v.dn=0; }
	lazyvec(const T* dd, int nn): piece<T>(new T[nn],nn)            { std::copy(dd, dd+nn, d); }
	lazyvec(const T* dd, const T* de): piece<T>(new T[de-dd],de-dd)    { std::copy(dd, de, d);}
	lazyvec(const T* dd, const T* de, T(*func)(const T&)): piece<T>(new T[de-dd],de-dd)    { std::transform(dd, de, d, func);}
	~lazyvec(){ if(d) delete[] d; /*(std::cout<<'~').flush();*/ };

	lazyvec(lazyvec& v, bool move): piece<T>(v.d,v.size())  {  v.d=0; v.dn=0; assert(move); } //same as above but enforced by move 

	void operator=(lazyvec&& v){ { if(d) delete[] d; }  d = v.d;  dn=v.dn; v.d=0; v.dn=0; /*(std::cout<<'$').flush();*/ };
	void operator=(const piece<T>& v){ { if(d) delete[] d; }  d = new T[v.size()]; std::copy(v.d, v.dn, d);  dn=d+v.size(); };
	void operator=(const lazyvec& v){ { if(d) delete[] d; }  d = new T[v.size()]; std::copy(v.d, v.dn, d);  dn=d+v.size();  /*(std::cout<<'&').flush();*/ };
	void operator=(const std::vector<T>& v){ { if(d) delete[] d; }  d = new T[v.size()]; std::copy(&v[0], &*v.end(), d);  dn=d+v.size(); };
	void operator=(const std::initializer_list<T>& v){ { if(d) delete[] d; }  d = new T[v.size()]; std::copy(v.begin(), v.end(), d);  dn=d+v.size(); };


	lazyvec& operator +=(const piece<T>& v)  { this->piece<T>::operator+=(v);  return (*this); }
	lazyvec& operator -=(const piece<T>& v)  { this->piece<T>::operator-=(v);   return (*this); }
	lazyvec& operator *=(const piece<T>& v)  { this->piece<T>::operator*=(v);   return (*this); }
	template<typename T2> lazyvec& operator *=(const piece<T2>& v)  { for(auto& a:*this){ a *= v[&a-d];};  return (*this); }
	lazyvec& operator /=(const piece<T>& v)  { for(auto& a:*this){ a /= v[&a-d];};  return (*this); }
	lazyvec& operator +=(T v)  { for(auto& a:*this){ a += v;};  return (*this); }
	lazyvec& operator -=(T v)  { for(auto& a:*this){ a -= v;};  return (*this); }
	lazyvec& operator *=(double t)             { for(auto& a:*this){ a *= t;};  return (*this); }
	lazyvec& operator *=(int t)             { for(auto& a:*this){ a *= t;};  return (*this); }
	lazyvec& operator /=(double t)  { return (*this)*=(1.0/t); }

	lazyvec  operator -(void) const  { lazyvec res(*this);  for(auto& a:res){ a = -a;};  return (*this);  }
	lazyvec  operator +(const piece<T>& v) const { lazyvec tmp(*this); return tmp+=v;  }
	lazyvec  operator -(const piece<T>& v) const { lazyvec tmp(*this); return tmp-=v;  }
	lazyvec  operator *(const piece<T>& v) const { lazyvec tmp(*this); tmp*=v;  return tmp;  }
	template<typename T2> lazyvec  operator *(const piece<T2>& v) const { lazyvec tmp(*this); return tmp*=v;  }
	lazyvec  operator /(const piece<T>& v) const { return lazyvec(*this)/=v;  }
	lazyvec  operator +(T t) const { lazyvec tmp(*this); tmp+=t;     return tmp; }
	lazyvec  operator -(T t) const { lazyvec tmp(*this); tmp-=t;     return tmp; }
	lazyvec  operator *(double t) const { lazyvec tmp(*this); tmp*=t;     return tmp; }
	lazyvec  operator *(int t) const { lazyvec tmp(*this); tmp*=t;     return tmp; }
	lazyvec  operator /(double t) const { lazyvec tmp(*this); tmp*=1.0/t; return tmp; }

	void resize(int nn)
	{ { if(d) delete[] d; }   if(nn) {d=new T[nn]; dn=d+nn;} else {d=0; dn=0;} }
	void resize(int nn,const T& val)
	{ { if(d) delete[] d; }   if(nn) {d=new T[nn]; dn=d+nn;  std::fill(d, dn, val); } else {d=0; dn=0;} }
	void pbak(T& vj)
	{	if(d)
		{ T* od=d;  d=new T[dn+1-od];
			std::copy(od, dn, d);// TODO: test
			dn=d+dn+1-od; *(dn-1)=vj;  delete[] od;  }
		else    { d=new T[1];   *d=vj;   dn=d+1; }
	}
};
template<class T>  lazyvec<T> operator -(T v, lazyvec<T> vs)  { lazyvec<T> tmp(vs); for(auto& a:tmp){ a =v-a;};  return tmp; }

inline 	dbl3 round(const dbl3& vec) 	{ 	dbl3 res(vec); 	res.x=round(vec.x); res.y=round(vec.y); res.z=round(vec.z); 	return res; 	}
template<class T> 	lazyvec<T> round(const lazyvec<T>& vec) 	{ 	lazyvec<T> res(vec); 	for(auto& v:res) v=round(v); 	return res; 	}

//! moves iss into oss then deletes iss, but efficiently
template<class T> void transferto(lazyvec<lazyvec<T> >& oss, size_t i, lazyvec<lazyvec<T> >&& iss, size_t j=0)  
{	auto* os = oss.d+i-1;	auto* is = iss.d+j-1;	while ((++is)<iss.dn) { (++os)->d=is->d; os->dn=is->dn; is->d=0;	}  /*iss.d=0;*/	}

using dbl2 = std::pair<double,double>;
using int2 = std::pair<int,int>;




#ifdef VMMLIB__VECTOR__HPP
 template< size_t M, typename T >
 Vctr<M,T>::Vctr(const dbl3& v3) 	{ array[ 0 ] = v3.x;	array[ 1 ] = v3.y;	array[ 2 ] = v3.z; 	}
#endif



#define toStr std::to_string
//template<typename T> std::string toStr(const T& n){  std::ostringstream stm;  stm<<n;  return stm.str();  }



//! static vector, ideally lazyvec, but testing needed
#if __cplusplus >= 201103L
 template<class T> using  vars = lazyvec<T>; // or std::vector<T>;
 using  dbls = vars<double>;
 using  floats = vars<float>;
 using  dbl3s = vars<dbl3>;
 using  ints = vars<int>;
 using  uchars = vars<unsigned char>;
#else
 #define vars  std::vector
 #define dbls  std::vector<double>
 #define floats  std::vector<float>
 #define dbl3s  std::vector<dbl3>
 #define ints  std::vector<int>
 #define uchars  std::vector<unsigned char>
#endif

//! lambda help
typedef double(*dblFunc)(const double&);
typedef float(*floatFunc)(const float&);


template<class T>
double sumdbl(const piece<T>& ps)  { double sm=1.0e-300; for(auto a:ps){ sm += a;}  return sm; }
template<class T>
double sumdbl(const piece<T>& ps, const piece<T>& ws)  { double sm=1.0e-300; const T* p = ps.d-1, *w=ws.d-1; while(++p<ps.dn){ sm += *w * *(++p);}  return sm; }
inline double sumdbl(const dbl3& ps, const dbl3& ws)  { return ps.x+ws.x + ps.y+ws.y + ps.z+ws.z; }
inline double sumdbl(const dbl3& ps)  { return ps.x+ ps.y+ ps.z; }
template<class T>//! Note: can be made more efficient if assuming non-zero vec
T sumvars(const piece<T>& ps, const piece<T>& ws)  { T sm; sm*=0.0; const T* p = ps.d-1, *w=ws.d-1; while(++p<ps.dn){ sm += *w * *(++p);}  return sm; }
//inline double sumdbl(const piece<double> ps, const piece<double> ws)  { double sm=1.0e-300; double* p = ps.d-1,  *w=ws.d-1; 
	 //while(++p<ps.dn){ sm += *w * *(++p);}  return sm; }






inline std::ostream& operator<< (std::ostream& out, const dbl3& node)
{
	std::ios_base::fmtflags flgs=out.flags();
	out.flags(std::ios::showpoint | std::ios::scientific);
	out << std::setprecision(5)  << node.x   <<" " << node.y   <<" " << node.z;
	out.flags(flgs);
	return out;
}
inline std::ofstream& operator<< (std::ofstream& out, const dbl3& node)
{
	std::ios_base::fmtflags flgs=out.flags();
	out.flags(std::ios::showpoint | std::ios::scientific);	
	out << std::setprecision(8)  << node.x   <<" " << node.y   <<" " << node.z;
	out.flags(flgs);
	return out;
}

inline std::ostream& operator<< (std::ostream& out, const int3& ijk)
{    out << ijk[0] <<" "<< ijk[1]<<" " << ijk[2];	return out;	}

inline std::istream& operator>> (std::istream& in, dbl3& node)
{	in >> node.x >> node.y >> node.z;	return in;	}

inline std::istream& operator>> (std::istream& in, int3& ijk)
{    in >> ijk[0] >> ijk[1] >> ijk[2];    return in;	}


template<typename T1, typename T2>
std::istream& operator>> (std::istream& in, std::pair<T1,T2>& interval)
{ 	in >> interval.first >> interval.second; 		return in; 	}

template<typename T1, typename T2>
inline std::ostream& operator<< (std::ostream& out, std::pair<T1,T2>& interval)
{ 	out  << interval.first<<" "<< interval.second; 	return out; 	}

template<typename Type>
inline std::vector<Type>& operator*= (std::vector<Type>& vs, double scale)
{ 	for(Type& v : vs) v*=scale; 		return vs; 	}

template<typename T>
std::ostream & operator << (std::ostream & out, const std::vector<T>& vec)
{
	if(vec.size() && vec.size()<10)  for (auto v : vec) out << v << '\t';
	else                             for (auto v : vec) out << v << '\n';
	return out;
}

template<typename T1, typename T2>
std::ostream & operator << (std::ostream & out, const std::map<T1,T2>& vec)
{ 	for (auto v : vec) out << v << '\n'; 	return out; 	}


template<typename T>
std::ostream & operator << (std::ostream & out, const piece<T>& vec)
{
	if(vec.size() && vec.size()<10 )  for (auto v : vec) out << v << '\t';
	else                             for (auto v : vec) out << v << '\n';
	return out;
}


template<class T> T min(const piece<T>& pis)  { return *std::min_element(pis.d,pis.dn); }
template<class T> T max(const piece<T>& pis)  { return *std::max_element(pis.d,pis.dn); }
template<class T> void transform(piece<T>& pis, float(*func)(float&))  { for(float& dr:pis) dr=func(dr); }
//template<class T> void transform(piece<T>& pis, float(*func)(float&))  { for(float& dr:pis) dr=func(dr); }
template<typename T> void transform(T* d, const T* dn, T(*func)(const T&))  { --d; while (++d<dn) *d=func(*d); }

//double (*func)(const double&) = ([](const double& d) {return d;})

template<typename T, template<typename ...> class C>
vars<vars<T> > transpose(const C<C<T> >& vecvec)
{
	if(!vecvec.size()) return vars<vars<T> >();
	vars<vars<T> > trans(vecvec[0].size(),vars<T>(vecvec.size()));
	for (size_t i=0; i<vecvec[0].size();++i)
		for (size_t j=0; j<vecvec.size();++j) trans[i][j] = vecvec[j][i] ;
	return trans;
}




#endif


