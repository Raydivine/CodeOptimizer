#include "NodeChain.h"
#include <stdlib.h>
#include <stdio.h>

Node* createNode(int thisRank){
  Node* newNode = malloc(sizeof(Node));
  
  newNode->rank         = thisRank;
  newNode->visitFlag    = 0;
  newNode->block        = createLinkedList();
  newNode->parent       = NULL;
  newNode->lastBrhDom   = NULL;
  newNode->imdDom       = NULL;
  newNode->numOfDom     = 0;
  newNode->doms         = NULL;
  newNode->numOfChild   = 0;
  newNode->children     = NULL;
  
  return newNode;
}


/*
 * brief @ Add a new child to a node as shown below.
 * Example:
 *              Before      [b]         After
 *           [a] rank 0    ---->       [a] rank 0
 *                                      |
 *                                     [b] rank 1
 *
 * brief @ The imdChild of the node(Node a in example) will assign to the added child(Node b in example).
 * brief @ The imdParent of the added child(Node b in example) will assign to the node(Node a in th example).
 *
 * param @ parentNode - The node that is going to add a new child(imdChild), it also will be the imdParent
 *                       of the child.
 * param @ childNode - The node that is going to be added to a node(imdParent), it also will be the imdChild
 *                       of the node.
 * retval@ void  - Nothing to return.
 */
