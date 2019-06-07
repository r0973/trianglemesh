namespace trianglemesh
{

void TriangleMesh::init(triangulateio& t) noexcept(true)
{
	t.pointlist                  = static_cast<REAL*>(NULL);
	t.pointattributelist         = static_cast<REAL*>(NULL);
	t.pointmarkerlist            = static_cast<int* >(NULL);
	t.numberofpoints             = 0 ;
	t.numberofpointattributes    = 0 ;                                   

	t.trianglelist               = static_cast<int* >(NULL);
	t.triangleattributelist      = static_cast<REAL*>(NULL);
	t.trianglearealist           = static_cast<REAL*>(NULL);
	t.neighborlist               = static_cast<int* >(NULL);
	t.numberoftriangles          = 0;
	t.numberofcorners            = 0;
	t.numberoftriangleattributes = 0;
  
	t.segmentlist                = static_cast<int* >(NULL);
	t.segmentmarkerlist          = static_cast<int* >(NULL);
	t.numberofsegments           = 0;

	t.holelist                   = static_cast<REAL*>(NULL);
	t.numberofholes              = 0;

	t.regionlist                 = static_cast<REAL*>(NULL);
	t.numberofregions            = 0;

	t.edgelist                   = static_cast<int* >(NULL);
	t.edgemarkerlist             = static_cast<int* >(NULL);
	t.normlist                   = static_cast<REAL*>(NULL);
	t.numberofedges              = 0;
}


void TriangleMesh::destroy(triangulateio& t, IO_Type io_type) noexcept(true)
{
	std::free(t.pointlist            );
	std::free(t.pointattributelist   );
	std::free(t.pointmarkerlist      );
	std::free(t.trianglelist         );
	std::free(t.triangleattributelist);
	std::free(t.trianglearealist     );
	std::free(t.neighborlist         );
	std::free(t.segmentlist          );
	std::free(t.segmentmarkerlist    );
  
	if (io_type==TINPUT)
	{
		std::free(t.holelist  );
		std::free(t.regionlist);
    }
  
	std::free (t.edgelist      );
	std::free (t.edgemarkerlist);
	std::free (t.normlist      );
}


bool TriangleMesh::read_poly(const std::string& filename) noexcept(false)
{

	std::string str(filename + std::string(".poly"));
	
	std::ifstream PolyFile(str.c_str(),std::ios::in);
	
	if(!PolyFile.good())
	{
		throw std::invalid_argument("TriangleMesh::readpoly: .poly file was not found!");
	}

	std::ifstream *inFile = &PolyFile;
	
	int numberofpoints;
	int meshdim;
	int pointmarker;
	
	*inFile >> numberofpoints;
	*inFile >> meshdim;
	*inFile >> in.numberofpointattributes;
	*inFile >> pointmarker;

	if (meshdim != 2)
	{
		throw std::invalid_argument("TriangleMesh::readpoly: meshdim not equal to 2!");
	}

	if(numberofpoints == 0)
	{
		str = filename + std::string(".node");
		
		std::ifstream NodeFile(str.c_str(), std::ios::in);

		if (!NodeFile.good())
		{
			throw std::invalid_argument("TriangleMesh::readpoly: .node file was not found!");
		}

		inFile = &NodeFile;
	
		*inFile >> in.numberofpoints;
		*inFile >> meshdim;
		*inFile >> in.numberofpointattributes;
		*inFile >> pointmarker;
	}
	else
	{   
		in.numberofpoints = numberofpoints;
	}
	
	if (meshdim != 2)
	{
		throw std::invalid_argument("TriangleMesh::readpoly: meshdim not equal to 2!");
	}

	//Allocate arrays
	if(in.numberofpointattributes!=0)
	{
		in.pointattributelist = static_cast<REAL*>(
				std::malloc(
					in.numberofpoints * 
					in.numberofpointattributes * 
					sizeof(REAL)
				)
			);
	}

	if(pointmarker)
	{
		in.pointmarkerlist = static_cast<int*>(
				std::malloc(
					in.numberofpoints * 
					sizeof(int)
				)
			);
	}

	in.pointlist = static_cast<REAL*>(
			 std::malloc(
				 in.numberofpoints * 
				 2 * 
				 sizeof(REAL)
			)
		);
	
	 //Read arrays from .node file
	for(int i = 0, index; i < in.numberofpoints; ++i)
	{   
		*inFile >> index;
		
		for(int j = 0; j < 2; ++j)
		{
			*inFile >> in.pointlist[i*2+j]; 
		}

		for(int j = 0; j < in.numberofpointattributes; ++j)    
		{
			*inFile >> in.pointattributelist[i * in.numberofpointattributes + j];
		}
		
		if(pointmarker)
		{
			*inFile >> in.pointmarkerlist[i];
		}
	};
	
	//if number of points is equal to zero then read from .poly file
	if(numberofpoints == 0)
	{
		inFile = &PolyFile;
	}

	*inFile >> in.numberofsegments;
	
	int segmentmarker;
	
	*inFile >> segmentmarker;
	
	if(segmentmarker)
	{
		in.segmentmarkerlist = static_cast<int*>(
				std::malloc(
					in.numberofsegments * 
					sizeof(int)
				)
			);
	}

	in.segmentlist = static_cast<int*>(
			std::malloc(
				in.numberofsegments * 
				2 * 
				sizeof(int)
			)
		);
	
	for(int i = 0, index; i < in.numberofsegments; ++i)
	{   
		*inFile >> index;
		
		for(int j = 0; j < 2; ++j)
		{
			*inFile >> in.segmentlist[i * 2 + j];
		}
		
		if(segmentmarker)
		{
			*inFile >> in.segmentmarkerlist[i];
		}
	};
	
	*inFile >> in.numberofholes;
	
	if(in.numberofholes !=0 )
	{   
		in.holelist = static_cast<REAL*>(
				std::malloc(
					in.numberofholes * 
					2 * 
					sizeof(REAL)
				)
			);
		
		for(int i = 0, index; i < in.numberofholes; ++i)
		{   
			*inFile >> index;
			
			for(int j = 0; j < 2; ++j)
			{
				*inFile >> in.holelist[i * 2 + j];
			}
		}
	}
        
    return true;
}


bool TriangleMesh::read_poly(std::stringstream& PolyFile)
{
	if (!PolyFile.good())
	{
		throw std::invalid_argument("TriangleMesh::readpoly: .poly file was not found!");
	}

	//int numberofpoints;
	int meshdim;
	int pointmarker;

	PolyFile >> in.numberofpoints;
	PolyFile >> meshdim;
	PolyFile >> in.numberofpointattributes;
	PolyFile >> pointmarker;

	if (meshdim != 2)
	{
		throw std::invalid_argument("TriangleMesh::readpoly: meshdim not equal to 2!");
	}

	if (in.numberofpoints == 0)
	{
		throw std::invalid_argument("TriangleMesh::readpoly: .node file was not found!");
	}

	if (meshdim != 2)
	{
		throw std::invalid_argument("TriangleMesh::readpoly: meshdim not equal to 2!");
	}

	//Allocate arrays
	if (in.numberofpointattributes != 0)
	{
		in.pointattributelist = static_cast<REAL*>(
				std::malloc(
					in.numberofpoints * 
					in.numberofpointattributes * 
					sizeof(REAL)
				)
			);
	}

	if (pointmarker)
	{
		in.pointmarkerlist = static_cast<int*>(
				std::malloc(
					in.numberofpoints * 
					sizeof(int)
				)
			);
	}

	in.pointlist = static_cast<REAL*>(
			std::malloc(
				in.numberofpoints * 
				2 * 
				sizeof(REAL)
			)
		);

	//Read arrays from .node file
	for (int i = 0, index; i< in.numberofpoints; ++i)
	{
		PolyFile >> index;
		
		for (int j = 0; j < 2; ++j)
		{
			PolyFile >> in.pointlist[i * 2 + j];
		}

		for (int j = 0; j < in.numberofpointattributes; ++j)
		{
			PolyFile >> in.pointattributelist[i * in.numberofpointattributes + j];
		}
		
		if (pointmarker)
		{
			PolyFile >> in.pointmarkerlist[i];
		}
	};

	PolyFile >> in.numberofsegments;

	int segmentmarker;

	PolyFile >> segmentmarker;

	if (segmentmarker)
	{
		in.segmentmarkerlist = static_cast<int*>(
				std::malloc(
					in.numberofsegments * 
					sizeof(int)
				)
			);
	}

	in.segmentlist = static_cast<int*>(
			std::malloc(
				in.numberofsegments * 
				2 * 
				sizeof(int)
			)
		);

	for (int i = 0, index; i < in.numberofsegments; ++i)
	{
		PolyFile >> index;
	
		for (int j = 0; j < 2; ++j)
		{
			PolyFile >> in.segmentlist[i * 2 + j];
		}
	
		if (segmentmarker)
		{
			PolyFile >> in.segmentmarkerlist[i];
		}
	};

	PolyFile >> in.numberofholes;

	if (in.numberofholes != 0)
	{
		in.holelist = static_cast<REAL*>(
					std::malloc(
						in.numberofholes * 
						2 * 
						sizeof(REAL)
					)
				);
		
		for (int i = 0, index; i < in.numberofholes; ++i)
		{
			PolyFile >> index;

			for (int j = 0; j < 2; ++j)
			{
				PolyFile >> in.holelist[i * 2 + j];
			}
		}
	}

	return true;
}


bool TriangleMesh::build_mesh(
	const std::string& path, 
	const std::string& delimiter, 
	const std::string& polyfilename, 
	const std::string& switches
) noexcept(false)
{
	try
	{
		std::string filename    = path + delimiter + polyfilename;
		std::string triswitches = switches;

		read_poly(filename);
		refine_mesh(triswitches);
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(
				std::string(	
					std::string("TriangleMesh::build_mesh: ") +
					e.what()
				).c_str()
			);

	}

	return true;
}


bool TriangleMesh::build_mesh(
	const std::string& switches, 
	std::stringstream& poly_file
) noexcept(false)
{
	try
	{
		read_poly(poly_file);
		refine_mesh(switches);
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(
				std::string(	
					std::string("TriangleMesh::build_mesh: ") +
					e.what()
				).c_str()
			);

	}

	return true;
}


bool TriangleMesh::refine_mesh(const std::string& triswitches)
{
	try
	{   
	   	destroy(   out, TOUTPUT);
		destroy(vorout, TOUTPUT);

		init(   out);
		init(vorout);

		triangulate(
				const_cast<char*>(triswitches.c_str()),
				&in,
				&out,
				&vorout
				); 

		replace(in,out);
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(
				std::string(	
					std::string("TriangleMesh::refine_mesh: ") +
					e.what()
				).c_str()
			);

	}

	return true;
    
}


bool TriangleMesh::replace(triangulateio& in, triangulateio& out)
{
	destroy(in,TINPUT);
	init(in);
  
	in.numberofpoints          = out.numberofpoints;
	in.numberofpointattributes = out.numberofpointattributes; 
  
	if(in.numberofpoints != 0)
	{
		in.pointlist = static_cast<REAL*>(
					std::malloc(
						in.numberofpoints * 
						2 * 
						sizeof(REAL)
					)
				);
	}
  
	if(in.numberofpointattributes != 0)
	{
		in.pointattributelist = static_cast<REAL*>(
				std::malloc(
					in.numberofpointattributes * 
					sizeof(REAL)
				)
			);
	}
  
	if(out.pointmarkerlist != 0)
	{
		in.pointmarkerlist = static_cast< int*>(
				std::malloc(
					in.numberofpoints * 
					sizeof(int)
				)
			);
	}

	for(int i = 0; i < in.numberofpoints; ++i)
	{   
		for(int j = 0; j < 2; ++j)
		{
			in.pointlist[i * 2 + j] = out.pointlist[i * 2 + j];
		}
	  
		for(int j = 0; j < in.numberofpointattributes; ++j)
		{
		     in.pointattributelist[i*in.numberofpointattributes+j] = 
			out.pointattributelist[i*in.numberofpointattributes+j]; 
		}
	  
		if(out.pointmarkerlist != 0)
		{
			in.pointmarkerlist[i] = out.pointmarkerlist[i];
		}
	};

	in.numberoftriangles          =  out.numberoftriangles;
	in.numberofcorners            =  out.numberofcorners;
	in.numberoftriangleattributes =  out.numberoftriangleattributes;
	  
	if(in.numberoftriangles != 0)
	{
		in.trianglelist = static_cast<int*>(
			std::malloc(
				in.numberoftriangles * 
				in.numberofcorners * 
				sizeof(int)
			)
		);
	}
  
	if(in.numberoftriangleattributes != 0)
	{
		in.triangleattributelist = static_cast<REAL*>(
				std::malloc(
					in.numberoftriangleattributes * 
					in.numberoftriangles * 
					sizeof(int)
				)
			);
	}
  
	if(out.trianglearealist != 0)
	{
		in.trianglearealist = static_cast<REAL*>(
			std::malloc(
				in.numberoftriangles *
				sizeof(REAL)
			)
		);
	}

	for(int i = 0; i < in.numberoftriangles; ++i)
	{	
		for(int j = 0; j < in.numberofcorners; ++j)
		{
			 in.trianglelist[i*in.numberofcorners+j] = 
			out.trianglelist[i*in.numberofcorners+j];
		}
			
		for(int j = 0; j < in.numberoftriangleattributes; ++j)
		{	
			 in.triangleattributelist[i*in.numberoftriangleattributes+j] = 
			out.triangleattributelist[i*in.numberoftriangleattributes+j]; 
		}
	};

	if(out.segmentlist != 0)
	{	
		in.numberofsegments = out.numberofsegments;

		if (in.numberofsegments != 0)
		{	
			//allocate segmentlist
			in.segmentlist = static_cast<int*>(
						std::malloc(
							in.numberofsegments * 
							2 * 
							sizeof(int)
						)
					);
			
			if(!in.segmentlist)
			{
				throw std::runtime_error("TriangleMesh::replace: segmentlist was not allocated properly");
			}

			//copy segmentlist
			for (int i = 0; i < in.numberofsegments; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					in.segmentlist[i * 2 + j] = out.segmentlist[i * 2 + j];
				}
			};

		};

		if (out.segmentmarkerlist != 0)
		{   
			//allocate segment markerlist
			in.segmentmarkerlist = static_cast<int*>(
					std::malloc(
						in.numberofsegments * 
						sizeof(int)
					)
				);

			if (!in.segmentmarkerlist)
			{
				throw std::runtime_error("TriangleMesh::replace: segmentmarkerlist was not allocated properly");
			}

			//copy segment marker list
			for (int i = 0; i < in.numberofsegments; ++i)
			{
				if (out.segmentmarkerlist != 0)
				{
					in.segmentmarkerlist[i] = out.segmentmarkerlist[i];
				}
			};
		};
	};
  
	if (out.numberofedges != 0)
	{
		in.numberofedges = out.numberofedges;

		//copy edgelist
		if (out.edgelist != 0)
		{
			in.edgelist = static_cast<int *>(
					std::malloc(
						in.numberofedges * 
						2 * 
						sizeof(int)
					)
				);

			if (!in.edgelist)
			{
				throw std::runtime_error("TriangleMesh::replace: edgelist was not allocated properly");
			}

			for (int i = 0; i < in.numberofedges; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					in.edgelist[i * 2 + j] = out.edgelist[i * 2 + j];
				}
			}
		}

		//copy edgemarkerlist
		if (out.edgemarkerlist != 0)
		{
			in.edgemarkerlist = static_cast<int *>(
					std::malloc(
						in.numberofedges * 
						sizeof(int)
					)
				);

			if (!in.edgemarkerlist)
			{
				throw std::runtime_error("TriangleMesh::replace: edgemarkerlist was not allocated properly");
			}

			for (int i = 0; i < in.numberofedges; ++i)
			{
				in.edgemarkerlist[i] = out.edgemarkerlist[i];
			}
		}
	};

	return true;
}


