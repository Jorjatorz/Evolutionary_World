#include "NEATNN.h"

#include <algorithm>
#include <list>
#include <unordered_set>
#include "FLog.h"

NEATNN::NEATNN()
{
	node_index = 0;
	input_nodes_num = output_nodes_num = 0;
}

NEATNN::NEATNN(int input_num, int output_num)
	:node_index(0)
{
	for (int i = 0; i < input_num + output_num; i++)
	{
		add_node();
	}

	// Connect every input to every output
	int index = 1;
	for (int in = 0; in < input_num; in++)
	{
		Node* in_node = nodes_list.at(in);
		for (int out = input_num; out < output_num + input_num; out++)
		{
			Node* out_node = nodes_list.at(out);
			NodeConnection* c = add_connection(in_node, out_node, rand_generator.randomFloat(-1.0, 1.0));
			c->innovation_num = index;
			index++;
		}
	}

	input_nodes_num = input_num;
	output_nodes_num = output_num;
}

NEATNN::NEATNN(const NEATNN & other)
	:input_nodes_num(other.input_nodes_num),
	output_nodes_num(other.output_nodes_num),
	node_index(other.node_index)
{

	// Deep copy node_list and connections
	std::unordered_map<short int, Node*> node_map;
	for (auto& node : other.nodes_list)
	{
		Node* newNode = new Node(*node);
		nodes_list.push_back(newNode);
		node_map.insert(std::make_pair(node->index, newNode));
	}
	for (auto& connection : other.connections)
	{
		NodeConnection* newCon = add_connection(node_map[connection->in->index], node_map[connection->out->index], connection->weight);
		if(newCon != nullptr)
			newCon->innovation_num = connection->innovation_num;
	}
}


NEATNN::~NEATNN()
{
	for (auto& connection : connections)
	{
		delete connection;
	}
	for (auto& node : nodes_list)
	{
		delete node;
	}
}

NEATNN& NEATNN::operator=(const NEATNN & other)
{
	// Clear current nn
	for (auto& connection : connections)
	{
		delete connection;
	}
	for (auto& node : nodes_list)
	{
		delete node;
	}
	connections.clear();
	nodes_list.clear();

	input_nodes_num = other.input_nodes_num;
	output_nodes_num = other.output_nodes_num;
	node_index = other.node_index;

	// Deep copy node_list and connections
	std::unordered_map<short int, Node*> node_map;
	for (auto& node : other.nodes_list)
	{
		Node* newNode = new Node(*node);
		nodes_list.push_back(newNode);
		node_map.insert(std::make_pair(node->index, newNode));
	}
	for (auto& connection : other.connections)
	{
		NodeConnection* newCon = add_connection(node_map[connection->in->index], node_map[connection->out->index], connection->weight);
		if (newCon != nullptr)
			newCon->innovation_num = connection->innovation_num;
	}

	return *this;
}

void NEATNN::execute(std::vector<float> input)
{
	// Check that input is correct and set the value to the input neurons
	if (input.size() != input_nodes_num)
	{
		FLog(FLog::FAILURE, "Input vector doesn't match number of input nodes");
		return;
	}

	for (int i = 0; i < input_nodes_num; i++)
	{
		nodes_list.at(i)->value = input.at(i);
		nodes_list.at(i)->activated = true;
	}

	// While any output node is not activated
	bool outputs_down = true;
	while (outputs_down)
	{
		// For each node propagate activations
		for (auto& current_node : nodes_list)
		{
			// If not an input node
			if(current_node->index >= input_nodes_num)
			{
				current_node->activated = false;
				current_node->activation_sum = 0;
				// Add all weights from incoming connections
				for (auto& in_connection : current_node->incoming_connections)
				{
					float add_amount = in_connection->weight * in_connection->in->value;
					if (in_connection->in->activated)
						current_node->activated = true;
					current_node->activation_sum += add_amount;
				}
			}
		}

		// Compute activations of all nodes (except inputs)
		for (auto& current_node : nodes_list)
		{
			// If not an input node
			if (current_node->index >= input_nodes_num)
			{
				current_node->value = std::max(0.0f, current_node->activation_sum); // RELU
			}
		}

		//Check if all outputs have been activated
		outputs_down = false;
		for (int i = input_nodes_num; i < output_nodes_num + input_nodes_num; i++)
		{
			if (nodes_list.at(i)->activated = false)
			{
				outputs_down = true;
				break;
			}
		}
	}
}

