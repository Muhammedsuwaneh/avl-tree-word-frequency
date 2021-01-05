////////////////////////////////////////////////////////////////////////////////////////////
/*
  Algorithms and Data Structures
  DS: Storing Map ADTs in AVL Trees
  DS Application: Word Frequency 
  Contributor:
  Muhammed Suwaneh - 152120181098
  Department of Computer Engineering
  Eskisehir Osmangazi University, January 2021
  Eskisehir, Turkey
 */
 ////////////////////////////////////////////////////////////////////////////////////////////


#include "WordFreq.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <ctype.h>

using namespace std;;
///-----------------------------------------------------------------
/// Process the given text file and return the most frequent "k" words
///
/// 

// ! AVLNode 
struct AVLNode {

	AVLNode* left, *right;
	int height;
	WordFreq WordNode;
	AVLNode(WordFreq WordNode) : WordNode(WordNode), left(NULL), right(NULL) {};
};

AVLNode* root = NULL;

//! AVL operations
AVLNode* insertNode(AVLNode*, WordFreq);
AVLNode* balanceNode(AVLNode*);
int height(AVLNode*);
int height_difference(AVLNode*);

//! AVL rotations
AVLNode* llrotations(AVLNode*);
AVLNode* rrrotations(AVLNode*);
AVLNode* rlrotations(AVLNode*);
AVLNode* lrrotations(AVLNode*);


//! other operations - max frequency
void mostFrequentWordFreq(AVLNode*, int);
void inorderTraversal(AVLNode*, vector<vector<string>>&);

//! reading and AVL insertion operations
vector<WordFreq> GetMostFrequentKWords(string filename, int k) {
	vector<WordFreq> result;
	vector<string>temp_words, tokens;
	fstream FILE;
	string buffer;

	FILE.open(filename, ios::in);

	if (!FILE) {
		cout << "No such file does not exists" << endl;
		exit(1);
	}

	//! reading file and parsing strings
	else {

		// read file
		while (!FILE.eof()) {

			// obtain sentence/lines
			getline(FILE, buffer);

			// convert all characters to lower case and remove non alpha's
			for_each(buffer.begin(), buffer.end(), [](char& c) {
				c = ::tolower(c);
				if (!isalpha(c)) { c = ' '; }
			});

			//! save sentence/line in temp vector
			temp_words.push_back(buffer);
		}//! end-while
	}

	int i = 0;
	string temp;
	char delimiter = ' ';

	//! obtain all words with more than 3 letters
	while (i < temp_words.size()) {

		stringstream iss(temp_words[i]);
		while (getline(iss, temp, delimiter)) {

			if (temp.length() >= 3)
				tokens.push_back(temp);
		}//!end-while

		i++;
	}//!end-while

	//! Start AVL operations
	int j = 0;

	while (j < tokens.size()) {

		WordFreq wordNode(tokens[j]);
		wordNode.freq = 1;
		
		root = insertNode(root, wordNode);
		
		j++;
	}//!end-while

	//! Obtain k frequent words
	//! heighest frequency
	AVLNode* tempNode = root;
	int max = tempNode->WordNode.freq;

	//! obtain most frequent map node
	mostFrequentWordFreq(tempNode, max);
	int t = max, l = 0;
	vector<vector<string>>words(t+1);
	//! save all map nodes in vector slots
	inorderTraversal(root, words);

	for (int m = t; m >= 0; m--) {

		for (int n = 0; n < words[m].size(); n++) {

			WordFreq tempNode(words[m][n]);
			tempNode.freq = m;
			result.push_back(tempNode);
			l++;

			if (l == k) break;
		}

		if (l == k) break;
	}

	return result;
} //!end-GetMostFrequentKWords

//! insert element in AVL tree
AVLNode *insertNode(AVLNode* root, WordFreq wordNode) {

	if (root == NULL) {

		root = new AVLNode(wordNode);
	}

	else if (root->WordNode.word == wordNode.word) {
		root->WordNode.freq++;
	}

	else if (wordNode.word < root->WordNode.word) {

		root->left = insertNode(root->left, wordNode);
		root = balanceNode(root);
	}

	else if (wordNode.word > root->WordNode.word) {

		root->right = insertNode(root->right, wordNode);
		root = balanceNode(root);
	}

	return root;
}//!end-insertNode

//! check for a balance AVL Tree
AVLNode* balanceNode(AVLNode* tempNode) {

	int balance_factor = height_difference(tempNode);
	if(balance_factor > 1) {
		if (height_difference(tempNode->left) > 0) {
			tempNode = llrotations(tempNode);
		}
		else {
			tempNode = lrrotations(tempNode);
		}
	}
	else if(balance_factor < -1) {
		if (height_difference(tempNode->left) > 0) {
			tempNode = rlrotations(tempNode);
		}
		else {
			tempNode = rrrotations(tempNode);
		}
	}

	return tempNode;
}//!end-balanceNode

//! obtain the height of the AVL tree
int height(AVLNode* tempNode) {

	int treeHeight = 0;

	if (tempNode != NULL) {

		int leftside_height = height(tempNode->left);
		int rightside_height = height(tempNode->left);
		int maximum_height = max(leftside_height, rightside_height);
		treeHeight = maximum_height + 1;
	}

	return treeHeight;
}//!end-height

//! obtain balance factor of the AVL tree
int height_difference(AVLNode* tempNode) {

	int leftside_height = height(tempNode->left);
	int rightside_height = height(tempNode->left);
	int balance_factor = leftside_height - rightside_height;
	return balance_factor;
}//!end-height_difference

//! rotation operations starts here

//! Perform LL Imbalance Rotation - case 1
AVLNode* llrotations(AVLNode* parentNode) {

	AVLNode *tempNode;
	tempNode = parentNode->left;
	parentNode->left = tempNode->right;
	tempNode->right = parentNode;

	return tempNode;
}//!end-llrotations

//! Perform RR Imbalance Rotations - case 2
AVLNode* rrrotations(AVLNode* parentNode) {

	AVLNode* tempNode;
	tempNode = parentNode->right;
	parentNode->right = tempNode->left;
	tempNode->left = parentNode;
	return tempNode;
}

//! Perform LR Imbalance Rotations - case 3
AVLNode* lrrotations(AVLNode* parentNode) {

	AVLNode* tempNode;
	//! step 1
	tempNode = parentNode->left;
	parentNode->left = rrrotations(tempNode);
	return llrotations(tempNode); //!step 2
}//!end-lrrotations

//! Perform RL Imbalance Rotations - case 3
AVLNode* rlrotations(AVLNode* parentNode) {

	AVLNode* tempNode;
	//! step 1
	tempNode = parentNode->right;
	parentNode->right = llrotations(tempNode);
	return rrrotations(tempNode); //!step 2
}//!end-rlrotations

//! obtain most frequent words frequency
//! using post order traversals
void mostFrequentWordFreq(AVLNode* tempNode, int max) {

	if (tempNode == NULL)
		return;
	mostFrequentWordFreq(tempNode->left, max);
	mostFrequentWordFreq(tempNode->right, max);
	if (max < tempNode->WordNode.freq)
		max = tempNode->WordNode.freq;
}

//! save all map nodes in vector slots using inorder traversals
void inorderTraversal(AVLNode* tempNode, vector<vector<string>>& words) {

	if (tempNode == NULL)
		return;
	inorderTraversal(tempNode->left, words);
	words[tempNode->WordNode.freq].push_back(tempNode->WordNode.word);
	inorderTraversal(tempNode->right, words);
}