bool TriangleMesh::write_msh2(const std::string& filename)
{
	std::ofstream out(filename.c_str());

	if (!out.good())
	{
		throw std::runtime_error(
				"TriangleMesh::write_msh2: Error: " + 
				filename + 
				" cannot be opened for writing!"
			);
	}

	out.setf(std::ios::scientific);
	out.precision(16);

	out << "$MeshFormat"      << "\n"
		<< "2.2 0 8"          << "\n"
		<< "$EndMeshFormat"   << "\n"
		<< "$Nodes"           << "\n" 
		<< in.numberofpoints  << "\n";
	
	//write arrays from .node file
	for (unsigned int i = 0; 
					  i < static_cast<unsigned int>(in.numberofpoints); 
					  ++i
		)
	{
		out << i + 1 << " ";
	
		for (unsigned int j = 0; j<2; ++j)
		{
			out << in.pointlist[i * 2 + j] << " ";
		}
	
		//z coordinate
		out << 0 << "\n";
	};
	
	const unsigned int n_all_elements = //in.numberofpoints    +
										//in.numberofsegments  +
										in.numberoftriangles;
	
	out << "$EndNodes"    << "\n" 
		<< "$Elements"    << "\n" 
		<< n_all_elements << "\n";

	unsigned int serial_number = 0;

	for (unsigned int i = 0; 
					  i < static_cast<unsigned int>(in.numberoftriangles); 
					  ++i, 
					  ++serial_number
		)
	{
		out << serial_number + 1 << " " /* serial number of element */
			<< 2 << " "  /* type of element suitable for Gmsh */
			<< 2 << " "  /* the number of tags */
			<< 1 << " "  /* physical domain */
			<< 9 << " "; /* elemetary domain - let it be the same */

		//for(int i=0; i< in.numberoftriangles; ++i)
		{   
			for(int j=0; j<in.numberofcorners; ++j)
			{
				out << in.trianglelist[i*in.numberofcorners+j]<< " ";
			}
			
			out << "\n";
		};
	}
	 
	//write_elements(out, points, serial_number);
	//write_elements(out, lines, serial_number);
	//write_elements(out, triangles, serial_number);
	//write_elements(out, tetrahedra, serial_number);
	//write_elements(out, quadrangles, serial_number);
	//write_elements(out, hexahedra, serial_number);

	out << "$EndElements\n";

	out.close();

	return true;
}


