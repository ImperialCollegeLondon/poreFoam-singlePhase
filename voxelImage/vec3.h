#ifndef VEC3_H
#define VEC3_H

#include <iomanip>
#include <sstream>
#include <array>
#include <cmath>
#include <cstring>

#ifndef verySmall
	#define verySmall  1.0e-31
#endif


//typedef  int Int;
typedef  long long Long;
typedef  std::array<int,3> int3;
//typedef  std::array<Int,3> int3;
typedef  std::array<int3,3> int3x3;


class vec3
{
public:

	double	x;
	double	y;
	double	z;


	vec3() {}
	vec3(double r, double s, double t)  { x = r;  y = s;  z = t; }
	//vec3(int3 n)  { x = n[0];  y = n[1];  z = n[2]; }
	vec3(int3 n)  { x = n[0];  y = n[1];  z = n[2]; }
	vec3(double* d)  { x = d[0];  y = d[1];  z = d[2]; }

	vec3& Set(double r, double s, double t)  { x = r;  y = s;  z = t;  return (*this); }

	double&       operator [](long k) {	return ((&x)[k]); }

	const double& operator [](long k) const  { return ((&x)[k]); }

	vec3& operator +=(const vec3& v)  { x += v.x;  y += v.y;  z += v.z;  return (*this); }
	vec3& operator -=(const vec3& v)  { x -= v.x;  y -= v.y;  z -= v.z;  return (*this); }
	vec3& operator *=(double t)  { x *= t;  y *= t;  z *= t;  return (*this); }
	vec3& operator /=(double t)  {  double f = 1.0 / t;  x *= f;  y *= f;  z *= f;  return (*this); }
	vec3& operator ^=(const vec3& v)  { double r, s;  r=y*v.z-z*v.y;  s=z*v.x-x*v.z;  z=x*v.y-y*v.x;  x=r; y=s; 	return (*this); }
	vec3& operator *=(const vec3& v)  { x *= v.x;  y *= v.y;  z *= v.z;  return (*this); }
	vec3  operator -(void) const  { return (vec3(-x, -y, -z)); }
	vec3  operator +(const vec3& v) const  { return (vec3(x+v.x, y+v.y, z+v.z)); }
	vec3  operator -(const vec3& v) const  { return (vec3(x-v.x, y-v.y, z-v.z)); }
	vec3  operator *(double t) const  { return (vec3(x*t, y*t, z*t)); }
	vec3  operator /(double t) const  { double f = 1.0 / t;  return (vec3(x*f, y*f, z*f)); }
	double operator &(const vec3& v) const  { return (x*v.x+y*v.y+z*v.z); }
	vec3  operator ^(const vec3& v) const  { return (vec3(y*v.z-z*v.y,  z*v.x-x*v.z,  x*v.y-y*v.x)); }
	vec3  operator *(const vec3& v) const  { return (vec3(x*v.x, y*v.y, z*v.z)); }
	vec3  operator /(const vec3& v) const  { return (vec3(x/v.x, y/v.y, z/v.z)); }
	//~ bool operator ==(const vec3& v) const  { return ((x == v.x) && (y == v.y) && (z == v.z)); }
	//~ bool operator !=(const vec3& v) const  { return ((x != v.x) || (y != v.y) || (z != v.z)); }
	bool operator ==(const vec3& v) const  { return ((x-v.x)*(x-v.x) < verySmall) && ((y-v.y)*(y-v.y) < verySmall) && ((z-v.z)*(z-v.z) < verySmall); }
	bool operator !=(const vec3& v) const  { return ((x-v.x)*(x-v.x) >= verySmall) || ((y-v.y)*(y-v.y) >= verySmall) || ((z-v.z)*(z-v.z) >= verySmall); }

};

