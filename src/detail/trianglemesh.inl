
namespace mesh
{

namespace trianglemesh
{

template<typename json_type>
json_type TriangleMesh::write_nodes()
{
	json_type json;
	
	//vertices coordinates
	int i = 0; 
	for (auto it = in_.pointlist; it != in_.pointlist + 2 * in_.numberofpoints; ++it)
	{
		if (i++ % 2 == 0)
		{
			json["x"].push_back(*it);
		}
		else
		{
			json["y"].push_back(*it);
		}
	}


    //vertices attributes
	if (in_.numberofpointattributes != 0)
    {   
		std::copy(
			in_.pointattributelist, 
			in_.pointattributelist + in_.numberofpoints, 
			std::back_inserter(json["pattrib"])
		);
    }
	

    //vertices markers
	if (in_.pointmarkerlist != 0)
    {   
		std::copy(
			in_.pointmarkerlist, 
			in_.pointmarkerlist + in_.numberofpoints, 
			std::back_inserter(json["pmarker"])
		);
	}

	return std::forward<json_type>(json);
};

template<typename json_type>
json_type TriangleMesh::write_edges()
{
	json_type json;
	
	std::copy(
		in_.edgelist, 
		in_.edgelist + 2 * in_.numberofedges, 
		std::back_inserter(json["enid"])
	);

	if (in_.edgemarkerlist != 0)
	{
		std::copy(
			in_.edgemarkerlist, 
			in_.edgemarkerlist + in_.numberofedges, 
			std::back_inserter(json["emarker"])
		);
	}

	return std::forward<json_type>(json);
}

template<typename json_type>
json_type TriangleMesh::write_elems()
{
	json_type json;
	
	std::copy(
		in_.trianglelist, 
		in_.trianglelist + in_.numberofcorners * in_.numberoftriangles, 
		std::back_inserter(json["elnid"])
	);

	if (in_.numberoftriangleattributes != 0)
	{
		std::copy(
			in_.triangleattributelist, 
			in_.triangleattributelist + in_.numberoftriangleattributes * in_.numberoftriangles, 
			std::back_inserter(json["eattrib"])
		);
	}

	return std::forward<json_type>(json);
}

} //namespace trianglemesh

} //namespace mesh