bool TriangleMesh::write_nodes(const std::string& nodefilename)
{
	std::string str(nodefilename + ".node");
	std::ofstream outfile(str.c_str());
	
	if (!outfile.good())
	{
		throw std::runtime_error(str + " cannot be opened for writing!");
	}

	outfile.setf(std::ios::scientific);
	outfile.precision(16);

	// Number of vertices, 
	// number of dimensions, 
	// number of vertex attributes, 
	// and number of boundary markers (zero or one)
	outfile << in.numberofpoints << " " 
			<< 2 << " " 
			<< in.numberofpointattributes << " " 
			<< ((in.pointmarkerlist != 0) ? "1" : "0") 
			<< "\n";

	for (int i = 0, l = 1; i < in.numberofpoints; ++i, ++l)
	{
		outfile << l << " ";
		
		// X and y coordinates
		for (int j = 0; j < 2; ++j)
		{
			outfile << in.pointlist[i * 2 + j] << "   ";
		}
		
		// point attributes
		if (in.numberofpointattributes != 0)
		{
			outfile << in.pointattributelist[i] << "   ";
		}
		
		// point markers
		if (in.pointmarkerlist != 0)
		{
			outfile << in.pointmarkerlist[i];
		}

		outfile << "\n";
	}

	outfile.close();

	return true;
}


