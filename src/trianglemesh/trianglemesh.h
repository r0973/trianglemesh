#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H


#if defined(_MSC_VER)
//Microsoft
#if defined(TRIANGLEMESH_EXPORTS)
#define TRIANGLEMESH_DLL_API __declspec(dllexport) 
#else
#define TRIANGLEMESH_DLL_API __declspec(dllimport) 
#endif
#elif defined(__GNUC__)
//GCC
#if defined(TRIANGLEMESH_EXPORTS)
#define TRIANGLEMESH_DLL_API __attribute__((visibility("default")))
#else
#define TRIANGLEMESH_DLL_API
#endif
#else

#define TRIANGLEMESH_DLL_API
#pragma warning Unknown dynamic link import/export semantics.
#endif



#include <precompiled.h>


namespace trianglemesh
{

extern "C" {

//#ifndef REAL
//#define REAL double
//#endif
//
//#ifndef	ANSI_DECLARATORS
//#define	ANSI_DECLARATORS
//#endif
//#define VOID int

#include <triangle/triangle.h>

}
  
class TRIANGLEMESH_DLL_API TriangleMesh
{
protected:
	enum IO_Type {
		TINPUT = 0,
		TOUTPUT = 1,
		TBOTH = 2
	};

public:
    triangulateio in;
    triangulateio out;
    triangulateio vorout;
    
public:
    TriangleMesh()
    {
      init(in);
      init(out);
      init(vorout);
    }

public:
	virtual ~TriangleMesh()
    { 
      destroy(in,      TINPUT);
      destroy(out,    TOUTPUT);
      destroy(vorout, TOUTPUT);
    }

public:
	bool read_poly(const std::string& filename);
	bool read_poly(std::stringstream& PolyFile);
	bool read_mesh(const std::string& filename);

public:
	bool build_mesh(
		const std::string& path, 
		const std::string& delimiter, 
		const std::string& polyfilename, 
		const std::string& triswitches
	);

	bool build_mesh(
		const std::string& switches, 
		std::stringstream& poly_file
	);

public:
    bool refine_mesh(const std::string& triswitches);
	bool write_msh2(const std::string& file);

public:
	bool write_nodes(const std::string& nodefilename);
	bool write_edges(const std::string& edgefilename);
	bool write_elems(const std::string& elementsfilename);

public:
    template<typename json_type>
	json_type write_nodes();
	
	template<typename json_type>
	json_type write_edges();
	
	template<typename json_type>
	json_type write_elems();

public:
	bool write_msh2 (std::stringstream& outfile);
	bool write_nodes(std::stringstream& outfile);
	bool write_edges(std::stringstream& outfile);
	bool write_elems(std::stringstream& outfile);

public:
	bool mesh_syntax()
	{
		//syntax();
		return true;
	};

public:
	bool mesh_info()
	{
		//info();
		return true;
	};
	
protected:
    void init(triangulateio& t);
    void destroy(triangulateio& t, IO_Type);
    bool replace(triangulateio& in, triangulateio& out);

protected:    
    bool read_nodes(const std::string& filename);
    bool read_edges(const std::string& filename);
    bool read_ele(const std::string& filename); 
    
};//class TriangleMesh 


} //namespace trianglemesh

#include<detail/trianglemesh.inl>

#endif // __TRIANGLEMESH__
