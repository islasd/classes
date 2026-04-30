#include <iostream>
using std::cout, std::cin, std::endl;

#include <string>
using std::string, std::to_string;

#include <cassert>

#include <list>
using std::list, std::advance;

int linear_search(list<int>& lst, int x) {
    list<int>::iterator it = lst.begin();
    for (int i=0; i < lst.size(); i++) {
        if (*it == x) {
            return i;
        }
        advance(it, 1);
    }
    return -1;
}

class GridGraph {
    private:
    /* Feel free to add more members if needed */

    list<int>* adjacents;
    int width;
    int height;
    int s_index;
    int t_index;
    char* labyrinth; // 1D array of the labyrinth chars

    bool* make_visited_array() { 
        // allocate manually since unable to do so in constructor
        bool* ret = new bool[width * height];
        for (int i=0; i < count(); i++) {
            ret[i] = false;
        }

        return ret;
    }

    int* make_depth_array() {
        int* ret = new int[count()];
        for (int i=0; i < count(); i++) {
            ret[i] = -1;
        }
        
        return ret;
    }

    int count() {
        return width * height;
    }

    void delete_array(bool to_delete[]) { 
        // deallocate manually since unable to do so in constructor
        delete [] to_delete;
    }

    void delete_array(int to_delete[]) {
        delete [] to_delete;
    }

    string find_best_path() {
		// BFS
		// initialize visited
	    bool* visited = make_visited_array();

		// initialize parents
		int* parent = new int[count()];
		for (int i=0; i < count(); i++) {
			parent[i] = -1;
		}
		parent[s_index] = s_index;

		// initialize frontier
		list<int> frontier;
		frontier.push_back(s_index);
		
		// exploring with BFS
		while (frontier.size() > 0) {
			int next = frontier.front();
			frontier.pop_front(); // frontier acting like a queue
			if (visited[next]) {continue;}
			visited[next] = true; 
			if (next == t_index) {break;}
			for (int i=0; i < count(); i++) {
				if (has_edge(next, i) and !visited[i]) {
					frontier.push_back(i);
					if (parent[i] == -1) {
						parent[i] = next;
					}
				}
			}
		}

		// did we reach t?
		if (visited[t_index]) {
			// path exists, follow generators to return it
			int x = parent[t_index];
			while (x != s_index) {
                assert(0 <= x and x <= width * height);
                labyrinth[x] = '0';
				x = parent[x];
			}
		}
		else {
			delete [] parent;
            delete_array(visited);
            return "path not found";
		}

		delete [] parent;
        delete_array(visited);

		return "done";

	}

	void find_reachable() {
		// BFS
		// initialize visited
	    bool* visited = make_visited_array();

		// initialize parents
		int* parent = new int[count()];
		for (int i=0; i < count(); i++) {
			parent[i] = -1;
		}
		parent[s_index] = s_index;

		// initialize frontier
		list<int> frontier;
		frontier.push_back(s_index);
		
		// exploring with BFS
		while (frontier.size() != 0) {
			int next = frontier.front();
			frontier.pop_front(); // frontier acting like a queue
			if (visited[next]) {continue;}
			visited[next] = true; 
			for (int i=0; i < count(); i++) {
				if (has_edge(next, i) and !visited[i]) {
					frontier.push_back(i);
					if (parent[i] == -1) {
						parent[i] = next;
					}
				}
			}
		}

        // turn everything reachable into 0's
        for (int i = 0; i < count(); i++) {
            assert(0 <= i and i <= width * height);
            if (i != s_index and parent[i] != -1) {
                labyrinth[i] = '0';
            }
        }

		delete [] parent;
        delete_array(visited);

	}

	void find_reachable_in(int d) {
		// BFS
		// initialize visited
	    bool* visited = make_visited_array();
        int* depth = make_depth_array();
        int max_depth;

		// initialize parents
		int* parent = new int[count()];
		for (int i=0; i < count(); i++) {
			parent[i] = -1;
		}
		parent[s_index] = s_index;
        depth[s_index] = 0;
		// initialize frontier
		list<int> frontier;
		frontier.push_back(s_index);
		
		// exploring with BFS
		while (frontier.size() != 0) {
			int next = frontier.front();
			frontier.pop_front(); // frontier acting like a queue
			if (visited[next]) {continue;}
			visited[next] = true; 
			for (int i=0; i < count(); i++) {
				if (has_edge(next, i) and !visited[i]) {
					frontier.push_back(i);
					if (parent[i] == -1) {
						parent[i] = next;
                        depth[i] = 1 + depth[next];
                        max_depth = depth[i];
					}
				}
			}
		}

        // turn everything reachable in d into 0's
        for (int i = 0; i < count(); i++) {
            assert(0 <= i and i <= width * height);
            if (i != s_index and parent[i] != -1 and depth[i] <= d) {
                labyrinth[i] = '0';
            }
        }

		delete [] parent;
        delete_array(visited);
        delete_array(depth);
	}

