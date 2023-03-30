#include <precompiled.h>
#include <trianglemesh.h>

/*
This is an example of box.poly file with PSLG for box with hole
The example shows how to create a *.poly file in stringstream object and then to
build the mesh of triangles with the help of trianglemesh class

//box.poly

# A box with eight points in 2D, no attributes, one boundary marker.
8 2 0 1
# Outer box has these vertices:
 1   0 0   0
 2   0 3   0
 3   3 0   0
 4   3 3   33     # A special marker for this point.
# Inner square has these vertices:
 5   1 1   0
 6   1 2   0
 7   2 1   0
 8   2 2   0
# Five segments with boundary markers.
5 1
 1   1 2   5      # Left side of outer box.
 2   5 7   0      # These four segments enclose the hole.
 3   7 8   0
 4   8 6   10
 5   6 5   0
# One hole in the middle of the inner square.
1
 1   1.5 1.5
*/

struct point { double x; double y; int marker; };
struct edge { int n0; int n1; int marker; };

int main(void) {

	using triangle_mesh_type     = trianglemesh::TriangleMesh;
	using triangle_mesh_ptr_type = std::unique_ptr<triangle_mesh_type>;

	//Define box points coordinates and markers
	std::vector<point> points = {
	//# Outer box has these vertices:
		{0., 0., 0}, 
		{0., 3., 0}, 
		{3., 0., 0}, 
		{3., 3.,33}, //A special marker for this point 
	//# Inner square has these vertices:
 		{1., 1., 0},
        {1., 2., 0},
 		{2., 1., 0},
        {2., 2., 0}
	}; 

	//Define box point segments
	std::vector<edge> edges = {
	//# Five segments with boundary markers.
		//# Left side of outer box
		{1, 2, 5},
		{2, 4, 5},
		{4, 3, 5},
		{1, 3, 5},
		//# These four segments enclose the hole.
		{5, 7, 0},
	    {7, 8, 0},
	    {8, 6,10},
		{6, 5, 0}
	};

	//define hole
	std::vector<point> holes = {
		{1.5,1.5,0}
	};

	//construct trianglemesh PSLG graph;
	std::stringstream poly_file;

	poly_file << points.size() << " 2 0 1" << "\n";

	//write points
	for (std::size_t i = {0}; i < points.size(); ++i) {
		const auto& p {points[i]};
		poly_file << i+1 << " " 
				  << p.x << " " 
				  << p.y << " "
				  << p.marker << "\n";  
	};

	//write segments
	poly_file << edges.size() << " 1" << "\n";

	for (std::size_t i = {0}; i < edges.size(); ++i) {
		const auto& e {edges[i]};
		poly_file << i+1  << " "
			      << e.n0 << " "
				  << e.n1 << " "
				  << e.marker << "\n";
	};

	//write holes
	poly_file << holes.size() << "\n";
	for (std::size_t i = {0}; i < holes.size(); ++i) {
		const auto& h {holes[i]};
		poly_file << i   << " " 
		          << h.x << " "
				  << h.y << "\n";
	};

	std::cout << poly_file.str() << "\n";

	//make ptr
	auto trimesh = std::make_unique<triangle_mesh_type>();

	//build mesh
	trimesh->build_mesh(
		std::string("-pq"),
		poly_file
	);

	//save to msh format file
	trimesh->write_msh2("box.msh");

	return 0;
}
