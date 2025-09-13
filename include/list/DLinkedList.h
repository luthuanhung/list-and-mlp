/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T> {
 public:
  class Node;         // Forward declaration
  class Iterator;     // Forward declaration
  class BWDIterator;  // Forward declaration

 protected:
  Node *head;
  Node *tail;
  int count;
  bool (*itemEqual)(T &lhs, T &rhs);
  void (*deleteUserData)(DLinkedList<T> *);

 public:
  DLinkedList(void (*deleteUserData)(DLinkedList<T> *) = 0,
              bool (*itemEqual)(T &, T &) = 0);
  DLinkedList(const DLinkedList<T> &list);
  DLinkedList<T> &operator=(const DLinkedList<T> &list);
  ~DLinkedList();

  // Inherit from IList: BEGIN
  void add(T e);
  void add(int index, T e);
  T removeAt(int index);
  bool removeItem(T item, void (*removeItemData)(T) = 0);
  bool empty();
  int size();
  void clear();
  T &get(int index);
  int indexOf(T item);
  bool contains(T item);
  string toString(string (*item2str)(T &) = 0);
  // Inherit from IList: END

  void println(string (*item2str)(T &) = 0) {
    cout << toString(item2str) << endl;
  }
  void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0) {
    this->deleteUserData = deleteUserData;
  }

  bool contains(T array[], int size) {
    int idx = 0;
    for (DLinkedList<T>::Iterator it = begin(); it != end(); it++) {
      if (!equals(*it, array[idx++], this->itemEqual)) return false;
    }
    return true;
  }

  Iterator begin() { return Iterator(this, true); }
  Iterator end() { return Iterator(this, false); }

  BWDIterator bbegin() { return BWDIterator(this, true); }
  BWDIterator bend() { return BWDIterator(this, false); }

 protected:
  void copyFrom(const DLinkedList<T> &list);
  void removeInternalData();
  Node *getPreviousNodeOf(int index);

  //! FUNTION STATIC
 public:
  static void free(DLinkedList<T> *list) {
    typename DLinkedList<T>::Iterator it = list->begin();
    while (it != list->end()) {
      delete *it;
      it++;
    }
  }

 protected:
  static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)) {
    if (itemEqual == 0)
      return lhs == rhs;
    else
      return itemEqual(lhs, rhs);
  }

 public:
  class Node {
   public:
    T data;
    Node *next;
    Node *prev;
    friend class DLinkedList<T>;

   public:
    Node(Node *next = 0, Node *prev = 0) {
      this->next = next;
      this->prev = prev;
    }
    Node(T data, Node *next = 0, Node *prev = 0) {
      this->data = data;
      this->next = next;
      this->prev = prev;
    }
  };

 public:
  class Iterator {
   private:
    DLinkedList<T> *pList;
    Node *pNode;

   public:
    Iterator(DLinkedList<T> *pList = 0, bool begin = true) {
      if (begin) {
        if (pList != 0)
          this->pNode = pList->head->next;
        else
          pNode = 0;
      } else {
        if (pList != 0)
          this->pNode = pList->tail;
        else
          pNode = 0;
      }
      this->pList = pList;
    }

    Iterator &operator=(const Iterator &iterator) {
      this->pNode = iterator.pNode;
      this->pList = iterator.pList;
      return *this;
    }
    void remove(void (*removeItemData)(T) = 0) {
      pNode->prev->next = pNode->next;
      pNode->next->prev = pNode->prev;
      Node *pNext = pNode->prev;  // MUST prev, so iterator++ will go to end
      if (removeItemData != 0) removeItemData(pNode->data);
      delete pNode;
      pNode = pNext;
      pList->count -= 1;
    }

    T &operator*() { return pNode->data; }
    bool operator!=(const Iterator &iterator) {
      return pNode != iterator.pNode;
    }
    // Prefix ++ overload
    Iterator &operator++() {
      pNode = pNode->next;
      return *this;
    }
    // Postfix ++ overload
    Iterator operator++(int) {
      Iterator iterator = *this;
      ++*this;
      return iterator;
    }
  };

  class BWDIterator {
    // TODO implement
  private:
    Node *currentNode;
    DLinkedList<T> *pList;
  public:
    BWDIterator(DLinkedList<T> *pList=0,bool begin=true){
      if(begin){
        if(pList!=0){
          this->currentNode=pList->tail->prev;
        }
        else currentNode=0;
      } 
      else{
        if(pList!=0){
          this->currentNode=pList->head;
        }
        else{
          currentNode=0;
        }
      }
      this->pList=pList;
    }
    BWDIterator operator=(const BWDIterator &iterator){
      this->pList=iterator.pList;
      this->currentNode=iterator.currentNode;
      return *this;
    }
    T &operator*(){
      return currentNode->data;
    }
    bool operator!=(const BWDIterator &iterator){
      return currentNode!=iterator.currentNode;
    }
    BWDIterator &operator++(){
      currentNode = currentNode->prev;
      return *this;
    }
    BWDIterator operator++(int){
      BWDIterator BWDiterator = *this;
      ++*this;
      return BWDiterator;
    }
    void remove(void (*removeItemData)(T) = 0) {
      currentNode->prev->next = currentNode->next;
      currentNode->next->prev = currentNode->prev;
      Node *currentNext = currentNode->next;  // MUST prev, so iterator-- will go to begin
      if (removeItemData != 0) removeItemData(currentNode->data);
      delete currentNode;
      currentNode = currentNext;
      pList->count -= 1;
    }
  };
};

template <class T>
using List = DLinkedList<T>;