bool TriangleMesh::write_edges(const std::string& edgefilename)
{
	std::string str(edgefilename + ".edge");
	std::ofstream outfile(str.c_str());
	
	if (!outfile.good())
	{
		throw std::runtime_error(str + " cannot be opened for writing!");
	}

	if (!in.edgelist)
	{
		outfile << "edges list is empty, "
				<< "because switch for edges generation was not defined."
				<< " Try to add ""-e"" switch to generate an edge list";

		return false;
	}

	//number of edges, number of boundary markers (zero or one)
	outfile << in.numberofedges << " " 
			<< ((in.edgemarkerlist != 0) ? "1" : "0") 
			<< "\n";
	
	for (int i = 0, l = 1; i < in.numberofedges; ++i, ++l)
	{
		outfile << l << " ";
		
		for (int j = 0; j < 2; ++j)
		{
			outfile << in.edgelist[i * 2 + j] << " ";
		}

		if (in.edgemarkerlist != 0)
		{
			outfile << in.edgemarkerlist[i];
		}

		outfile << "\n";
	}

	outfile.close();

	return true;
}


bool TriangleMesh::write_elems(const std::string& elementsfilename)
{
	std::string str(elementsfilename + ".ele");
	std::ofstream outfile(str.c_str());

	if (!outfile.good())
	{
		throw std::runtime_error(str + " cannot be opened for writing!");
	}

	//number of triangles, vertices per triangle, attributes per triangle
	outfile << in.numberoftriangles << " " 
			<< in.numberofcorners   << " " 
			<< ((in.numberoftriangleattributes != 0) ? "1" : "0") 
			<< "\n";

	for (int i = 0, l = 1; i < in.numberoftriangles; ++i, ++l)
	{
		outfile << l << " ";
		
		for (int j = 0; j < in.numberofcorners; ++j)
		{
			outfile << in.trianglelist[i * in.numberofcorners + j] << " ";
		}
		for (int j = 0; j < in.numberoftriangleattributes; ++j)
		{
			outfile << in.triangleattributelist[i * in.numberoftriangleattributes + j] << " ";
		}
		
		outfile << "\n";
	};

	outfile.close();

	return true;
}


