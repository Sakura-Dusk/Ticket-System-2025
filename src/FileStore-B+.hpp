#ifndef FILESOTRE_HPP
#define FILESOTRE_HPP

#include<iostream>
#include<vector>
#include<cstring>

#include "Chars.hpp"
#include "vector.hpp"
#include "MemoryRiver_With_Cache.hpp"


const int BLOCK_SIZE = 16;
const int MAX_DEG = 1005;

template<typename indextype, typename valuetype>
//需要 indextype 和 valuetype 可比较
class FileStore {
    class data {
        private:
            indextype index;
            valuetype value;
            friend class head_element;
            friend class FileStore;

        public:

            data() {
                index = indextype(); value = valuetype();
            }
            data(indextype index_, valuetype value_) : index(index_), value(value_) {}

            bool operator ==(const data &other) const {
                return index == other.index && value == other.value;
            }
            bool operator !=(const data &other) const {
                return !(*this == other);
            }
            bool operator <(const data &other) const {
                if (index != other.index) return index < other.index;
                return value < other.value;
            }
            bool operator <=(const data &other) const {
                if (*this == other) return 1;
                if (index != other.index) return index < other.index;
                return value < other.value;
            }
            bool operator >(const data &other) const {
                return !(*this <= other);
            }
            bool operator >=(const data &other) const {
                return !(*this < other);
            }
    };

    class BPlusNode {
        public:
            bool is_leaf;
            int size, id;
//            string keys[BLOCK_SIZE];
            int children[BLOCK_SIZE + 2]; // For internal nodes
//            int values[BLOCK_SIZE];       // For leaf nodes
			//data* xs;
			data key[BLOCK_SIZE + 1];
            //std::vector <data> xs;

        public:
            BPlusNode(int id = 0) : id(id) {
				size = 0; is_leaf = 0;
				for (int i = 0; i < BLOCK_SIZE; i++) key[i] = data();
				for (int i = 0; i < BLOCK_SIZE + 1; i++) children[i] = -1;
			}

			//add a son to this node, its key is data_ and node_id is id_
			void insert_node(data data_, int id_) {
				int now = size;
				for (int i = 0; i < size; i++)
					if (key[i] > data_) {now = i; break;}
				for (int i = size; i >= now; i--) {key[i + 1] = key[i]; children[i + 2] = children[i + 1];}
            	children[now + 1] = id_;
				key[now] = data_; size++;
			}

			//add a data on leaf, key is data_
			void insert_leaf_node(data data_) {
				int now = size;
				for (int i = 0; i < size; i++)
					if (key[i] > data_) {now = i; break;}
				for (int i = size - 1; i >= now; i--) key[i + 1] = key[i];
				key[now] = data_; children[size + 1] = children[size]; children[size] = 0; size++;
			}

    		void erase(int keyx, int childx) {
	            for (int i = keyx; i < size - 1; i++)
	            	key[i] = key[i + 1];
            	for (int i = childx; i < size; i++)
            		children[i] = children[i + 1];
            	size--;
            }
    };

    private:
		friend class USER;

        MEMORYRIVER_WITH_CACHE<BPlusNode, 2> node_storage; // Stores root position and next_pos
        int root_pos;
		int node_chain[MAX_DEG];

    public:
        FileStore(string s = "") {
            // head_file.initialise(s + "_head");
            // if (!std::filesystem::exists(s + "_body")) head = -1;
            //     else head_file.get_info(head, 1);
            // body_file.initialise(s + "_body");
        }

        void Init(string s) {
            // std::cerr << s << std::endl << s + "_head" << std::endl;
            if (!std::filesystem::exists(s)) node_storage.initialise(s), root_pos = -1;
                else node_storage.initialise(s), node_storage.get_info(root_pos, 1);
        	// std::cerr << "read root_pos = " << root_pos << std::endl;
        }

		void clear_all() {
	        node_storage.clear_all();
        }

        ~FileStore() {
        	// std::cerr << "write root_pos = " << root_pos << std::endl;
            node_storage.write_info(root_pos, 1);
        }

        BPlusNode read_node(int pos) {
            BPlusNode node;
            node_storage.read(node, pos);
            return node;
        }

        void write_node(const BPlusNode& node, int pos) {
            node_storage.update(const_cast<BPlusNode&>(node), pos);
        }