//! ////////////////////////////////////////////////////////////////////
//! //////////////////////     METHOD DEFNITION      ///////////////////
//! ////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(void (*deleteUserData)(DLinkedList<T> *),
                            bool (*itemEqual)(T &, T &)) {
  // TODO implement
  head=new Node();
  tail=new Node();
  head->next=tail;
  tail->prev=head;
  this->count=0;
  this->deleteUserData=deleteUserData;
  this->itemEqual=itemEqual;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list) {
  // TODO implement
  if(this!=&list){
    head=new Node();
    tail=new Node();
    head->next=tail;
    tail->prev=head;
    copyFrom(list);
  }
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list) {
  // TODO implement
  if(this!=&list){
    removeInternalData();
    copyFrom(list);
  }
  return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList() {
  // TODO implement
  removeInternalData();
  delete head;
  delete tail;
  this->deleteUserData=nullptr;
  this->itemEqual=nullptr;
}

template <class T>
void DLinkedList<T>::add(T e) {
  // TODO implement
  Node* newnode=new Node(e,tail,tail->prev);
  tail->prev->next=newnode;
  tail->prev=newnode;
  ++count;
}
template <class T>
void DLinkedList<T>::add(int index, T e) {
  // TODO implement
  if(index<0 || index>count){
    throw std::out_of_range("Index is out of range!");
  }
  Node* mark=getPreviousNodeOf(index);
  Node* newnode=new Node(e,mark->next,mark);
  mark->next->prev=newnode;
  mark->next=newnode;
  ++count;
}

template <class T>
T DLinkedList<T>::removeAt(int index) {
  // TODO implement
  if(index<0 || index>=count){
    throw std::out_of_range("Index is out of range!");
  }
  Node* mark=getPreviousNodeOf(index);
  Node* removenode=mark->next;
  T val=removenode->data;
  mark->next=mark->next->next;
  mark->next->prev=mark;
  delete removenode;
  --count;
  return val;
}

template <class T>
bool DLinkedList<T>::empty() {
  // TODO implement
  return !this->count;
}

template <class T>
int DLinkedList<T>::size() {
  // TODO implement
  return this->count;
}

template <class T>
void DLinkedList<T>::clear() {
  // TODO implement
  removeInternalData();
}

template <class T>
T &DLinkedList<T>::get(int index) {
  // TODO implement
  if(index<0 || index>=count){
    throw std::out_of_range("Index is out of range!");
  }
  Node* mark=getPreviousNodeOf(index);
  return mark->next->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item) {
  // TODO implement
  Node* mark=head->next;
  int pos=0;
  while(mark!=tail){
    if(equals(mark->data,item,this->itemEqual)) return pos;
    mark=mark->next;
    ++pos;
  }
  return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T)) {
  // TODO implement
  Node* mark=head->next;
  int pos=0;
  while(mark!=tail){
      if(equals(item,mark->data,this->itemEqual)){
        if(removeItemData!=nullptr)removeItemData(mark->data);
        mark->prev->next=mark->next;
        mark->next->prev=mark->prev;
        delete mark;
        --count;
        return true;
      }
      mark=mark->next;
      ++pos;
  }
  return false; 
}

template <class T>
bool DLinkedList<T>::contains(T item) {
  // TODO implement
  if(this->indexOf(item)==-1) return false;
  else return true;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &)) {
  // TODO implement
  string result="[";
  //ss<<"[";
  Node* mark=head->next;
  if(item2str!=nullptr){
    while(mark!=tail){
      string word;
      word=item2str(mark->data);
      result+=word;
      if(mark->next!=tail)result+=", ";//ss<<", ";
      mark=mark->next;
    }
  }
  else{
    while(mark!=tail){
      stringstream ss;
      ss<<mark->data;
      result+=ss.str();
      if(mark->next!=tail)result+=", ";//ss<<", ";
      mark=mark->next;
    }
  }
  //ss<<"]";
  result+="]";
  return result;
}

//! ////////////////////////////////////////////////////////////////////
//! ////////////////////// (private) METHOD DEFNITION //////////////////
//! ////////////////////////////////////////////////////////////////////
template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list) {
  /**
   * Copies the contents of another doubly linked list into this list.
   * Initializes the current list to an empty state and then duplicates all data
   * and pointers from the source list. Iterates through the source list and
   * adds each element, preserving the order of the nodes.
   */
  // TODO implement
  this->itemEqual=list.itemEqual;
  this->deleteUserData=list.deleteUserData;
  this->count=list.count;
  Node* root=list.head->next;
  while(root!=list.tail){
    Node* temp=new Node(root->data,tail,tail->prev);
    tail->prev->next=temp;
    tail->prev=temp;
    root=root->next;
  }
}

template <class T>
void DLinkedList<T>::removeInternalData() {
  /**
   * Clears the internal data of the list by deleting all nodes and user-defined
   * data. If a custom deletion function is provided, it is used to free the
   * user's data stored in the nodes. Traverses and deletes each node between
   * the head and tail to release memory.
   */
  // TODO implement
  if(deleteUserData!=nullptr) deleteUserData(this);
  Node* mark=head->next;
  while(mark!=tail){
    head->next=head->next->next;
    delete mark;
    mark=head->next;
  }
  tail->prev=head;
  this->count=0;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index) {
  /**
   * Returns the node in the specified index in the doubly linked list.
   * If the index is in the first half of the list, it traverses from the head;
   * otherwise, it traverses from the tail. Efficiently navigates to the node by
   * choosing the shorter path based on the index's position.
   */
  // TODO implement
  if(index<0 || index>count){
    throw std::out_of_range("Index is out of range!");
  }
  Node* mark;
  if(index<count/2){
    mark=head;
    for(int i=-1;i<index-1;i++){
      mark=mark->next;
    }
  }
  else{
    mark=tail;
    for(int i=count;i>=index;i--){
      mark=mark->prev;
    }  
  }
  return mark;
}

#endif /* DLINKEDLIST_H */