bool TriangleMesh::read_mesh(const std::string& filename)
{
        if(!read_nodes(filename))
		{
			return false;
		}

//      if(!readedges(filename))
//      {
//			return false;
//		}

		if(!read_ele(filename))
		{
			return false;
		}

        return true;
}


bool TriangleMesh::read_nodes(const std::string& filename)
{
        std::string str;

        str = filename + std::string(".node");
        std::ifstream NodeFile(str.c_str(), std::ios::in);
        
		if (!NodeFile.good())
		{	
			throw std::runtime_error("TriangleMesh::read_mesh: file was not found or could not be read!");
		}

        NodeFile >> in.numberofpoints;
        
        int meshdim;
        
		NodeFile >> meshdim;
        
		if(meshdim != 2)
        {    
			return false;
        }
        
        NodeFile >> in.numberofpointattributes;
        
        int pointmarker;

        NodeFile >> pointmarker;
        
        in.pointlist = static_cast<REAL*>(
				std::malloc(
					in.numberofpoints * 
					2 * 
					sizeof(REAL)
				)
			);
        
        if(in.numberofpointattributes!=0)
		{
			in.pointattributelist = static_cast<REAL*>(
					std::malloc(
						in.numberofpoints * 
						in.numberofpointattributes * 
						sizeof(REAL)
					)
				);
		}

        if(pointmarker)
		{
			in.pointmarkerlist = static_cast<int*>(
					std::malloc(
						in.numberofpoints * 
						sizeof(int)
					)
				);
		}
        
        for(int i = 0, index; i < in.numberofpoints; ++i)
        {   
			NodeFile >> index;
            
			for(int j = 0; j < 2; ++j)
			{
				NodeFile >> in.pointlist[i * 2 + j]; 
			}
			
			//if(pointattribute)
            for(int j = 0; j < in.numberofpointattributes; ++j)    
			{
				NodeFile >> in.pointattributelist[i * in.numberofpointattributes + j];
			}
			
			if(pointmarker)
			{
				NodeFile >> in.pointmarkerlist[i];
			}
        };

		NodeFile.close();

        return true;
}


