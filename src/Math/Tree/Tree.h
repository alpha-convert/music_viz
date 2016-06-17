#pragma once
#include <functional>
#include <cstdio>
#include <iostream>
#include "Node.h"

template <typename T>
class AVLTree{
public:
	AVLTree(T rootval){
		root = new Node<T>(rootval);
	};

	AVLTree(){
		root = nullptr;
	};

	~AVLTree(){
		if(root) root->RecursiveDelete();
	};

	unsigned int Height();

	void Balance();

	void Insert(T item){
		root->Insert(item);
	};

	bool Delete(T item){
		auto item_ptr = Search(item);
		if(item_ptr){
			delete item_ptr;
			return true;
		}
		return false;
	};

	Node<T>* Search(T item){
		return root->Search(item);
	}

	void Apply(const std::function<T(T)>& f){
		root->RecursiveApply(f);
	}

	void Print(){
		printf("digraph G{\n");
		printf("graph [ordering=\"out\"];\n");
		root->RecursivePrint();
		printf("}\n");
	};
	

	Node<T>* root;

};