std::vector<float> NEATNN::getOutput(const Activation_function a_func)
{
	std::vector<float> toRet;
	for (int i = input_nodes_num; i < output_nodes_num + input_nodes_num; i++)
	{
		if(a_func == Activation_function::RELU)
			toRet.emplace_back(nodes_list.at(i)->value);
		else if (a_func == Activation_function::TANH)
		{
			float value = (std::exp(nodes_list.at(i)->activation_sum) - std::exp(-nodes_list.at(i)->activation_sum)) / (std::exp(nodes_list.at(i)->activation_sum) + std::exp(-nodes_list.at(i)->activation_sum));
			toRet.emplace_back(value);
		}
	}

	return toRet;
}

NEATNN NEATNN::crossOver(const NEATNN & parent2)
{
	NEATNN child;

	auto parent1_it = connections.begin();
	auto parent2_it = parent2.connections.begin();

	RandomGenerator rand_generator;
	std::unordered_map<short int, Node*> nodes_copied;

	// Assuming parent2 has worse fitness
	while (parent1_it != connections.end() && parent2_it != parent2.connections.end())
	{
		// Same innovation, take random selection
		if ((*parent1_it)->innovation_num == (*parent2_it)->innovation_num)
		{
			int chosen = rand_generator.randomInteger(0, 1);
			if (chosen == 0) // Copy from parent 1
			{
				// Check if nodes hadn't been created and create them
				if (nodes_copied.find((*parent1_it)->in->index) == nodes_copied.end())
				{
					Node* newNode = new Node(*(*parent1_it)->in);
					child.nodes_list.push_back(newNode);

					nodes_copied.insert(std::make_pair(newNode->index, newNode));
				}
				if (nodes_copied.find((*parent1_it)->out->index) == nodes_copied.end())
				{
					Node* newNode = new Node(*(*parent1_it)->out);
					child.nodes_list.push_back(newNode);

					nodes_copied.insert(std::make_pair(newNode->index, newNode));
				}
				// Add connection
				Node* in = nodes_copied.find((*parent1_it)->in->index)->second;
				Node* out = nodes_copied.find((*parent1_it)->out->index)->second;
				child.connections.push_back(new NodeConnection(in, out, (*parent1_it)->weight, (*parent1_it)->innovation_num));
			}
			else // Copy from parent 2
			{
				// Check if nodes hadn't been created and create them
				if (nodes_copied.find((*parent2_it)->in->index) == nodes_copied.end())
				{
					Node* newNode = new Node(*(*parent2_it)->in);
					child.nodes_list.push_back(newNode);

					nodes_copied.insert(std::make_pair(newNode->index, newNode));
				}
				if (nodes_copied.find((*parent2_it)->out->index) == nodes_copied.end())
				{
					Node* newNode = new Node(*(*parent2_it)->out);
					child.nodes_list.push_back(newNode);

					nodes_copied.insert(std::make_pair(newNode->index, newNode));
				}
				// Add connection
				Node* in = nodes_copied.find((*parent2_it)->in->index)->second;
				Node* out = nodes_copied.find((*parent2_it)->out->index)->second;
				child.connections.push_back(new NodeConnection(in, out, (*parent2_it)->weight, (*parent2_it)->innovation_num));
			}

			++parent1_it;
			++parent2_it;
		}
		// Best parent has innovation num smaller
		else if ((*parent1_it)->innovation_num < (*parent2_it)->innovation_num)
		{
			// Copy connections and nodes from parent 1
			if (nodes_copied.find((*parent1_it)->in->index) == nodes_copied.end())
			{
				Node* newNode = new Node(*(*parent1_it)->in);
				child.nodes_list.push_back(newNode);

				nodes_copied.insert(std::make_pair(newNode->index, newNode));
			}
			if (nodes_copied.find((*parent1_it)->out->index) == nodes_copied.end())
			{
				Node* newNode = new Node(*(*parent1_it)->out);
				child.nodes_list.push_back(newNode);

				nodes_copied.insert(std::make_pair(newNode->index, newNode));
			}

			Node* in = nodes_copied.find((*parent1_it)->in->index)->second;
			Node* out = nodes_copied.find((*parent1_it)->out->index)->second;
			child.connections.push_back(new NodeConnection(in, out, (*parent1_it)->weight, (*parent1_it)->innovation_num));

			++parent1_it;
		}
		// Worst parent innovation smaller, we dont copy
		else
		{
			++parent2_it;
		}
	}

	// Finish copying best parent remaining connections and nodes
	while (parent1_it != connections.end())
	{
		if (nodes_copied.find((*parent1_it)->in->index) == nodes_copied.end())
		{
			Node* newNode = new Node(*(*parent1_it)->in);
			child.nodes_list.push_back(newNode);

			nodes_copied.insert(std::make_pair(newNode->index, newNode));
		}
		if (nodes_copied.find((*parent1_it)->out->index) == nodes_copied.end())
		{
			Node* newNode = new Node(*(*parent1_it)->out);
			child.nodes_list.push_back(newNode);

			nodes_copied.insert(std::make_pair(newNode->index, newNode));
		}

		Node* in = nodes_copied.find((*parent1_it)->in->index)->second;
		Node* out = nodes_copied.find((*parent1_it)->out->index)->second;
		child.connections.push_back(new NodeConnection(in, out, (*parent1_it)->weight, (*parent1_it)->innovation_num));

		++parent1_it;
	}

	// Populate incoming vectors
	for (auto& connection : child.connections)
	{
		connection->out->incoming_connections.push_back(connection);
	}

	child.input_nodes_num = input_nodes_num;
	child.output_nodes_num = output_nodes_num;
	child.node_index = node_index >= parent2.node_index ? node_index : parent2.node_index;

	// NOTE - When returning the child the copy iteration is in charge of removing recursive connections
	return child;
}