	void find_furthest() {
		// BFS
		// initialize visited
	    bool* visited = make_visited_array();
        int* depth = make_depth_array();
        int max_depth;

		// initialize parents
		int* parent = new int[count()];
		for (int i=0; i < count(); i++) {
			parent[i] = -1;
		}
		parent[s_index] = s_index;
        depth[s_index] = 0;
		// initialize frontier
		list<int> frontier;
		frontier.push_back(s_index);
		
		// exploring with BFS
		while (frontier.size() != 0) {
			int next = frontier.front();
			frontier.pop_front(); // frontier acting like a queue
			if (visited[next]) {continue;}
			visited[next] = true; 
			for (int i=0; i < count(); i++) {
				if (has_edge(next, i) and !visited[i]) {
					frontier.push_back(i);
					if (parent[i] == -1) {
						parent[i] = next;
                        depth[i] = 1 + depth[next];
                        max_depth = depth[i];
					}
				}
			}
		}

        // turn everything reachable in d into 0's
        for (int i = 0; i < count(); i++) {
            assert(0 <= i and i <= width * height);
            if (i != s_index and parent[i] != -1 and depth[i] == max_depth) {
                labyrinth[i] = '0';
            }
        }

		delete [] parent;
        delete_array(visited);
        delete_array(depth);

	}

    public:
    GridGraph(int w, int h) {
        width = w;
        height = h;
        labyrinth = new char[width * height];
        for (int i=0; i < width * height; i++) {
            cin >> labyrinth[i];
            if (labyrinth[i] == 'S') {s_index = i;}
            if (labyrinth[i] == 'T') {t_index = i;}
        }
        adjacents = new list<int>[width * height];
        for (int row=0; row < height; row++) {
            for (int col=0; col < width; col++) {
                int index = row*width + col;
                // north-west
                if (row > 0 and col > 0 and labyrinth[index - width - 1] != '*') {
                    adjacents[index].push_back(index - width - 1);
                }
                // north
                if (row > 0 and labyrinth[index - width] != '*') {
                    adjacents[index].push_back(index - width);
                }
                // north-east
                if (row > 0 and col < width - 1 and labyrinth[index - width + 1] != '*') {
                    adjacents[index].push_back(index - width + 1);
                }
                // west
                if (col > 0 and labyrinth[index - 1] != '*') {
                    adjacents[index].push_back(index - 1);
                }
                // east
                if (col < width - 1 and labyrinth[index + 1] != '*') {
                    adjacents[index].push_back(index + 1);
                }
                // south-west
                if (row < height - 1 and col > 0 and labyrinth[index + width - 1] != '*') {
                    adjacents[index].push_back(index + width - 1);
                }
                // south
                if (row < height - 1 and labyrinth[index + width] != '*') {
                    adjacents[index].push_back(index + width);
                }
                // south-east
                if (row < height - 1 and col < width - 1 and labyrinth[index + width + 1] != '*') {
                    adjacents[index].push_back(index + width + 1);
                }
            }
        }
    }

    ~GridGraph() {
        delete [] labyrinth;
        delete [] adjacents;
    }

    bool has_edge(int v, int w) {
        return (linear_search(adjacents[v], w) != -1);
    }

    /* Part (a) methods */
    void show_best_path() {
        cout << "Pathfinding... " << find_best_path() << endl; 
        output();
    }

	



    /* Part (b) methods
     A lot of the weirdness of how outputs are approached is because I can't modify main() */
    
    void show_reachable() {
        cout << "Reachable:" << endl;
        find_reachable();
        output();
    }

    void show_reachable_in(int d) {
        cout << "Reachable in " << d << " steps:" << endl;
        find_reachable_in(d);
        output();
    }

    void show_furthest() {
        cout << "Furthest:" << endl;
        find_furthest();
        output();
    }

    void output() {
        for (int i=0; i < width * height; i++) {
            assert(0 <= i and i <= width * height - 1);
            cout << labyrinth[i];
            if ((i + 1) % width == 0) {
                cout << endl;
            }
        }
    }

};

/* Do not modify the main() function */

int main() {
    int w;
    cin >> w;
    int h;
    cin >> h;
    GridGraph mygraph(w, h);
    string instruction;
    cin >> instruction;
    if (instruction == "show_best_path") {
        mygraph.show_best_path();
    }
    else if (instruction == "show_reachable") {
        mygraph.show_reachable();
    }
    else if (instruction == "show_reachable_in") {
        int d;
        cin >> d;
        mygraph.show_reachable_in(d);
    }
    else if (instruction == "show_furthest") {
        mygraph.show_furthest();
    }
    else {
        cout << "Invalid instruction" << endl;
    }
    return 0;
}
