#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <sstream>

template <typename T>
class Node{
public:
	T val;
	Node<T>* left;
	Node<T>* right;

	Node(T i){
		viz_label = rand();
		val = i;
		left = nullptr;
		right = nullptr;
	};

	void Insert(T i){
		//TODO: <? <=?
		if(i <= val){
			//TODO: Case when you're actually inserting 0
			if(left == nullptr){
				MakeLeft(i);
			} else {
				left->Insert(i);
			}
		}

		if(i > val){
			if(right == nullptr){
				MakeRight(i);
			} else {
				right->Insert(i);
			}
		}
	}

	void RecursiveDelete(){
		if(right) right->RecursiveDelete();
		if(left) left->RecursiveDelete();
		delete this;
	}

	void MakeLeft(T i){
		left = new Node<T>(i);
	}

	void MakeRight(T i){
		right = new Node<T>(i);
	}

	void DeleteLeft(){
		delete left;
	}
	void DeleteRight(){
		delete right;
	}

	//https://upload.wikimedia.org/wikipedia/commons/3/31/Tree_rotation_animation_250x250.gif
	void RightRotate(){
		auto root = this;
		auto pivot = left;
		auto alpha = pivot->left;
		auto beta = pivot->right;
		auto gamma = right;

		root->left = beta;
	}

	Node<T>* Search(T item){
		if(val == item) return this;
		if(item < val && left) return left->Search(item);
		if(item > val && right) return right->Search(item);
		return nullptr;
	}

	void RecursiveApply(const std::function<T(T)>& f){
		if(left) left->RecursiveApply(f);
		if(right) right->RecursiveApply(f);
		val = f(val);
	}

	void RecursivePrint(){
		if(left) left->RecursivePrint();
		if(right) right->RecursivePrint();
		auto uniq = addr_parse(this);
		if(left){
			auto left_ident = addr_parse(left);
			std::cout << "\t{ " << uniq << " [label=\"" << val << "\"]} -> { " << left_ident << " [label=\"" <<  left->val << "\"]}" << std::endl; 
		}
		if(right){
			auto right_ident = addr_parse(right);
			std::cout << "\t{ " << uniq << " [label=\"" << val << "\"]} -> { " << right_ident << " [label=\"" <<  right->val << "\"]}" << std::endl; 
		}
	}


private:
	unsigned int viz_label;
	//this is so hacky
	std::string addr_parse(Node<T> *addr){

		std::stringstream ss;
		ss << addr;  
		std::string as_str = ss.str(); 

		as_str.erase(as_str.begin(),as_str.begin() + 2);
		for(char& c : as_str){
			if(isdigit(c)){
				c += 'A';
			}
		}

		return as_str;
	}
};