        int allocate_node(bool is_leaf) {
            BPlusNode new_node;
            new_node.is_leaf = is_leaf;
            new_node.size = 0;
            return node_storage.write(const_cast<BPlusNode&>(new_node));
        }

        int find_leaf(const indextype &index) {
	        node_chain[0] = 0;
        	// std::cerr << root_pos << std::endl;
        	if (root_pos == -1) return -1;

        	valuetype tmp;
        	BPlusNode now_point = read_node(root_pos);
        	// std::cerr << "qwq - qwq\n";
        	// std::cerr << "\n\n\n-------\n";
        	// std::cerr << index << std::endl;
        	while (!now_point.is_leaf) {
        		node_chain[++node_chain[0]] = now_point.id;
				int now = now_point.size;
        		// std::cerr << "now id = " << now_point.id << " is leaf ?" << now_point.is_leaf << std::endl;
        		for (int i = 0; i < now_point.size; i++) {
        			// std::cerr << "i = " << i << ", now in tree : " << now_point.key[i].index << " " <<  now_point.key[i].value << std::endl;
        			if (index <= now_point.key[i].index) {now = i; break;}
        		}
        		now_point = read_node(now_point.children[now]);
        	}
        	node_chain[++node_chain[0]] = now_point.id;
        	return now_point.id;
        }

		int find_leaf(const indextype &index, const valuetype &value) {
        	node_chain[0] = 0;
        	// std::cerr << root_pos << std::endl;
        	if (root_pos == -1) return -1;

        	data now_data(index, value);
        	BPlusNode now_point = read_node(root_pos);
        	// std::cerr << "qwq - qwq\n";
        	while (!now_point.is_leaf) {
        		node_chain[++node_chain[0]] = now_point.id;
        		int now = now_point.size;
        		// std::cerr << "now id = " << now_point.id << " is leaf ?" << now_point.is_leaf << std::endl;
        		if (now_point.id == 0) return -1;
        		// std::cerr << "size number = " << now_point.size << std::endl;
        		for (int i = 0; i < now_point.size; i++) {
        			// std::cerr << "now in tree : " << now_point.key[i].index << " " <<  now_point.key[i].value << std::endl;
        			if (now_data < now_point.key[i]) {now = i; break;}
        		}
        		now_point = read_node(now_point.children[now]);
        	}
        	node_chain[++node_chain[0]] = now_point.id;
        	return now_point.id;
        }

		void insert_in_parent(data key, int id) {
        	node_chain[0]--;
        	int parent_id = node_chain[node_chain[0]];
        	BPlusNode parent = read_node(parent_id);

        	parent.insert_node(key, id);
        	if (parent.size < BLOCK_SIZE) {
        		write_node(parent, parent_id);
        		return ;
        	}

        	BPlusNode new_parent;
        	new_parent.id = allocate_node(0);

        	new_parent.size = 0;
        	data key_new = parent.key[BLOCK_SIZE / 2];
        	for (int i = BLOCK_SIZE / 2 + 1; i < BLOCK_SIZE; i++) {
        		new_parent.children[new_parent.size] = parent.children[i];
        		new_parent.key[new_parent.size++] = parent.key[i];
        	}
        	new_parent.children[new_parent.size] = parent.children[BLOCK_SIZE];
        	parent.size = BLOCK_SIZE / 2;

        	write_node(parent, parent.id);
        	write_node(new_parent, new_parent.id);

			if (node_chain[0] == 1) {
				int new_root = allocate_node(0);
				BPlusNode root(new_root);
				root.size = 1;
				root.key[0] = key_new;
				root.children[0] = parent.id;
				root.children[1] = new_parent.id;
				root_pos = new_root;
				write_node(root, root_pos);
				return ;
			}

			insert_in_parent(key_new, new_parent.id);
		}

