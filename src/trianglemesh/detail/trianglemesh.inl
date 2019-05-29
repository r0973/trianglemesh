namespace trianglemesh
{

template<typename json_type>
json_type TriangleMesh::write_nodes()
{
	json_type json;
	
	//vertices coordinates
	int i = 0; 
	for (auto it  = in.pointlist; 
			  it != in.pointlist + 2 * in.numberofpoints; 
			  ++it
		)
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
	if (in.numberofpointattributes != 0)
    {   
		std::copy(
			in.pointattributelist, 
			in.pointattributelist + in.numberofpoints, 
			std::back_inserter(json["pattrib"])
		);
    }
	

    //vertices markers
	if (in.pointmarkerlist != 0)
    {   
		std::copy(
			in.pointmarkerlist, 
			in.pointmarkerlist + in.numberofpoints, 
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
		in.edgelist, 
		in.edgelist + 2 * in.numberofedges, 
		std::back_inserter(json["enid"])
	);

	if (in.edgemarkerlist != 0)
	{
		std::copy(
			in.edgemarkerlist, 
			in.edgemarkerlist + in.numberofedges, 
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
		in.trianglelist, 
		in.trianglelist + in.numberofcorners * in.numberoftriangles, 
		std::back_inserter(json["elnid"])
	);

	if (in.numberoftriangleattributes != 0)
	{
		std::copy(
			in.triangleattributelist, 
			in.triangleattributelist + in.numberoftriangleattributes * in.numberoftriangles, 
			std::back_inserter(json["eattrib"])
		);
	}

	return std::forward<json_type>(json);
}

} //namespace trianglemesh
