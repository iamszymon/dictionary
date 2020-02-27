#include<iostream>
#include <string>
#include <math.h>
#include <iomanip>
using namespace std;
template <typename key, typename info>
class TreeDictionary {
	struct Node {
		key id;
		info data;
		Node *parent;
		Node *left;
		Node *right;
		int height;
	};
	Node *root;
	void erase(Node *node) {
		if (node == NULL) 
			return;
		erase(node->left);
		erase(node->right);
		
		delete node;
	}
	Node *newNode(key keyy, info data)
	{
		Node *temp = new Node;
		temp->id = keyy;
		temp->data = data;
		temp->left = temp->right = NULL;
		temp->parent = NULL;
		return temp;
	}

	Node *findInfo(Node *node, info data){
		if (node == NULL) 
		return NULL;
		else if (node->data == data) 
		return node;
		else if (data > node->data) 
		return findInfo(node->right, data);
		return findInfo(node->left, data);
	}
	Node *findKey(Node *node, key keyy){
		if (node == NULL) return NULL;
		else if (node->id == keyy) return node;
		else if (keyy > node->id) return findKey(node->right, keyy);
		return findKey(node->left, keyy);
	}

	int height(Node *node) {
		return node == NULL ? -1 : node->height;
	}
	
	Node *add(key keyy, info data, Node *node)
	{
		if (node == NULL)
		{
			node = newNode(keyy, data);
			node->id = keyy;
			node->data = data;
			node->height = 0;
			node->left = node->right = NULL;
		}
		else if (keyy < node->id) 
		{
			Node *leftChild = add(keyy, data, node->left);
			node->left = leftChild;
			leftChild->parent = node;
			if (height(node->left) - height(node->right) == 2) {
				if (keyy < node->left->id)
					node = rotateRight(node);
				else
					node = rotateRightTwice(node);
			}
		}
		else if (keyy > node->id) 
		{
			Node *rightChild = add(keyy, data, node->right);
			node->right = rightChild;
			rightChild->parent = node;
			if (height(node->right) - height(node->left) == 2) {
				if (keyy > node->right->id)
					node = rotateLeft(node);
				else
					node = rotateLeftTwice(node);
			}
		}
	
		node->height = max(height(node->left), height(node->right)) + 1;
		return node;
	}
	
	int getBalance(Node *node){
		return node == NULL ? 0 : height(node->left) - height(node->right);
	}
	Node *findMinInSubTree(Node *node)
	{
		if (node == NULL) return NULL;
		else if (node->left == NULL) return node;
		else return findMinInSubTree(node->left);
	}
	Node *findMaxInSubTree(Node *node)
	{
		if (node == NULL) return NULL;
		else if (node->right == NULL) return node;
		else return findMaxInSubTree(node->right);
	}
	Node *rotateRight(Node *&node) 
	{
		Node *tmp = node->left;
		node->left = tmp->right;
		if (tmp->right != NULL) tmp->right->parent = node;
		tmp->parent = node->parent;
		tmp->right = node;
		node->parent = tmp;

		node->height = max(height(node->left), height(node->right)) + 1;
		tmp->height = max(height(tmp->left), node->height) + 1;
		return tmp;
	}

	Node *rotateLeft(Node *&node) 
	{
		Node *tmp = node->right;
		node->right = tmp->left;
		if (tmp->left != NULL)
			tmp->left->parent = node;
		tmp->parent = node->parent;
		tmp->left = node;
		node->parent = tmp;


		node->height = max(height(node->left), height(node->right)) + 1;
		tmp->height = max(height(node->right), node->height) + 1;
		return tmp;
	}

	Node *rotateLeftTwice(Node *&node)
	{
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}

	Node *rotateRightTwice(Node *&node)
	{
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}

	int max(int a, int b)
	{
		if (a > b) return a;
		return b;
	}

