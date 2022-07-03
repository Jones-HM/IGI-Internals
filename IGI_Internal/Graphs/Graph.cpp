#include "Graph.hpp"
#include "Logger.hpp"
#include "Utils/Utility.hpp"

using namespace IGI;

Graph Graph::GetGraph4mId(graph_t graph_id) {
	for (auto& graph : g_level_graphs) {
		if (graph.GraphId() == graph_id) {
			LOG_FILE("%s Found graph for Id: %d", FUNC_NAME, graph_id);
			return graph;
		}
	}
	return {};
}

Graph::Node Graph::GetNode4mId(graph_t graph_id, node_t node_id) {
	for (auto& graph : g_level_graphs) {
		if (graph.GraphId() == graph_id) {
			for (auto& node : graph.GetNodes()) {
				if (node.NodeId() == node_id) {
					LOG_FILE("%s Found graph for Graph Id: %d and Node Id : %d", FUNC_NAME, graph_id, node_id);
					return node;
				}
			}
		}
	}
	return {};
}


void Graph::DebugGraphsInfo(bool node_info, bool link_info, graph_t graph_id) {
	std::stringstream ss;

	LOG_INFO("Level_%d Graphs: %d", g_curr_level, g_level_graphs.size());

	auto graph = GetGraph4mId(graph_id);
	std::vector<Graph> level_graph;
	if (graph_id == -1) level_graph = g_level_graphs;
	else level_graph.push_back(graph);

	for (auto& graph : level_graph) {

		ss << "'Graph" << graph.graph_id << "' Max Nodes: " << graph.node_max << " Total Nodes: " << graph.GetNodes().size();
		LOG_FILE("%s", ss.str().c_str()); ss.str(std::string());

		for (auto& node : graph.GetNodes()) {
			if (node_info) {
				ss << "Node Id: " << node.NodeId() << " Material: '" << node.NodeMaterialType() << "' Radius: " << node.NodeRadius() << " Gamma: " << node.NodeGamma() << " Criteria: " << node.NodeCriteria();
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

void IGI::Graph::DebugGraphInfo(graph_t graph_id)
{
	DebugGraphsInfo(true, true, graph_id);
}

string Graph::GetNodeLinks(graph_t graph_id) {
	std::stringstream ss;
	//ss << "Graph #" << std::to_string(graph_id) << std::endl;

	for (auto& graph : g_level_graphs) {
		if (graph.GraphId() == graph_id) {
			for (auto& link : graph.GetLinks()) {
				string node_name_1 = std::to_string(link.NodeLink1());
				string node_name_2 = std::to_string(link.NodeLink2());
				ss << node_name_1 << "=" << node_name_2 << std::endl;
			}
			break;
		}
	}

	//ss << "}" << std::endl;
	return ss.str();
}

float Graph::GetNodeRadius(graph_t graph_id, node_t node_id) {
	auto node = GetNode4mId(graph_id, node_id);
	return node.NodeRadius();
}

float Graph::GetNodeGamma(graph_t graph_id, node_t node_id) {
	auto node = GetNode4mId(graph_id, node_id);
	return node.NodeGamma();
}

int Graph::GetNodeMaterial(graph_t graph_id, node_t node_id) {
	auto node = GetNode4mId(graph_id, node_id);
	return node.NodeMaterial();
}

string Graph::GetNodeMaterialType(graph_t graph_id, node_t node_id) {
	auto node = GetNode4mId(graph_id, node_id);
	return node.NodeMaterialType();
}

int Graph::GetMaxNodes(graph_t graph_id) {
	auto graph = GetGraph4mId(graph_id);
	return graph.NodeMax();
}

int Graph::GetTotalNodes(graph_t graph_id) {
	return g_level_graphs.size();
}


//DOT Graphs section.
void Graph::DOT_CompileGraph(string graph_file, string output_fmt, string layout_engine) {
	string compile_cmd = "dot " + graph_file + " -o " + graph_file + "." + output_fmt + " -T " + output_fmt + " -K " + layout_engine;
	g_Utility.ShellExec(compile_cmd);
}

void Graph::DOT_SaveGraphs(string node_shape, string node_color, bool remove_source) {
	std::stringstream ss;
	const string node_attrib = "node [shape=" + node_shape + ",color=black,style=filled,fillcolor=" + node_color + "]";

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