void NEATNN::mutate()
{
	// Weights mutation
	for (auto& connection : connections)
	{
		if (rand_generator.randomFloat() < 0.5)
		{
			connection->weight += rand_generator.gaussianFloat(0.0, 0.05);
		}
	}

	// Structural mutation
	if (rand_generator.randomFloat() < 0.5)
	{
		// Add connection
		if (rand_generator.randomFloat() < 0.5)
		{
			// Choose nodes
			Node* in_node = nodes_list.at(rand_generator.randomInteger(0, nodes_list.size() - 1));
			Node* out_node = nodes_list.at(rand_generator.randomInteger(input_nodes_num, nodes_list.size() - 1)); // Dont include input nodes
			// Search if connection already exits
			bool exists = false;
			for (auto it : out_node->incoming_connections)
			{
				if (it->in->index == in_node->index)
					exists = true;
			}

			if (!exists)
			{
				add_connection(in_node, out_node, rand_generator.randomFloat(-1.0, 1.0));
				NodeConnection::global_innovation_number++;
			}
			
		}
		// Add node
		if (rand_generator.randomFloat() < 0.05)
		{
			Node* new_node = add_node();

			// Choose random connection
			int connection_index = rand_generator.randomInteger(0, connections.size() - 1);
			NodeConnection* old_connection = connections.at(connection_index);
			add_connection(old_connection->in, new_node, 1.0);
			NodeConnection::global_innovation_number++;
			add_connection(new_node, old_connection->out, old_connection->weight);
			NodeConnection::global_innovation_number++;

			// Remove old connection from incoming
			for (auto it = old_connection->out->incoming_connections.begin(); it != old_connection->out->incoming_connections.end(); it++)
			{
				if (*it == old_connection)
				{
					old_connection->out->incoming_connections.erase(it);
					break;
				}
			}

			// Delete old connection from connections
			delete old_connection;
			connections.erase(connections.begin() + connection_index);
		}
	}

	//FLog(FLog::INFO, "%d", connections.size());

}

NEATNN::Node* NEATNN::add_node()
{
	Node* newNode = new Node(node_index);
	node_index++;
	nodes_list.push_back(newNode);
	return newNode;
}

NEATNN::NodeConnection* NEATNN::add_connection(Node* in, Node* out, float weight)
{
	// Find if a connection exist on the other direction (out -> in)
	for (const auto& connection : in->incoming_connections)
	{
		if (connection->in == out)
		{
			FLog(FLog::FAILURE, "Connection already exists in the other way");
			return NULL;
		}
	}

	NodeConnection* newConnection = new NodeConnection(in, out, weight);
	connections.push_back(newConnection);
	out->incoming_connections.push_back(newConnection);

	return newConnection;
}

long int NEATNN::NodeConnection::global_innovation_number = 10;
NEATNN::NodeConnection::NodeConnection(Node* in_node, Node* out_node, float weight)
	:in(in_node),
	out(out_node),
	weight(weight),
	innovation_num(global_innovation_number)
{
}

NEATNN::NodeConnection::NodeConnection(Node * in_node, Node * out_node, float weight, long int innovation_num)
	:in(in_node),
	out(out_node),
	weight(weight),
	innovation_num(innovation_num)
{
}

NEATNN::Node::Node(short int index)
	:index(index),
	value(0.0),
	activated(false),
	activation_sum(0.0)
{
}

NEATNN::Node::Node(const Node& other)
	:index(other.index),
	value(other.value)
{
}
