#pragma once

#include <vector>
#include <unordered_map>
#include "RandomGenerator.h"

class NEATNN
{
public:
	NEATNN();
	NEATNN(int input_num, int output_num);
	NEATNN(const NEATNN& other);
	~NEATNN();

	NEATNN& operator=(const NEATNN& other);

	void execute(std::vector<float> input);

	std::vector<float> getOutput();

	NEATNN crossOver(const NEATNN& parent2);
	void mutate();

private:
	int input_nodes_num, output_nodes_num;
	int node_index; // Keeps track of the maximun index to give to a node

	struct Node;
	struct NodeConnection
	{
		NodeConnection(Node* in_node, Node* out_node, float weight);
		NodeConnection(Node* in_node, Node* out_node, float weight, long int innovation_num);

		float weight;
		Node* in;
		Node* out;
		int innovation_num;

		static long int global_innovation_number;

		bool operator==(const NodeConnection& other)
		{
			return in == other.in && out == other.out;
		}
	};
	struct Node
	{
		Node(short int index);
		Node(const Node& other); // Copy constructor (doesnt copy incoming connections list)

		short int index;
		float value;
		std::list<NodeConnection*> incoming_connections;
		float activation_sum;
		bool activated;
	};
	std::vector<Node*> nodes_list;
	std::vector<NodeConnection*> connections;


	Node* add_node();
	NodeConnection* add_connection(Node* in, Node* out, float weight);

	RandomGenerator rand_generator;

};