		void delete_parent(int id) {//delete id (we make sure that id-1 exist)
        	node_chain[0]--;
        	int now_node_id = node_chain[node_chain[0]];
        	BPlusNode now_node = read_node(now_node_id);
        	if (node_chain[0] == 1) {
        		if (!now_node.size) {
        			root_pos = -1;
        			return ;
        		}
				now_node.erase(id - 1, id);
        		write_node(now_node, now_node.id);
        		if (!now_node.size) {
					root_pos = now_node.children[0];
        		}
        		return ;
        	}

        	now_node.erase(id - 1, id);
        	if (now_node.size >= (BLOCK_SIZE - 1) / 2) {
        		write_node(now_node, now_node.id);
        		return ;
        	}

        	int parent_id = node_chain[node_chain[0] - 1];
        	BPlusNode parent = read_node(parent_id);

        	int now_son_id = -1;
        	for (int i = 0; i <= parent.size; i++)
        		if (parent.children[i] == now_node.id) {now_son_id = i; break;}
        	// std::cerr << "delete = " << now_son_id << std::endl;
        	// if (now_son_id == -1) {
        	// 	std::cerr << node_chain[0] << std::endl;
        	// 	for (int i = 1; i <= node_chain[0]; i++) std::cerr << node_chain[i] << " ";
        	// 	std::cerr << std::endl;
        	// 	std::cerr << "now_node.id = " << now_node.id << ", node_chain[node_chain[0]] = " << now_node_id << "\n";
        	// 	for (int i = 0; i <= parent.size; i++)
        	// 		std::cerr << "parent.children[" << i << "] = " << parent.children[i] << std::endl;
        	// }

        	BPlusNode left_brother, right_brother;
        	if (now_son_id == 0) {
        		right_brother = read_node(parent.children[now_son_id + 1]);
        		if (right_brother.size >= (BLOCK_SIZE - 1) / 2 + 1) {
        			data point = right_brother.key[0];//second element
        			int pos = right_brother.children[0];
        			data up_data = parent.key[now_son_id];//first element
        			right_brother.erase(0, 0);

        			now_node.key[now_node.size] = up_data; now_node.children[now_node.size + 1] = pos; now_node.size++;

        			parent.key[now_son_id] = point;
        			write_node(right_brother, right_brother.id);
        			write_node(now_node, now_node.id);
        			write_node(parent, parent.id);
        			return ;
        		}
        	}
        	else {
        		left_brother = read_node(parent.children[now_son_id - 1]);
        		if (left_brother.size >= (BLOCK_SIZE - 1) / 2 + 1) {
        			data point = left_brother.key[left_brother.size - 1];//last key
					int pos = left_brother.children[left_brother.size];
        			data up_data = parent.key[now_son_id - 1];
        			left_brother.erase(left_brother.size - 1, left_brother.size);

        			for (int i = now_node.size; i >= 1; i--) now_node.key[i] = now_node.key[i - 1];
        			for (int i = now_node.size + 1; i >= 1; i--) now_node.children[i] = now_node.children[i - 1];
        			now_node.key[0] = up_data; now_node.children[0] = pos; now_node.size++;

        			parent.key[now_son_id - 1] = point;
        			write_node(left_brother, left_brother.id);
        			write_node(now_node, now_node.id);
        			write_node(parent, parent.id);
        			return ;
        		}
        	}
			if (now_son_id == 0) {
				now_node.key[now_node.size] = parent.key[now_son_id]; now_node.children[now_node.size + 1] = right_brother.children[0]; now_node.size++;
				for (int i = 0; i < right_brother.size; i++) {
					now_node.key[now_node.size] = right_brother.key[i];
					now_node.children[now_node.size + 1] = right_brother.children[i + 1];
					now_node.size++;
				}
				write_node(now_node, now_node.id);
				delete_parent(now_son_id + 1);
			}
        	else {
        		left_brother.key[left_brother.size] = parent.key[now_son_id - 1]; left_brother.children[left_brother.size + 1] = now_node.children[0]; left_brother.size++;
        		for (int i = 0; i < now_node.size; i++) {
        			left_brother.key[left_brother.size] = now_node.key[i];
        			left_brother.children[left_brother.size + 1] = now_node.children[i + 1];
        			left_brother.size++;
        		}
        		write_node(left_brother, left_brother.id);
        		delete_parent(now_son_id);
        	}
        }


