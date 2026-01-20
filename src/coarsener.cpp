#include "coarsener.hpp"

namespace TarjanCoarsenerUtils {
    void strongConnect(unsigned v, std::vector<int>& index, std::vector<int>& lowlink, std::vector<bool>& onStack, std::stack<unsigned>& st, int& currentIndex, std::vector<std::vector<unsigned>>& sccs) {
    index[v]   = currentIndex;
    lowlink[v] = currentIndex;
    currentIndex++;
    st.push(v);
    onStack[v] = true;
    auto node = _graph.getNode(v);
    for (auto& edge : node->getOutEdges()) {
        unsigned w = edge->getTarget()->getId();

        if (index[w] == -1) {
            strongConnect(w);
            lowlink[v] = std::min(lowlink[v], lowlink[w]);
        }
        else if (onStack[w]) {
            lowlink[v] = std::min(lowlink[v], index[w]);
        }
    }
    // root of SCC
    if (lowlink[v] == index[v]) {
        std::vector<unsigned> component;

        while (true) {
            unsigned w = st.top();
            st.pop();
            onStack[w] = false;

            component.push_back(w);

            if (w == v)
                break;
        }
        sccs.push_back(component);
    }
}

}