	Node *remove(key keyy, info data, Node *node)
	{
		Node *temp;

		if (node == NULL) return NULL;
		else if (keyy < node->id) node->left = remove(keyy, data, node->left);
		else if (keyy > node->id) node->right = remove(keyy, data, node->right);

		else if (node->left && node->right) 
		{
			temp = findMinInSubTree(node->right);
			node->id = temp->id;
			node->data = temp->data;
			node->right = remove(node->id, node->data, node->right);
		}
		else
		{
			temp = node;
			if (node->left == NULL) node = node->right;
			else if (node->right == NULL) node = node->left;
			delete temp;
		}
		if (node == NULL) return node;

		node->height = max(height(node->left), height(node->right)) + 1;

		if (height(node->left) - height(node->right) == 2)
		{
			if (height(node->left->left) - height(node->left->right) == 1) return rotateLeft(node);
			else return rotateLeftTwice(node);
		}
		else if (height(node->right) - height(node->left) == 2)
		{
			if (height(node->right->right) - height(node->right->left) == 1) return rotateRight(node);
			else return rotateRightTwice(node);
		}
		return node;
	}
	void printInOrder(ostream& os, Node* n) const{
		if (n == NULL) {
			return;
		}
		printInOrder(os, n->left);
		os << "[" << n->id << "]" << "" << n->data << " ";
		printInOrder(os, n->right);
	}
	void display(Node *node, int free)
	{
		int counter = 10;
		if (node == NULL) return;
		free += counter;
		display(node->right, free);

		cout <<endl;
		for (int i =counter; i< free; i++)cout<< " ";
		cout <<node->id << "  " << node->data << endl;
		display(node->left, free); 
	}

public:
 template <typename Key, typename Info>
	class Iterator {
		Node *iteratt;
	public:
		Iterator(){ 
		iteratt = NULL;
		}
		~Iterator() {}
		Iterator(Node *element){
			iteratt = element;
		}
		Iterator(const Iterator &cc) { iteratt = cc.iteratt; }
		Iterator operator=(const Iterator &iterator1)
		{
			if (this == &iterator1) return *this;
			iteratt = iterator1.iteratt;
			return *this;
		}
		Iterator operator+(int length)
		{
			if (length > 0)for (int i = 0; iteratt && iteratt++ && i < length; i++) iteratt = iteratt++;
			return *this;
		}
		Iterator &operator++()
		{
			Node *p;

			if (iteratt == NULL) 
			{
				if (iteratt == NULL) return NULL;
				while (iteratt->left != NULL)
				{
					iteratt = iteratt->left;
				}
			}
			else if (iteratt->right != NULL)
			{
				iteratt = iteratt->right;

				while (iteratt->left != NULL)
				{
					iteratt = iteratt->left;
				}
			}
			else
			{
				p = iteratt->parent;
				while (p != NULL && iteratt == p->right) 
				{
					iteratt = p;
					p = p->parent;
				}
				iteratt = p;
			}
			return *this;
		}
	const Iterator operator++(int)
		{
			if (!iteratt) return *this;
			else
			{
				Node *p;
				Iterator *temporary = new Iterator(iteratt);
				if (iteratt == NULL)
				{
					while (iteratt->left != NULL) 
					{
						iteratt = iteratt->left;
					}
				}
				else if (iteratt->right != NULL)
				{
					iteratt = iteratt->right;

					while (iteratt->left != NULL) 
					{
						iteratt = iteratt->left;
					}
				}
				else {
					p = iteratt->parent;
					while (p != NULL && iteratt == p->right) {
						iteratt = p;
						p = p->parent;
					}
					iteratt = p;
				}
				return *temporary;
			}
		}
		
		Iterator operator-(int length) 
		{
			if (length > 0)for (int i = 0; iteratt && iteratt++ && i < length; i++) iteratt--;
			return *this;
		}
		const Iterator operator--(int) 
		{
			if (!iteratt) return *this;
			else {
				Node *p;
				Iterator *temporary = new Iterator(iteratt);
				if (iteratt == NULL) 
				{
					while (iteratt->right != NULL) 
					{
						iteratt = iteratt->right;
					}
				}
				else if (iteratt->left != NULL) 
				{
					iteratt = iteratt->left;

					while (iteratt->right != NULL)
					{
						iteratt = iteratt->right;
					}
				}
				else
				{
					p = iteratt->parent;
					while (p != NULL && iteratt == p->left)
					{
						iteratt = p;
						p = p->parent;
					}
					iteratt = p;
				}
				return *temporary;
			}
		}
		Iterator &operator--() {
			Node *p;

			if (iteratt == NULL)
			{
				if (iteratt == NULL) return NULL;
				while (iteratt->right != NULL)
				{
					iteratt = iteratt->right;
				}
			}
			else if (iteratt->left != NULL) 
			{
				iteratt = iteratt->left;

				while (iteratt->right != NULL)
				{
					iteratt = iteratt->right;
				}
			}
			else 
			{
				p = iteratt->parent;
				while (p != NULL && iteratt == p->left)
				{
					iteratt = p;
					p = p->parent;
				}
				iteratt = p;
			}
			return *this;
		}

