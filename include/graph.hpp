#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <memory>

class Edge;
class Node;
class AggregatedEdge;
class AggregatedNode;
class Graph;
class AggregationLevel;

class Property {
    public:
        virtual std::string getName() = 0;
        virtual ~Property() {}
};

class DataProprerty : public Property {
    public:
        std::string getName() override { return "DATA"; }
        DataProprerty(int data) : _data(data) {};
        int getData() { return _data; };
    private:
        int _data;
};

class GraphObject {
    public:
        virtual std::string getString() = 0;
        virtual void setProperty(const std::string& propertyName, Property* property) = 0;
        const Property* getProperty(const std::string& propName) const;
        unsigned getId() const { return _id; };
        GraphObject(unsigned id): _id(id) {}
        ~GraphObject() {
            for (auto& it : _properties) {
                delete it.second;
            }
        }
    protected:
        std::unordered_map<std::string, Property*>& getProperties() { return _properties; };
    private:
        unsigned _id;
        std::unordered_map<std::string, Property*> _properties;
};


class Edge : public GraphObject {
    public:
        inline static const std::set<std::string> _DEFAULT_EDGE_PROPRETIES;
        void setProperty(const std::string& propertyName, Property* property) override;
        std::string getString() override { return "Edge <" + std::to_string(getId()) + ">"; }
        const std::shared_ptr<Node>& getSource() const { return _source; }
        const std::shared_ptr<Node>& getTarget() const { return _target; }
        const std::shared_ptr<AggregatedEdge>& getAggregatedEdge() { return _aggregatedEdge; }
        void setAggregatedEdge(std::shared_ptr<AggregatedEdge> aggregatedEdge) { _aggregatedEdge = aggregatedEdge; }
        bool isAggregated() const { return _aggregatedEdge != nullptr; }
        void removeAggregatedEdge() { _aggregatedEdge = nullptr; }
        Edge(unsigned id, std::shared_ptr<Node> source, std::shared_ptr<Node> target): GraphObject(id), _source(source), _target(target) {}
        ~Edge() = default;
    protected:
        std::shared_ptr<Node> _source;
        std::shared_ptr<Node> _target;
        std::shared_ptr<AggregatedEdge> _aggregatedEdge;
};

class AggregatedEdge : public Edge {
    public:
        std::string getString() override { return "AggregatedEdge <" + std::to_string(getId()) + ">"; };
        std::vector<std::shared_ptr<Edge>> getChildEdges() const { return _childEdges; };
        AggregatedEdge(unsigned id, std::shared_ptr<Node> source, std::shared_ptr<Node> target, std::vector<std::shared_ptr<Edge>> childEdges): Edge(id, source, target), _childEdges(childEdges) {};
        ~AggregatedEdge();
    private:
        std::vector<std::shared_ptr<Edge>> _childEdges;
};

class Node : public GraphObject{
    public:
        inline static const std::set<std::string> _DEFAULT_NODE_PROPRETIES = {"DATA"};
        void setProperty(const std::string& propertyName, Property* property) override ;
        std::string getString() override { return "Node <" + std::to_string(getId()) + "> DATA: " + std::to_string(static_cast<DataProprerty *>(const_cast<Property *>(getProperty("DATA")))->getData()); };
        Node(unsigned id):GraphObject(id) {};
        void addOutEdge(std::shared_ptr<Edge> edge) { _outEdes.push_back(edge); };
        void addInEdge(std::shared_ptr<Edge> edge) { _inEdes.push_back(edge); };
        std::vector<std::shared_ptr<Edge>> getOutEdges() { return _outEdes; };
        std::vector<std::shared_ptr<Edge>> getInEdges() { return _inEdes; };
        const std::shared_ptr<AggregatedNode>& getAggregatedNode() { return _aggregatedNode; }
        void setAggregatedNode(std::shared_ptr<AggregatedNode> aggregatedNode) { _aggregatedNode = aggregatedNode; }
        bool isAggregated() const { return _aggregatedNode != nullptr; }
        void removeAggregatedNode() { _aggregatedNode = nullptr; }
        ~Node() = default;
    private:
        std::vector<std::shared_ptr<Edge>> _outEdes;
        std::vector<std::shared_ptr<Edge>> _inEdes;
        std::shared_ptr<AggregatedNode> _aggregatedNode;
};

class AggregatedNode : public Node {
    public:
        std::string getString() override { return "AggregatedNode <" + std::to_string(getId()) + ">"; };
        std::vector<std::shared_ptr<Node>> getChildNodes() const { return _childNodes; };
        std::vector<std::shared_ptr<AggregationLevel>> getAggregationLevels() const { return _aggregationLevels; };
        AggregatedNode(unsigned id, std::vector<std::shared_ptr<Node>> childNodes): Node(id), _childNodes(childNodes) {};
        ~AggregatedNode();
    private:
        std::vector<std::shared_ptr<Node>>              _childNodes;
        std::vector<std::shared_ptr<AggregationLevel>>  _aggregationLevels;
};

class AggregationLevel : public GraphObject {
    public:
        std::string getString() override { return "AggregationLevel <" + std::to_string(getId()) + ">"; };
        std::vector<std::shared_ptr<GraphObject>> getGraphObjects() const { return _graphObjects; };
        
        AggregationLevel(unsigned id, unsigned level): GraphObject(id), _level(level) {};
        ~AggregationLevel() = default;
    private:
        std::vector<std::shared_ptr<GraphObject>> _graphObjects;
        unsigned                                  _level;
};

class Graph {
    public:
        static Graph* createFromDataset(const std::string& dataset_name);
        void printGraphStats() const;

        std::string getString() const { return "Graph <N:" + std::to_string(_nodesCount) + ", E:" + std::to_string(_edgesCount) + ">"; };
        const size_t getNodesCount() const { return _nodesCount; }
        const size_t getEdgesCount() const { return _edgesCount; }
        const std::vector<std::shared_ptr<Node>>& getNodes() const { return _nodes; };
        const std::vector<std::shared_ptr<Edge>>& getEdges() const { return _edges; };
        const std::shared_ptr<Node>& getNode(unsigned id) const { return _nodes[id]; };
        ~Graph() = default;
    private:
        Graph(): _nodesCount(0), _edgesCount(0){};
        void CreateNode(Node* node);
        std::vector<std::shared_ptr<Node>> _nodes;
        std::vector<std::shared_ptr<Edge>> _edges;
        size_t             _nodesCount = 0;
        size_t             _edgesCount = 0;

        std::vector<std::shared_ptr<AggregationLevel>> _aggregationLevels;
};