bool TriangleMesh::read_edges(const std::string& filename)
{
        std::string str;

		str = filename + std::string(".edge");
		
		std::ifstream EdgeFile(str.c_str(), std::ios::in);
        
        EdgeFile >> in.numberofedges;
        
        int edgemarker;
        
		EdgeFile >> edgemarker;
        
        in.edgelist = static_cast<int*>(
				std::malloc(
					in.numberofedges * 
					2 * 
					sizeof(int)
				)
			);
        
        if(edgemarker)
		{
			in.edgemarkerlist = static_cast<int*>(
					std::malloc(
						in.numberofedges * 
						sizeof(int)
					)
				);
		}

        for(int i = 0, index; i < in.numberofedges; ++i)
        {   
			EdgeFile >> index;
            
			for(int j = 0; j < 2; ++j)
			{
				EdgeFile >> in.edgelist[i * 2 + j]; 
			}
			
			if(edgemarker)
			{
				EdgeFile >> in.edgemarkerlist[i];
			}
		};

		EdgeFile.close();

        return true;
    
}


bool TriangleMesh::read_ele(const std::string& filename)
{
        std::string str;

        str = filename + std::string(".ele");
        
		std::ifstream EleFile(str.c_str(),std::ios::in);
        
        EleFile >> in.numberoftriangles;
        EleFile >> in.numberofcorners;
        
        EleFile >> in.numberoftriangleattributes;
        
        in.trianglelist = static_cast<int*>(
				std::malloc(
					in.numberoftriangles * 
					in.numberofcorners * 
					sizeof(int)
				)
			);
        
        if(in.numberoftriangleattributes!=0)
		{
			in.triangleattributelist = static_cast<REAL*>(
					std::malloc(
						in.numberofpoints * 
						in.numberoftriangleattributes * 
						sizeof(REAL)
					)
				);
		}

        for(int i = 0, index; i < in.numberoftriangles; ++i)
        {   
			EleFile >> index;
            
			for(int j = 0; j < in.numberofcorners; ++j)
			{
				EleFile >> in.trianglelist[i * in.numberofcorners + j]; 
			}
			for(int j = 0; j < in.numberoftriangleattributes; ++j)
			{
				EleFile >> in.triangleattributelist[i * in.numberoftriangleattributes + j];
			}
		}

		EleFile.close();

        return true;
}