void addChild(Node** parentNode, Node** childNode){
  if(*parentNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input parent node is NULL!");
  
  if(*childNode == NULL)
    ThrowError(ERR_NULL_NODE, "Input child node is NULL!");

  /***************************************************
  *  Link Child to parentNode                       *
  ***************************************************/
  (*parentNode)->numOfChild++;
  (*parentNode)->children = (Node**)realloc((*parentNode)->children, \
                            (sizeof(Node*) * ((*parentNode)->numOfChild)));
                            
  (*parentNode)->children[((*parentNode)->numOfChild) - 1] = *childNode;
  
  /***************************************************
  *  Link Parent to childNode                       *
  ***************************************************/
  (*childNode)->parent = *parentNode;
  
}

void setLastBrhDom(Node** rootNode){
  if(*rootNode == NULL)
    ThrowError(ERR_NULL_NODE, "Empty Tree input detected!");
  
  LinkedList* tempList      = assembleList(rootNode);
  ListElement* tempElement  = tempList->head;
  Node* tempNode            = tempElement->node;
  int i, k;
  Node *highRankNode, *testRankNode_1, *testRankNode_2;

/****************************************************
 *  With the LinkedList, find and assign lastBrhDom *
 *  of each node in the Node tree                   *
 ****************************************************/
  while(tempElement != NULL){
    tempNode = tempElement->node;
    for(i = 0; i < tempNode->numOfChild; i++){
      //handle ROOT Node
      if(tempNode->parent == NULL){
        tempNode->lastBrhDom = NULL;
      }//handle child with 1 parent only (no assign to lastBrhDom yet)
      if(tempNode->children[i]->lastBrhDom == NULL){
        if(tempNode->numOfChild > 1 || tempNode->parent == NULL)
          tempNode->children[i]->lastBrhDom = tempNode;
        else if(tempNode->lastBrhDom != NULL)
          tempNode->children[i]->lastBrhDom = tempNode->lastBrhDom;
      }//handle child with more than 1 parent (lastBrhDom assigned by prev parents)
      else{
        testRankNode_1 = tempNode->lastBrhDom;
        testRankNode_2 = tempNode->children[i]->lastBrhDom;
        FIND_SAME_NODE(testRankNode_1, testRankNode_2);
        tempNode->children[i]->lastBrhDom = testRankNode_1;
      }
    }
    tempElement = tempElement->next;
  }
}

void getImdDom(Node* nodeA){
  if(nodeA->rank == 0)
    nodeA->imdDom = NULL;
  else{
    int i;
    Node *boudariesNode       = nodeA->lastBrhDom;
    LinkedList* tempList      = assembleList(&boudariesNode);
    ListElement* tempElement  = tempList->head;
    Node *tempNode;
    /*************************************************
    * Find the imdDominator and return it           *
    *************************************************/
    while(tempElement != NULL){
      tempNode = tempElement->node;
      for(i = 0; i < tempNode->numOfChild; i++){
        if(tempNode->children[i] == nodeA){
          if(nodeA->imdDom == NULL)
            nodeA->imdDom = tempNode;
          else{
            if(tempNode != nodeA)
              nodeA->imdDom = nodeA->lastBrhDom; 
          }
        }
      }
      tempElement = tempElement->next;
    }
  }
}

void setAllImdDom(Node** rootNode){
  LinkedList* tempList      = assembleList(rootNode);
  ListElement* tempElement  = tempList->head;
  Node *tempNode;
  /*************************************************
  * Find the imdDominator and return it           *
  *************************************************/
  while(tempElement != NULL){
    tempNode = tempElement->node;
    getImdDom(tempNode);
    tempElement = tempElement->next;
  }
}


/*
 * brief @ To get The dominatorFrontiers of a node.
 * Example:
 *
 *          | Entry                 DF(A) = { A }
 *         \/                       DF(B) = { D } 
 *        [ A ]<<<<                 DF(C) = { D }
 *       |   |    /\                DF(D) = { A }
 *      \/  \/    /\
 *     [B]  [C]   /\
 *      |    |    /\
 *     \/   \/    /\
 *       [D] >>>>>>>
 *
 *
 * brief @ There is three condition to be a dominatorFrontier of the node:
 *          1) the node(or the children of the node) must be an immediate predeccessor of domFrontier.
 *          2) (if any) to get domFrontiers from grandchild(child of child), the child must be dominated by the node
 *          3)domFrontier is not strictly dominated by the node
 *
 * param @ Node* node  - The node that is going to use this function to find the domFrontiers of it.
 *
 * retval@ LinkedList* - The list of domFrontiers of the 'Node* node' is going to return.
 */
LinkedList* getNodeDomFrontiers(Node* node){
  
  LinkedList* domFrontiers = createLinkedList();
  LinkedList* checklist = createLinkedList();
  ListElement* tempCLElement = NULL;
  ListElement* tempHeadCL = NULL;
  Node *tempImdDom = NULL;
  int i = 0;

  if(!node->numOfChild)
    return domFrontiers;
  
  addListLast(checklist, node);
  tempHeadCL = checklist->head;

 while(tempHeadCL){
    
    for( i = 0; i < ((Node*)tempHeadCL->node)->numOfChild; i++){
      
      tempImdDom = ((Node*)tempHeadCL->node)->children[i]->imdDom;
      
      //checking the imdDom of the child isn't the parentNode; imdDom of the parentNode is NULL
      if(tempImdDom){
        while(tempImdDom->rank > node->rank){
          tempImdDom = tempImdDom->imdDom;
        }
      }
      
      //the child is not strictly dominated by the node
      if(tempImdDom != node || !tempImdDom){
        addListLast(domFrontiers, ((Node*)tempHeadCL->node)->children[i]);
        continue; //any grandchild need to be dominated by node, otherwise the child of grandchild cannot be DF of node
      }
      
      tempCLElement = checklist->head;
      
      //checking is the children already put in the checklist
      while(tempCLElement){
       
        if( ((Node*)tempHeadCL->node)->children[i] == tempCLElement->node )
          break;

        tempCLElement = tempCLElement->next;
      }
     
     if(!tempCLElement)
        addListLast(checklist, ((Node*)tempHeadCL->node)->children[i]);
    }
    
    tempHeadCL = tempHeadCL->next;

  }
  
  return domFrontiers;
}

LinkedList* getAllDomFrontiers(Node** root){
  
  LinkedList* domFrontiers = createLinkedList();
  LinkedList* checklist = createLinkedList();
  ListElement* tempCheckList = NULL;
  ListElement* tempDFList = NULL;
  ListElement* tempCLElement = NULL;
  ListElement* tempNodeDF = NULL;
  int i;
  
  addListLast(checklist, *root);
  tempCheckList = checklist->head;

  //Assemble the tree into a LinkedList 
  while(tempCheckList){
    
    for( i = 0; i < ((Node*)tempCheckList->node)->numOfChild; i++){
      tempCLElement = checklist->head;
      
      //checking is the children already put in the checklist
      while(tempCLElement){
        if( ((Node*)tempCheckList->node)->children[i] == tempCLElement->node )
          break;

        tempCLElement = tempCLElement->next;
      }
     
     if(!tempCLElement)
        addListLast(checklist, ((Node*)tempCheckList->node)->children[i]);
    }
    
    tempCheckList = tempCheckList->next;
  }
  
  
  tempCheckList = checklist->head;
  //get domFs of each node and add in the list, compare between DFlist and nodeDFlist, if already inside DFlist, just skip
  while(tempCheckList){
    ((Node*)tempCheckList->node)->domFrontiers = getNodeDomFrontiers(tempCheckList->node);
    tempNodeDF = ((Node*)tempCheckList->node)->domFrontiers->head;
    
    while(tempNodeDF){
      
      tempDFList = domFrontiers->head;
      while(tempDFList){
        
          if(tempDFList->node == tempNodeDF->node )
            break;
  
          tempDFList = tempDFList->next;
        }
      
      if(!tempDFList)
          addListLast(domFrontiers, tempNodeDF->node);
        
      tempNodeDF = tempNodeDF->next;
    }
    
    tempCheckList = tempCheckList->next;
  }
    
  return domFrontiers;
    
}

//ASSEMBLE ALL THE NODE THAT HAVE A RANK EQUAL OR LOWER THAN THE ROOT INTO A LINKEDLIST
LinkedList* assembleList(Node **rootNode){
    LinkedList* tempList      = createLinkedList();
    addListLast(tempList, *rootNode);
    int i;
    ListElement* tempElement  = tempList->head;
    Node *tempNode            = tempElement->node;

    while(tempElement != NULL){
      for(i = 0; i < tempNode->numOfChild; i++){
        if(tempList->tail->node != tempNode->children[i] \
        && tempNode->rank < tempNode->children[i]->rank)
          addListLast(tempList, tempNode->children[i]);
      }
      tempElement = tempElement->next;
      
      if(tempElement != NULL)
        tempNode = tempElement->node;
    }
    return tempList;
}
  
