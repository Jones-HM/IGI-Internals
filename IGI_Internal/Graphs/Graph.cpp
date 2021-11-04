#include "Graph.hpp"
#include "Logger.hpp"
#include "Utils/Utility.hpp"

using namespace IGI;

string Graph::GetMaterialType(int material_id) {
	if (material_id < 0 || material_id > MATERIAL_SIZE) return "INVALID MATERIAL";
	string node_material = game_materials[material_id];
	return node_material;
}


void Graph::DebugGraphInfo(bool node_info, bool link_info) {
	std::stringstream ss;

	LOG_INFO("Level_%d Graphs: %d", g_curr_level, g_level_graphs.size());

	for (auto& graph : g_level_graphs) {

		ss << "'Graph" << graph.graph_id << "' Max Nodes: " << graph.node_max << " Total Nodes: " << graph.GetNodes().size();
		LOG_FILE("%s", ss.str().c_str()); ss.str(std::string());

		for (auto& node : graph.GetNodes()) {
			if (node_info) {
				ss << "Node Id: " << node.NodeId() << " Material: '" << graph.GetMaterialType(node.NodeMaterial()) << "' Radius: " << node.NodeRadius() << " Gamma: " << node.NodeGamma() << " Criteria: " << node.NodeCriteria();
				LOG_FILE("%s", ss.str().c_str()); ss.str(std::string());
			}
		}

		for (auto& link : graph.GetLinks()) {
			if (link_info) {
				ss << "Node Link1: " << link.NodeLink1() << " Link2: " << link.NodeLink2() << " Type: " << link.NodeLinkType();
				LOG_FILE("%s", ss.str().c_str()); ss.str(std::string());
			}
		}
	}
}

void Graph::DOT_CompileGraph(string graph_file,string output_fmt,string layout_engine) {
	string compile_cmd = "dot " + graph_file + " -o " + graph_file + "." + output_fmt + " -T " + output_fmt + " -K " + layout_engine;
	g_Utility.ShellExec(compile_cmd);
}

void Graph::DOT_SaveGraphs(string node_shape,string node_color,bool remove_source) {
	std::stringstream ss;
	const string node_attrib = "node [shape=" + node_shape +",color=black,style=filled,fillcolor=" + node_color + "]";

	string graph_dir = g_Utility.GetModuleFolder() + "\\Graphs_DOT\\level" + std::to_string(g_curr_level);
	fs::create_directories(graph_dir);
	string graph_file;

	for (auto& graph : g_level_graphs) {

		//Skip Cutscene graphs.
		if (graph.GetNodes().size() <= 1) {
			LOG_INFO("Level_%d has Cutscene Area GraphId_%d", g_curr_level, graph.GraphId());
			continue;
		}

		string graph_name = "graph_" + std::to_string(graph.graph_id);
		graph_file = graph_dir + "\\" + graph_name + ".gv"; // DOT - Graph file GraphViz.

		ss << "digraph {\n" << node_attrib << std::endl;

		bool graph_heading = false;

		for (auto& link : graph.GetLinks()) {
			string node_name_1 = std::to_string(link.NodeLink1());
			string node_name_2 = std::to_string(link.NodeLink2());
			ss << node_name_1 << " -> " << node_name_2 << std::endl;
		}

		ss << "}" << std::endl;

		//Write DOT Graph file.
		string graph_dot_data = ss.str();
		WriteFileType(graph_file, binary_t(graph_dot_data.begin(), graph_dot_data.end()), BINARY_FILE);
		DOT_CompileGraph(graph_file);
		ss.str(std::string()); ss.clear();
		if (remove_source) g_Utility.ShellExec("del " + graph_dir + "\\" + "*.gv");
	}
}