        void data_insert(const indextype index, const valuetype &value) {
        	// std::cerr << "in data_insert" << std::endl;
            int leaf_pos = find_leaf(index, value);
            if (leaf_pos == -1) {
                BPlusNode root;
                root.is_leaf = 1;
                root.size = 1;
                root.id = allocate_node(0);
				root.key[0] = (data){index, value};
                write_node(root, root.id);
                root_pos = root.id;
                return ;
            }

            BPlusNode leaf;
            leaf = read_node(leaf_pos);
        	// std::cerr << "leaf.id = " << leaf.id << " , leaf.size = " << leaf.size << std::endl;
        	leaf.insert_leaf_node((data){index, value});
            if (leaf.size < BLOCK_SIZE) {
				write_node(leaf, leaf_pos);
				return ;
			}

			//split to insert
        	// std::cerr << "Split node : " << index << " " <<value << std::endl;
			leaf.is_leaf = 1;
			BPlusNode new_leaf;
        	new_leaf.id = allocate_node(1);
        	new_leaf.is_leaf = 1;

			new_leaf.size = 0;
			for (int i = BLOCK_SIZE / 2; i < BLOCK_SIZE; i++) new_leaf.key[new_leaf.size++] = leaf.key[i];
        	new_leaf.children[new_leaf.size] = leaf.children[BLOCK_SIZE];
			leaf.size = BLOCK_SIZE / 2;
        	leaf.children[leaf.size] = new_leaf.id;
        	// std::cerr << leaf.size << " " << new_leaf.size << std::endl;

			// std::cerr << leaf.id << "is leaf ? " << leaf.is_leaf << " " << new_leaf.id << "is leaf ? " << new_leaf.is_leaf << std::endl;

        	write_node(leaf, leaf_pos);
        	write_node(new_leaf, new_leaf.id);

			if (node_chain[0] == 1) {
				int new_root = allocate_node(0);
				BPlusNode root(new_root);
				root.size = 1;
				root.key[0] = new_leaf.key[0];
				// std::cerr << "root id = " << new_root << std::endl;
				// std::cerr << "leaf.id = " << leaf.id << ", new_leaf.id = " <<new_leaf.id << std::endl;
				// std::cerr << "new root key[0] = " << root.key[0].index << " " << root.key[0].value << std::endl;
				root.children[0] = leaf.id;
				root.children[1] = new_leaf.id;
				root_pos = new_root;
				write_node(root, root_pos);
				// std::cerr << "write_node root done\n";
				return ;
			}

			data new_leaf_key = new_leaf.key[0];
			insert_in_parent(new_leaf_key, new_leaf.id);
        }

