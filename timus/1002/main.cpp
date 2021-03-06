#include <vector>
#include <iostream>
#include <iterator>
#include <inttypes.h>
#include <map>
#include <list>
#include <sstream>

using namespace std;

struct Node {
	uint32_t pos;
	string word;
	uint32_t idx;
	Node *prev;
	vector<Node*> next;
	Node(Node *prev, uint32_t pos, string word, uint32_t idx) {
		this->pos = pos;
		this->word = word;
		this->idx = idx;
		this->prev = prev;
	};
	~Node() {
		for (vector<Node*>::iterator it = next.begin(); it != next.end(); ++it) {
			delete *it;
		}
	}
};

struct Test {
	uint64_t number;
	vector<string> words;
	map<uint8_t,uint8_t> sym2num;

	Test(uint64_t number) {
		this->number = number;
		int words_count;
		cin >> words_count;
		words.reserve(words_count);

		string str;
		for (int i = 0; i < words_count; ++i) {
			cin >> str;
			words.push_back(str);
		}

		sym2num.insert(pair<uint8_t,uint8_t>('i','1'));
		sym2num.insert(pair<uint8_t,uint8_t>('j','1'));
		sym2num.insert(pair<uint8_t,uint8_t>('a','2'));
		sym2num.insert(pair<uint8_t,uint8_t>('b','2'));
		sym2num.insert(pair<uint8_t,uint8_t>('c','2'));
		sym2num.insert(pair<uint8_t,uint8_t>('d','3'));
		sym2num.insert(pair<uint8_t,uint8_t>('e','3'));
		sym2num.insert(pair<uint8_t,uint8_t>('f','3'));
		sym2num.insert(pair<uint8_t,uint8_t>('g','4'));
		sym2num.insert(pair<uint8_t,uint8_t>('h','4'));
		sym2num.insert(pair<uint8_t,uint8_t>('k','5'));
		sym2num.insert(pair<uint8_t,uint8_t>('l','5'));
		sym2num.insert(pair<uint8_t,uint8_t>('m','6'));
		sym2num.insert(pair<uint8_t,uint8_t>('n','6'));
		sym2num.insert(pair<uint8_t,uint8_t>('p','7'));
		sym2num.insert(pair<uint8_t,uint8_t>('r','7'));
		sym2num.insert(pair<uint8_t,uint8_t>('s','7'));
		sym2num.insert(pair<uint8_t,uint8_t>('t','8'));
		sym2num.insert(pair<uint8_t,uint8_t>('u','8'));
		sym2num.insert(pair<uint8_t,uint8_t>('v','8'));
		sym2num.insert(pair<uint8_t,uint8_t>('w','9'));
		sym2num.insert(pair<uint8_t,uint8_t>('x','9'));
		sym2num.insert(pair<uint8_t,uint8_t>('y','9'));
		sym2num.insert(pair<uint8_t,uint8_t>('o','0'));
		sym2num.insert(pair<uint8_t,uint8_t>('q','0'));
		sym2num.insert(pair<uint8_t,uint8_t>('z','0'));
	}

	string toNumberString(const string &s) {
		string r;
		r.reserve(s.size());
		for (string::const_iterator it = s.begin(); it != s.end(); ++it) {
			r.push_back(sym2num.at(*it));
		}
		return r;
	}

	template <typename T>
		string toString(const T &t) {
			ostringstream ss;
			ss << t;
			return ss.str();
		};

	void process() {
		ostringstream ss_result;
		/* convert words to number strings */
		vector<string> words_num;
		words_num.reserve(words.size());

		for (vector<string>::iterator it = words.begin(); it != words.end(); it++) {
			words_num.push_back(toNumberString(*it));
		}

		list<Node*> active_nodes;
		list<Node*> last_nodes;
		Node result_head(NULL, 0, "", 0);

		/* initial condition */
		string str_number(toString(number));
		uint32_t idx = 0;
		for (vector<string>::const_iterator words_it = words_num.begin(); words_it != words_num.end(); words_it++, idx++) {
			if (words_it->compare(0, words_it->size(), str_number,0,words_it->size()) == 0) {
				Node *node = new Node(&result_head, words_it->size(), *words_it, idx++);
				active_nodes.push_back(node);
				result_head.next.push_back(node);
			}
		}

		/* building tree - new active nodes are added to back, active nodes to process are picked up at front */
		while (active_nodes.size() > 0) {

			/* pick up active node */
			Node *act_node = active_nodes.front();
			active_nodes.pop_front();

			/* add new active nodes */
			idx = 0;
			for (vector<string>::const_iterator w_it = words_num.begin(); w_it != words_num.end(); w_it++, idx++) {
				if (w_it->compare(0, w_it->size(), str_number, act_node->pos, w_it->size()) == 0) {
					Node *new_node = new Node(act_node, act_node->pos + w_it->size(), *w_it, idx);
					act_node->next.push_back(new_node);
					active_nodes.push_back(new_node);
				}
			}

			if (act_node->next.size() == 0) {
				last_nodes.push_back(act_node);
			}
		}

		/* remove incomplete branches (total lenght of branch is smaller than "str_number" */
		Node *minimum_size_end_node = NULL;
		uint32_t min_size = str_number.size();
		for (list<Node*>::iterator it = last_nodes.begin(); it != last_nodes.end(); it++) {
			Node *last_node = *it;
			if (last_node->pos != str_number.size()) {
				Node *prev = last_node->prev;
				while (prev != NULL) {
					for (vector<Node*>::iterator it = prev->next.begin(); it != prev->next.end(); it++) {
						if (*it == last_node) {
							prev->next.erase(it);
							break;
						}
					}
					delete last_node;
					if (prev->next.size() == 0) {
						prev = prev->prev;
					} else {
						prev = NULL;
					}
				}
			} else {
				/* find shortest branch */
				Node *prev = last_node->prev;
				uint32_t size = 0;
				while (prev != NULL) {
					size++;
					prev = prev->prev;
				}
				if (size < min_size) {
					min_size = size;
					minimum_size_end_node = last_node;
				}
			}
		}

		/* print result */
		if (min_size == str_number.size()) {
			ss_result << "No solution." << endl;
		} else {
			Node *prev = minimum_size_end_node;
			vector<string> result;
			while (prev != NULL && prev->word != "") {
				result.push_back(words[prev->idx]);
				prev = prev->prev;
			}
			for (size_t i = result.size(); i-- > 0; ) {
				ss_result << result[i];
				if (i != 0) ss_result << " ";
			}
			ss_result << endl;
		}
		cout << ss_result.str();
	}
};

int main(int argc, char *argv[]) {
	vector<Test> tests;

	while (!cin.eof()) {
		int64_t number;
		cin >> number;
		if (number == -1) { break; }
		tests.push_back(Test(number));
	}

	/* process data structures */
	for (vector<Test>::iterator it = tests.begin(); it != tests.end(); ++it) {
		it->process();
	}

	return 0;
}