bool TriangleMesh::write_msh2(std::stringstream& outfile)
{
	if (!outfile.good())
	{
		throw std::runtime_error("TriangleMesh::write_msh2: stream cannot be opened for writing!");
	}

	outfile.setf(std::ios::scientific);
	outfile.precision(16);

	outfile << "$MeshFormat"      << "\n" 
			<< "2.2 0 8"          << "\n"
			<<"$EndMeshFormat"    << "\n"
			<< "$Nodes"           << "\n" 
			<< in.numberofpoints << "\n";
	
	//Read arrays from .node file
	for (unsigned int i = 0; 
					  i < static_cast<unsigned int>(in.numberofpoints); 
					  ++i
		)
	{
		outfile << i + 1 << " ";
	
		for (unsigned int j = 0; j < 2; ++j)
		{
			outfile << in.pointlist[i * 2 + j] << " ";
		}
	
		//z coordinate
		outfile << 0 << "\n";
	};

	const unsigned int n_all_elements = //in.numberofpoints    +
										//in.numberofsegments  +
										in.numberoftriangles;

	outfile << "$EndNodes"    << "\n"
			<< "$Elements"    << "\n" 
			<< n_all_elements << "\n";

	unsigned int serial_number = 0;

	for (unsigned int i = 0; 
					  i < static_cast<unsigned int>(in.numberoftriangles); 
					  ++i, 
					  ++serial_number
		)
	{
		outfile << serial_number + 1 << " " /* serial number of element */
				<< 2 << " "  /* type of element suitable for Gmsh */
				<< 2 << " "  /* the number of tags */
				<< 1 << " "  /* physical domain */
				<< 9 << " "; /* elemetary domain - let it be the same */

		//for(int i=0; i< in.numberoftriangles; ++i)
		{   
			for (int j = 0; j < in.numberofcorners; ++j)
			{
				outfile << in.trianglelist[i * in.numberofcorners + j] << " ";
			}
			
			outfile << "\n";
		};

	}

	outfile << "$EndElements\n";
	
	return true;
}