        void data_delete(const indextype &index, const valuetype &value) {
        	// std::cerr << "------------------\n";
        	if (root_pos == -1) return ;

        	// std::cerr << "root_pos = " << root_pos << std::endl; return ;
        	int leaf_pos = find_leaf(index, value);
        	// std::cerr << "leaf_pos = " << leaf_pos << std::endl;
        	BPlusNode leaf = read_node(leaf_pos);
        	int now = -1;
        	// std::cerr << "leaf_size = " << leaf.size << std::endl;
        	for (int i = 0; i < leaf.size; i++) {
        		// std::cerr << "index = " << leaf.key[i].index << "      value = " << leaf.key[i].value << std::endl;
        		if (leaf.key[i].index == index && leaf.key[i].value == value) {
					now = i; break;
        		}
        	}
        	// std::cerr << "now = " << now << std::endl;
        	if (now == -1) return ;
        	leaf.erase(now, now);

        	if (node_chain[0] == 1) {
        		if (leaf.size == 0) root_pos = -1;
        			else write_node(leaf, leaf.id);
        		return ;
        	}
        	if (leaf.size >= (BLOCK_SIZE - 1) / 2) {
        		write_node(leaf, leaf.id); return ;
        	}

        	//find whether brother can give
        	int parent_id = node_chain[node_chain[0] - 1];
        	BPlusNode parent = read_node(parent_id);
        	// std::cerr << "parent.id = " << parent.id << std::endl;

        	int now_son_id = -1;
        	for (int i = 0; i <= parent.size; i++)
        		if (parent.children[i] == leaf_pos) {now_son_id = i; break;}
        	// std::cerr << "delete leaves = " << now_son_id << std::endl;
        	BPlusNode left_brother, right_brother;
        	if (now_son_id == 0) {
        		right_brother = read_node(parent.children[now_son_id + 1]);
        		if (right_brother.size >= (BLOCK_SIZE - 1) / 2 + 1) {
        			// std::cerr << "op1";
        			data point = right_brother.key[0];
        			right_brother.erase(0, 0);

        			int next = leaf.children[leaf.size]; leaf.children[leaf.size] = 0;
        			leaf.key[leaf.size] = point; leaf.children[leaf.size + 1] = next; leaf.size++;

        			parent.key[now_son_id] = right_brother.key[0];
        			write_node(right_brother, right_brother.id);
        			write_node(leaf, leaf.id);
        			write_node(parent, parent.id);
        			return ;
        		}
        	}
        	else {
        		left_brother = read_node(parent.children[now_son_id - 1]);
        		// std::cerr << "parent.children[now_son_id - 1] = " << parent.children[now_son_id - 1] << std::endl;
        		if (left_brother.size >= (BLOCK_SIZE - 1) / 2 + 1) {
        			// std::cerr << "op2\n";
        			data point = left_brother.key[left_brother.size - 1];
        			left_brother.erase(left_brother.size - 1, left_brother.size - 1);

        			for (int i = leaf.size; i >= 1; i--) leaf.key[i] = leaf.key[i - 1];
        			for (int i = leaf.size + 1; i >= 1; i--) leaf.children[i] = leaf.children[i - 1];
        			leaf.key[0] = point; leaf.children[0] = 0; leaf.size++;

        			parent.key[now_son_id - 1] = leaf.key[0];
        			write_node(left_brother, left_brother.id);
        			write_node(leaf, leaf.id);
        			write_node(parent, parent.id);
        			return ;
        		}
        	}
			if (now_son_id == 0) {
				// std::cerr << "op3\n";
				for (int i = 0; i < right_brother.size; i++)
					leaf.key[leaf.size + i] = right_brother.key[i];
				leaf.children[leaf.size] = 0;
				leaf.size += right_brother.size;
				leaf.children[leaf.size] = right_brother.children[right_brother.size];
				write_node(leaf, leaf.id);
				delete_parent(now_son_id + 1);
			}
        	else {
        		// std::cerr << "op4\n";
        		// std::cerr << parent.id << " " << left_brother.id << " " << leaf.id << std::endl;
        		for (int i = 0; i < leaf.size; i++)
        			left_brother.key[left_brother.size + i] = leaf.key[i];
        		left_brother.children[left_brother.size] = 0;
        		left_brother.size += leaf.size;
        		left_brother.children[left_brother.size] = leaf.children[leaf.size];
        		write_node(left_brother, left_brother.id);
        		delete_parent(now_son_id);
        	}
        }

        vector <valuetype> data_find(const indextype &index) {
			vector <valuetype> result;
        	if (root_pos == -1) return result;

        	// std::cerr << "find leaf\n";
        	int leaf_pos = find_leaf(index);
        	// std::cerr << "leaf_pos = " << leaf_pos << std::endl;
        	if (leaf_pos == -1) return result;

        	BPlusNode leaf = read_node(leaf_pos);
			for (int i = 0; i < leaf.size; i++)
				if (leaf.key[i].index == index) {
					result.push_back(leaf.key[i].value);
        			// std::cerr << leaf.key[i].value << " ";
				}
        	// std::cerr << std::endl;
        	leaf_pos = leaf.children[leaf.size];
        	while (leaf_pos > 0) {
        		leaf = read_node(leaf_pos);
				bool yes = 1;
        		for (int i = 0; i < leaf.size; i++)
        			if (leaf.key[i].index == index) {
        				result.push_back(leaf.key[i].value);
        				// std::cerr << leaf.key[i].value << " ";
        			}
        			else {yes = 0; break;}
        		// std::cerr << std::endl;
        		if (!yes) break;
        		leaf_pos = leaf.children[leaf.size];
        	}

        	return result;
        }

        bool data_find_bool(const indextype &index) {
            vector <valuetype> res = data_find(index);
            return res.size() > 0;
        }
};


#endif


/* some cases
* 26
insert 1 1
insert 1 2
insert 1 3
insert 1 4
insert 1 5
insert 1 6
insert 1 7
insert 1 8
insert 1 9
insert 1 10
insert 1 11
insert 1 12
insert 1 13
insert 1 14
insert 1 15
insert 1 16
insert 1 17
insert 1 18
insert 1 19
insert 1 20
insert 1 21
insert 1 22
insert 1 23
insert 1 24
insert 1 25
find 1

*/