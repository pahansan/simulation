#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

class RandomTreeLimitedDegree {
public:
  explicit RandomTreeLimitedDegree(unsigned int seed = std::random_device{}())
      : gen(seed) {}

  std::vector<std::pair<int, int>> generate(int N, int Deg) {
    if (N <= 1 || Deg < 1 || Deg > N - 1)
      return {};

    std::vector<std::pair<int, int>> edges;
    edges.reserve(N - 1);

    std::vector<int> I(N);
    std::iota(I.begin(), I.end(), 1);
    std::vector<int> Degree(N + 1, 0);

    std::uniform_int_distribution<> root_dist(0, N - 1);
    std::swap(I[N - 1], I[root_dist(gen)]);

    int m = 1;
    int k = 0;

    while (m < N) {
      std::uniform_int_distribution<> old_dist(N - m, N - k - 1);
      std::uniform_int_distribution<> new_dist(0, N - m - 1);

      int s_idx = old_dist(gen);
      int t_idx = new_dist(gen);

      int s = I[s_idx];
      int t = I[t_idx];

      edges.emplace_back(s, t);

      Degree[s]++;
      Degree[t]++;

      if (Degree[s] == Deg) {
        std::swap(I[s_idx], I[N - k - 1]);
        k++;
      }

      std::swap(I[t_idx], I[N - m - 1]);

      m++;
    }

    return edges;
  }
  std::vector<std::pair<int, int>>
  addRandEdges(std::vector<std::pair<int, int>> tree) {
    const int n_edges = tree.size();
    const int n_vertexes = n_edges + 1;
    const int max_edges = n_vertexes * (n_vertexes - 1) / 2;
    std::uniform_int_distribution<> to_add_edge_dist(1, max_edges - n_edges);
    const int n_edges_to_add = to_add_edge_dist(gen);
    std::uniform_int_distribution<> edge_dist(1, n_vertexes);
    int i = 0;
    while (i < n_edges_to_add) {
      std::pair<int, int> new_edge{edge_dist(gen), edge_dist(gen)};
      if (new_edge.first == new_edge.second)
        continue;
      std::pair<int, int> reverted_edge{new_edge.second, new_edge.first};

      auto it = std::find(tree.begin(), tree.end(), new_edge);
      if (it != tree.end()) {
        continue;
      }
      it = std::find(tree.begin(), tree.end(), reverted_edge);
      if (it != tree.end()) {
        continue;
      }
      tree.push_back(new_edge);
      i++;
    }
    return tree;
  }

private:
  std::mt19937 gen;
};

void create_neo4j_script(const std::vector<std::pair<int, int>> &tree) {
  std::cout << "#!/bin/bash\n";
  std::cout << "docker exec --interactive --tty neo4j cypher-shell -u neo4j -p "
               "password \"\n";
  std::cout << "MATCH (n) DETACH DELETE n;\n";
  for (int i = 1; i <= tree.size() + 1; i++) {
    std::cout << "CREATE (:Node {Id: " << i << "});\n";
  }
  for (const auto &[u, v] : tree) {
    std::cout << "MATCH (a:Node {Id: " << u << "}), (b:Node {Id: " << v
              << "}) MERGE (a)-[:CONNECTED]->(b);\n";
  }
  std::cout << "\"\n";
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0]
              << "<tree/graph> <N Vertexes> <Max Degree>\n";
    return 1;
  }
  int N = std::stoi(argv[2]);
  int Deg = std::stoi(argv[3]);

  RandomTreeLimitedDegree generator;
  auto tree = generator.generate(N, Deg);

  if (std::string(argv[1]) == "graph")
    tree = generator.addRandEdges(tree);

  create_neo4j_script(tree);
  return 0;
}