inline vec3 rotateAroundLine(vec3 y, double gamma,  vec3 n, vec3 x)
{///. rotate y around line passing through x, in the direction of n, http://inside.mines.edu/~gmurray/ArbitraryAxisRotation
	double s = sinf(gamma),   c = cosf(gamma);
	double k = 1.0 - c;
	return vec3(
	 	( x.x*(n.y*n.y+n.z*n.z) - n.x*( x.y*n.y+x.z*n.z-n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.x*c + (-x.z*n.y+x.y*n.z-n.z*y.y+n.y*y.z )*s,
		( x.y*(n.x*n.x+n.z*n.z) - n.y*( x.x*n.x+x.z*n.z-n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.y*c + ( x.z*n.x-x.x*n.z+n.z*y.x-n.x*y.z )*s,
		( x.z*(n.x*n.x+n.y*n.y) - n.z*( x.x*n.x+x.y*n.y-n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.z*c + (-x.y*n.x+x.x*n.y-n.y*y.x+n.x*y.y )*s );
}
inline vec3 rotateAroundVec(const vec3 y, double gamma, vec3 n)
{///. rotate y around n (line passing through centre, in the direction of n) http://inside.mines.edu/~gmurray/ArbitraryAxisRotation
	double s = sinf(gamma),   c = cosf(gamma);
	double k = 1.0 - c;
	return vec3(
		(  - n.x*( -n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.x*c + (n.y*y.z-n.z*y.y)*s,
		(  - n.y*( -n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.y*c + (n.z*y.x-n.x*y.z)*s,
		(  - n.z*( -n.x*y.x- n.y*y.y-n.z*y.z ) )*k + y.z*c + (n.x*y.y-n.y*y.x)*s );
}



inline vec3 operator*(double t, const vec3& v) { return vec3(t*v.x, t*v.y, t*v.z); }

inline double mag(const vec3& v) { return std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z); }

inline double magSqr(const vec3& v) { return (v.x*v.x+v.y*v.y+v.z*v.z); }
inline vec3 norm(const vec3& v)  { return  v/(mag(v)+1.0e-300); }



inline vec3  operator*( int3 n, vec3 x) { return vec3(n[0]*x[0], n[1]*x[1], n[2]*x[2]);}
inline int3 operator-( int3 n, int3 x) { return int3{{n[0]-x[0], n[1]-x[1], n[2]-x[2]}};}
inline int3 operator*( int s, int3 n) { return int3{{s*n[0], s*n[1], s*n[2]}};}
inline int3 operator*( double s, int3 n) { return int3{{int(s*n[0]), int(s*n[1]), int(s*n[2])}};}
inline int3 operator/( int3 n, int s) { return int3{{n[0]/s, n[1]/s, n[2]/s}};}
//inline int3& operator+=( int3& n, int3 x) { n[0]+=x[0]; n[1]+=x[1]; n[2]+=x[2]; return n;}
inline int3& operator+=( int3& n, int3 x) { n[0]+=x[0]; n[1]+=x[1]; n[2]+=x[2]; return n;}






inline std::ostream& operator<< (std::ostream& out, const vec3& node)
{
	out.flags(std::ios::showpoint);
	out.flags(std::ios::scientific);
	out << std::setprecision(8)  << std::setw(19) << node.x  << std::setw(19) << node.y  << std::setw(19) << node.z;
	return out;
}

inline std::ostream& operator<< (std::ostream& out, const int3& ijk)
{
    out << ijk[0] <<" "<< ijk[1]<<" " << ijk[2];
	return out;
}
inline std::istream& operator>> (std::istream& in, vec3& node)
{
	in >> node.x >> node.y >> node.z;
	return in;
}
inline std::istream& operator>> (std::istream& in, int3& ijk)
{
    in >> ijk[0] >> ijk[1] >> ijk[2];
    return in;
}


#if cplusplus > 201103L
using dblpair = std::pair<double,double>;
#else
#define dblpair  std::pair<double,double>
#endif

template<typename T1, typename T2>
inline std::istream& operator>> (std::istream& in, std::pair<T1,T2>& interval)
{
	in >> interval.first >> interval.second;
	return in;
}

template<typename T1, typename T2>
inline std::ostream& operator<< (std::ostream& out, std::pair<T1,T2>& interval)
{
	out  << std::setw(15)<< interval.first <<std::setw(15)<< interval.second;
	return out;
}

template<typename Type>
inline std::vector<Type>& operator*= (std::vector<Type>& vs, double scale)
{
	for(Type& v : vs) v*=scale;
	return vs;
}

template<typename T>
std::ostream & operator << (std::ostream & outstream, const std::vector<T>& vec)
{
	if(vec.size() && vec.size()<10)  for (auto v : vec) outstream << v << ' ';
	else                             for (auto v : vec) outstream << v << '\n';
	return outstream;
}





#ifndef TOSTR
#define TOSTR
template<typename T> std::string toStr(const T& n){  std::ostringstream stm;  stm<<n;  return stm.str();  }
#endif



#if __cplusplus >= 201103L
 template<class T> using  svec = std::vector<T>;
#else
 #define svec  std::vector
#endif


template<class T>
class lazyvec
{
  public:

	lazyvec(){n=0; d=NULL;};
	lazyvec(int siz){ d = new T[siz]; n=siz;};  
	lazyvec(const lazyvec& v){ d = new T[v.n]; std::memcpy(d, v.d, v.n*sizeof(T));  n=v.n; };
	lazyvec(const T* dd, int nn){ d = new T[nn]; std::memcpy(d, dd, nn*sizeof(T)); n=nn;};
	~lazyvec(){ if(n) delete[] d; n=0;};

	lazyvec& operator=(const lazyvec& v){ { if(n) delete[] d; }  d = new T[v.n]; std::memcpy(d, v.d, v.n*sizeof(T));  n=v.n; };

	bool isThere(const T& neib) const
	{	for (int ii=0;ii<n;++ii)  if (d[ii]==neib)	return true;
		return false;
	};

	T* begin() const {return d;};
	T* end()   const {return d+n;};
	const T* cbegin() const {return d;};
	const T* cend()   const {return d+n;};
	T* operator()() {return d;};
	const T& operator[](int i) const {return d[i];};
	T& operator[](int i) {return d[i];};
	const T* operator()() const {return d;};
	const int& size() const {return n;};

   void resize(int nn)  { { if(n) delete[] d; }   d=new T[nn]; n=nn; }
   void pbak(T& vj)
   {	if(n>0) { T* od=d;  d=new T[n+1];  std::memcpy(d, od, n*sizeof(T)); d[n++]=vj;  delete[] od;  }
		else    { d=new T[1];   d[0]=vj;   n=1;  }
	}
private:
	T*  d;
	int n;
};


#endif


