#pragma once

class coarsener {
public:
    virtual void coarsen() = 0;
};

class TarjanCoarsener : public coarsener {
public:
    TarjanCoarsener(Graph& graph);

    void coarsen() override;

private:
    Graph& _graph;
};



