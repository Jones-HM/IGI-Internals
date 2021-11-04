#pragma once
#include "Common.hpp"
#include "CommonConst.hpp"

namespace IGI {
	//Class to hold Level/A.I Graph information.
	class Graph {
	private:
		bool graph_init = false;
		bool node_init = false;
		graph_t graph_id; //Graph Id.
		int node_max;//Maximum no. of nodes.

		struct Real64
		{
			double x, y, z;
		public:
			Real64() { x = y = z = 0.0f; }
			Real64(double x, double y, double z)
			{
				this->x = x;
				this->y = y;
				this->z = z;
			}
		};

		//Class to hold  Graph Nodes information.
	public:
		class Node {
			graph_t node_id; //Current Node id.
			Real64 node_pos; //Node position in Real64.
			float node_radius; //Node radius in metre.
			float node_gamma; //Node gamma angle.
			int node_material; //Node material like 'Air','Ground','Metal'.
			int node_criteria;//Node criteria example Door,Ladder for A.I Graph.

		public:
			//Ctors/Dtors
			Node() = default;
			~Node() = default;
			Node(graph_t node_id, Real64 node_pos, float node_radius, float node_gamma, int node_material, int node_criteria) {
				this->node_id = node_id; this->node_pos = node_pos; this->node_radius = node_radius; this->node_gamma = node_gamma;
				this->node_material = node_material; this->node_criteria = node_criteria;
			}
			//Setters/Getters.
			graph_t NodeId() { return node_id; }
			void NodeId(graph_t node_id) { this->node_id = node_id; }

			Real64 NodePosition() { return node_pos; }
			void NodePosition(Real64 node_pos) { this->node_pos = node_pos; }

			float NodeRadius() { return node_radius; }
			void NodeRadius(float node_radius) { this->node_radius = node_radius; }

			float NodeGamma() { return node_gamma; }
			void NodeGamma(float node_gamma) { this->node_gamma = node_gamma; }

			int NodeMaterial() { return node_material; }
			string NodeMaterialType() { return game_materials[node_material]; }
			void NodeMaterial(int node_material) { this->node_material = node_material; }

			int NodeCriteria() { return node_criteria; }
			void NodeCriteria(int node_criteria) { this->node_criteria = node_criteria; }
		};

		//Class to hold  Graph Node Links information.
		class Link {
			int node_link1; //Node link 1, Connection/Link to 1st node adjacent to it.
			int node_link2; //Node link 2, Connection/Link to 2nd node adjacent to it.
			int node_link_type; //Link type to other nodes.
		public:
			//Ctors/Dtors
			Link() = default;
			~Link() = default;
			Link(int node_link1, int node_link2, int node_link_type) { this->node_link1 = node_link1; this->node_link2 = node_link2; this->node_link_type = node_link_type; }

			//Setters/Getters.
			int NodeLink1() { return node_link1; }
			void NodeLink1(int node_link1) { this->node_link1 = node_link1; }

			int NodeLink2() { return node_link2; }
			void NodeLink2(int node_link2) { this->node_link2 = node_link2; }

			int NodeLinkType() { return node_link_type; }
			void NodeLinkType(int node_link_type) { this->node_link_type = node_link_type; }
		};

	private:
		//Graph Node list data.
		Node node;
		Link link;
		std::vector<Node> m_graph_nodes;
		std::vector<Link> m_nodes_link;

	public:
		//Ctors/Dtors
		Graph() = default;
		~Graph() = default;
		Graph(int graph_id, int node_max, int node_id, Node& node) {
			this->graph_id = graph_id; this->node_max = node_max; this->node = node;
		}

		//Graph Methods.
		void AddNode() { m_graph_nodes.push_back(node); }
		void AddNode(Node& node) { m_graph_nodes.push_back(node); }
		void AddLink() { m_nodes_link.push_back(link); }
		void AddLink(Link& link) { m_nodes_link.push_back(link); }

		//Init Methods.
		bool GraphInit() { return graph_init; }
		void GraphInit(bool graph_init) { this->graph_init = graph_init; }
		bool NodeInit() { return node_init; }
		void NodeInit(bool node_init) { this->node_init = node_init; }


		void DebugGraphInfo(bool node_info = true, bool link_info = false);
		string DebugGraphInfo(graph_t graph_id);
		void DOT_SaveGraphs(string node_shape="doublecircle", string node_color = "yellow", bool remove_source = false);
		void DOT_CompileGraph(string graph_file, string output_fmt="svg", string layout_engine="fdp");
		Graph GetGraphData(graph_t graph_id);
		int GetMaxNodes(graph_t graph_id);
		int GetTotalNodes(graph_t graph_id);

		//Node Methods.
		int GetNodeMaterial(graph_t graph_id, int node_id);
		string GetNodeMaterialType(graph_t graph_id, int node_id);
		string GetMaterialType(int material_id);
		float GetNodeRadius(graph_t graph_id, int node_id);
		float GetNodeGamma(graph_t graph_id, int node_id);
		int GetNodeCriteria(graph_t graph_id, int node_id);


		//Setters/Getters for Node.
		Node& GetNode() { return node; }
		std::vector<Node> GetNodes() { return m_graph_nodes; }
		void SetNode(Node& node) { this->node = node; }

		//Setters/Getters For Link.
		Link& GetLink() { return link; }
		std::vector<Link> GetLinks() { return m_nodes_link; }
		void SetLink(Link& node) { this->link = link; }

		graph_t GraphId() { return graph_id; }
		void GraphId(graph_t graph_id) { this->graph_id = graph_id; }

		int NodeMax() { return node_max; }
		void NodeMax(int node_max) { this->node_max = node_max; }
	};
	inline Graph g_Graph;
	inline std::vector<Graph> g_level_graphs;
}