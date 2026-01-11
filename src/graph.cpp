#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm> 
const Property *
GraphObject::getProperty(const std::string &propName) const
{
    const auto &it = _properties.find(propName);
    if (it != _properties.end())
    {
        return it->second;
    }
    return nullptr;
};

void
Edge::setProperty(const std::string &propertyName, Property *property)
{
    const auto &it = _DEFAULT_EDGE_PROPRETIES.find(propertyName);
    if (it != _DEFAULT_EDGE_PROPRETIES.end())
    {
        const auto &itt = getProperties().find(propertyName);
        if (itt == getProperties().end())
        {
            getProperties()[propertyName] = property;
        }
        else
        {
            std::cout << "[WRN]: Trying to overwrite property \"" << propertyName << "\"\n";
        }
    }
    else
    {
        std::cerr << "[ERR]: Undefined property \"" << propertyName << "\"\n";
    }
}

void
Node::setProperty(const std::string &propertyName, Property *property)
{
    const auto &it = _DEFAULT_NODE_PROPRETIES.find(propertyName);
    if (it != _DEFAULT_NODE_PROPRETIES.end())
    {
        const auto &itt = getProperties().find(propertyName);
        if (itt == getProperties().end())
        {
            getProperties()[propertyName] = property;
        }
        else
        {
            std::cout << "[WRN]: Trying to overwrite property \"" << propertyName << "\"\n";
        }
    }
    else
    {
        std::cerr << "[ERR]: Undefined property \"" << propertyName << "\"\n";
    }
}

Graph*
Graph::createFromDataset(const std::string& dataset_name) {
    Graph* graph = new Graph();
    std::ifstream data_set(dataset_name);
    if (!data_set.is_open()) {
        std::cerr << "Error: Cannot open file :"<<dataset_name<< "\n";
        return nullptr;
    }
    std::unordered_map<int, std::shared_ptr<Node>> nodes;
    std::string line;
    while (std::getline(data_set, line)) {
        if (line.empty() || line[0] == '#') continue; 
        std::istringstream iss(line); int from, to; 
        if (!(iss >> from >> to)) continue;
        std::shared_ptr<Node> fromNode;
        std::shared_ptr<Node> toNode;
        if (nodes.find(from) != nodes.end()) {
            fromNode = nodes[from];
        } else {
            fromNode = std::make_shared<Node>(graph->_nodesCount++);
            fromNode->setProperty("DATA", new DataProprerty(from));
            nodes[from] = fromNode;
            graph->_nodes.push_back(fromNode);
        }
        if (nodes.find(to) != nodes.end()) {
            toNode = nodes[to];
        } else {
            toNode = std::make_shared<Node>(graph->_nodesCount++);
            toNode->setProperty("DATA", new DataProprerty(to));
            nodes[to] = toNode;
            graph->_nodes.push_back(toNode);
        }
        std::shared_ptr<Edge> edge = std::make_shared<Edge>(graph->_edgesCount++, fromNode, toNode);
        fromNode->addOutEdge(edge);
        toNode->addInEdge(edge);
        graph->_edges.push_back(edge);
    }
    return graph;
};

void Graph::printGraphStats() const {
    std::cout<<" == GRAPH STATS == "<<std::endl;
    std::cout <<"| SIZE : "<<getString() << std::endl;
    
    size_t srcNodes = 0;
    size_t sinkNodes = 0;
    size_t unconectedNodes = 0;
    size_t maxFanout = 0;
    size_t maxFanin = 0;
    size_t avgFanout = 0;
    size_t avgFanin = 0;

    for (const auto& node : _nodes) {
        if (node->getInEdges().size() == 0) {
            srcNodes++;
        }
        if (node->getOutEdges().size() == 0) {
            sinkNodes++;
        }
        if (node->getOutEdges().size() > maxFanout) {
            maxFanout = node->getOutEdges().size();
        }
        if (node->getInEdges().size() > maxFanin) {
            maxFanin = node->getInEdges().size();
        }
        if (node->getOutEdges().size() == 0 && node->getInEdges().size() == 0) {
            unconectedNodes++;
        }
        avgFanout += node->getOutEdges().size();
        avgFanin += node->getInEdges().size();
    }
    avgFanout /= _nodes.size();
    avgFanin /= _nodes.size();

    
    std::cout << "| Source nodes: " << srcNodes << std::endl;
    std::cout << "| Sink nodes: " << sinkNodes << std::endl;
    std::cout << "| Max fanout: " << maxFanout << std::endl;
    std::cout << "| Max fanin: " << maxFanin << std::endl;
    std::cout << "| Avg fanout: " << avgFanout << std::endl;
    std::cout << "| Avg fanin: " << avgFanin << std::endl;
    std::cout << "| Unconnected nodes: " << unconectedNodes << std::endl;

};