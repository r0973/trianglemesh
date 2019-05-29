#include <precompiled.h>
#include <trianglemesh.h>


int main(void) {

	std::cout << "hello world!" << std::endl;

	using triangle_mesh_type     = trianglemesh::TriangleMesh;
	using triangle_mesh_ptr_type = std::unique_ptr<triangle_mesh_type>;

	triangle_mesh_ptr_type trimesh;

	std::stringstream poly_file;

	

	trimesh->build_mesh(
		std::string("-p"),
		poly_file
	);

	return 0;
}
