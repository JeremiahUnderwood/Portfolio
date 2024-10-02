#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

class Node{
	public:
		int data;
		Node *next;
		
		Node(int initData){
			data = initData;
		}	
};

class LinkedList{
	private:
		Node head;
		int size;
		
	public:
		LinkedList(int initData): head(initData){ //since head is initialised when called it has to be done like this for some reason.	
			size = 0;
		}	
		
		int GetSize(){
			return size + 1;
		}
		
		void Append(int value){
			Node* place = &head;
			for (int i = 0; i < size; i++){
				place = place->next;
			}
			size++;
			place->next = new Node(value);
		}
		
		int Remove(int itr){
			//head.next->data = 1; dont do this you fucking with yo memory!!!
			if (itr > size){
				return 0;
			}
			if (itr < 0){
				return 0;
			}
			
			if (itr == 0){
				Node* headPointer = &head;
				int returnValue = headPointer->data;
				Node *ptrHold = headPointer->next;
				delete headPointer;
				head = *ptrHold;
				size--;
				return returnValue;
				
			}
			
			Node* prevPlace = NULL;
			Node* place = &head;
			for (int i = 0; i < itr; i++){
				prevPlace = place;
				place = place->next;
			}
			int returnValue = place->data;
			prevPlace->next = place->next;
			delete place;
			size--;
			return  returnValue;
		}
		
		int Pop(){
			return Remove(size);
		}
		
		int Insert(int value, int itr){ //int return type so I can return to end the function
		
			if (itr > size + 1){
				return 0;
			}
			if (itr < 0){
				return 0;
			}
			Node* nextPlace = &head;
			if (itr == 0){				
				Node* headClone = new Node(head.data);
				headClone->next = head.next;											
				head = Node(value);				
				head.next = headClone;
				size++;				
				return 0;
			}
			else{				
				Node* prevPlace = nextPlace;
				nextPlace = nextPlace->next;
				for (int i = 1; i < itr; i++){
					prevPlace = nextPlace;
					nextPlace = nextPlace->next;
				}
				prevPlace->next = new Node(value);
				prevPlace->next->next = nextPlace;
				size++;
				return 0;
			}
		}
		
		int GetValue(int itr){
			//head.next->data = 1; dont do this you fucking with yo memory!!!
			if (itr > size){
				return 0;
			}
			Node* place = &head;
			for (int i = 0; i < itr; i++){
				place = place->next;
			}
			return place->data;
		}
		
		void PrintList(){
			for (int i = 0; i <= size; i++){
				cout << GetValue(i) << "\n";
			}
		}
};

int main(int argc, char *argv[]){
	cout << " " << "\n";
	LinkedList listyboi = LinkedList(100);
	listyboi.Append(6);
	listyboi.PrintList();
	listyboi.Insert(200, 0);
	listyboi.Insert(54, 1);
	listyboi.Insert(55, 4);
	listyboi.Remove(4);
	listyboi.Remove(2);
	listyboi.Remove(0);
	listyboi.Append(89);
	printf("\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\n");
	listyboi.PrintList();
	
	
	return 0;
}