bool TriangleMesh::write_nodes(std::stringstream& outfile)
{
	if (!outfile.good())
	{
		throw std::runtime_error("TriangleMesh::write_nodes: stream cannot be opened for writing!");
	}

	outfile.setf(std::ios::scientific);
	outfile.precision(16);

	// Number of vertices, 
	// number of dimensions, 
	// number of vertex attributes, 
	// and number of boundary markers (zero or one)
	outfile << in.numberofpoints << " " 
			<< 2 << " " 
			<< in.numberofpointattributes << " " 
			<< ((in.pointmarkerlist != 0) ? "1" : "0") 
			<< "\n";

	for (int i = 0, l = 1; i < in.numberofpoints; ++i, ++l)
	{
		outfile << l << " ";
		
		// X and y coordinates
		for (int j = 0; j < 2; ++j)
		{
			outfile << in.pointlist[i * 2 + j] << "   ";
		}
		
		// Vertex attributes
		if (in.numberofpointattributes != 0)
		{
			outfile << in.pointattributelist[i] << "   ";
		}
		
		// Vertex markers
		if (in.pointmarkerlist != 0)
		{
			outfile << in.pointmarkerlist[i];
		}

		outfile << "\n";
	}

	return true;
}


bool TriangleMesh::write_edges(std::stringstream& outfile)
{
	if (!outfile.good())
	{
		throw std::runtime_error("TriangleMesh::write_edges: stream cannot be opened for writing!");
	}

	if (!in.edgelist)
	{
		outfile << "edges list is empty, "
				<< "because switch for edges generation was not defined. "
				<< "Try to add ""-e"" switch to generate edge list";

		return false;
	}

	// Number of edges, number of boundary markers (zero or one)
	outfile << in.numberofedges << " " 
			<< ((in.edgemarkerlist != 0) ? "1" : "0") 
			<< "\n";

	for (int i = 0, l = 1; i < in.numberofedges; ++i, ++l)
	{
		outfile << l << " ";
		
		for (int j = 0; j < 2; ++j)
		{
			outfile << in.edgelist[i * 2 + j] << " ";
		}

		if (in.edgemarkerlist != 0)
		{
			outfile << in.edgemarkerlist[i];
		}

		outfile << "\n";
	}

	return true;
}

bool TriangleMesh::write_elems(std::stringstream& outfile)
{
	if (!outfile.good())
	{
		throw std::runtime_error("TriangleMesh::write_elems cannot be opened for writing!");
	}

	// Number of triangles, vertices per triangle, attributes per triangle
	outfile << in.numberoftriangles << " " 
			<< in.numberofcorners   << " " 
			<< ((in.numberoftriangleattributes != 0) ? "1" : "0") 
			<< "\n";

	for (int i = 0, l = 1; i < in.numberoftriangles; ++i, ++l)
	{
		outfile << l << " ";
		
		for (int j = 0; j < in.numberofcorners; ++j)
		{
			outfile << in.trianglelist[i * in.numberofcorners + j] << " ";
		}
		
		for (int j = 0; j < in.numberoftriangleattributes; ++j)
		{
			outfile << in.triangleattributelist[i * in.numberoftriangleattributes + j] 
					<< " ";
		}
		
		outfile << "\n";
	};

	return true;
}


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