		bool operator==(Iterator iterator1) const{ 
		return iteratt == iterator1.iteratt; 
		}
		bool operator!=(Iterator iterator1) const{ 
		return iteratt != iterator1.iteratt; 
		}
		Node &operator*() const{ 
		return *iteratt; 
		}
		Node *operator->() const{ 
		return iteratt; 
		}
		key getKey(){ 
		return iteratt->id; 
		}
		info getValue(){ 
		return iteratt->data; 
		}
		friend ostream &operator<<(ostream &o, const Iterator &iterator2){
			o << *iterator2;
			return o;
		}
	};

	typedef Iterator<key, info> dictionaryIterator;
	dictionaryIterator begin(){
		Node *curr = root;
		if (curr != NULL)
			while (curr->left != NULL)
				curr = curr->left;
		return dictionaryIterator(curr);
	}

	dictionaryIterator end(){
		return dictionaryIterator(NULL);
	}

	dictionaryIterator last(){
	return root ? dictionaryIterator(findMaxInSubTree(root)) : dictionaryIterator(NULL); 
	}

	dictionaryIterator find(const key &keyy){
		for (dictionaryIterator iterator2 = begin(); iterator2 != last(); ++iterator2) 
		if (iterator2->id == keyy) 
			return iterator2;
		return dictionaryIterator(NULL);
	}

	TreeDictionary(){
		root = NULL;
	}

	TreeDictionary(TreeDictionary &tree){
		root = NULL;
		
		for (dictionaryIterator iteratt = tree.begin(); iteratt != tree.end(); iteratt++)
		{
			add(iteratt->id, iteratt->data);
		}
	}

	void add(key keyy, info i){
		root = add(keyy, i, root);
	}

	void remove(key keyy){
		info i =searchKey(keyy)->data;
		root =remove(keyy, i, root);
	}

	Node *searchKey(key keyy){
		return findKey(root, keyy);
	}

	Node *searchValue(info data){
		return findInfo(root, data);
	}

	void display(){
		if (root == NULL) 
		cout << "It's empty!'" << endl;
		display(root, 1);
	}
	
	TreeDictionary& operator=(TreeDictionary<key, info>& t) {
		if (this == &t) {
			return *this;
		}
		erase(root);
		root = NULL;
		for (dictionaryIterator iteratt = t.begin(); iteratt != t.end(); iteratt++)
		{
			add(iteratt->kinf, iteratt->data);
		}
		return *this;
	}
	
	key operator()(info data) 
	{
		if (searchValue(data) == NULL)
		{
			throw invalid_argument("Doesn't exist!'");
		}
		key keyy = searchValue(data)->id;
		return keyy;
	}
	friend bool operator==(TreeDictionary &tree1, TreeDictionary &tree2)
	{
		if (tree2.root == NULL && tree1.root == NULL) return true;
		dictionaryIterator iteratt = tree2.begin();
		for (dictionaryIterator itTree = tree1.begin(); itTree != tree1.end(); itTree++)
		{
			if (iteratt == NULL && itTree == NULL) return true;
			if (iteratt == NULL || itTree == NULL) return false;
			if (iteratt->key != itTree->id && iteratt->data != itTree->data) return false;
			iteratt++;
		}
		return true;
	}
	info operator[](key keyy)
	{
		if (searchKey(keyy) == NULL) 
		{
			throw invalid_argument("Didn't find key");
		}
		info val = searchKey(keyy)->data;
		return val;
	}
	friend ostream& operator<<(ostream& output, const TreeDictionary<key, info>& src)
	{
		src.printInOrder(output, src.root);
		return output;
	}
};

int main() {
	TreeDictionary<int, string> dictionarytree;
	dictionarytree.add(1, "first" );
	dictionarytree.add(2, "second");
	dictionarytree.add(3, "third");
	dictionarytree.add(4, "fourth");
	dictionarytree.add(4, "fourth1");
	dictionarytree.add(6, "sixth");
	dictionarytree.add(7, "seventh");
	dictionarytree.display();
	cout << endl << endl;
	dictionarytree.remove(1);
	cout << dictionarytree("dictionary") << "  " << dictionarytree[2] << endl << endl << endl;
	TreeDictionary<int, string> second = dictionarytree;
	second.display();
	TreeDictionary<int, string> third(dictionarytree);
	third.add(10, "tenten");
	third.display();
	second = third;
	cout << second<<endl;
	return 0;
}
