#include "unity.h"
#include "NodeChain.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "LinkedList.h"
#include "CException.h"
// TEST_ASSERT_NODE_DATA(char* expectExpress, int expectRank, Node* expectParent, Node* actualNode)
// TEST_ASSERT_NODE_DATA use to test internal data of the Node, not necessary to be the same node

void setUp(void){}

void tearDown(void){}

void test_createBlock(void){
	Block* testBlock = createBlock("Hello World", 3);

  TEST_ASSERT_NOT_NULL(testBlock);
  TEST_ASSERT_EQUAL(3, testBlock->data);
  TEST_ASSERT_EQUAL("Hello World", testBlock->string);
}

void test_createNode(void){
	Node* testNode = createNode(0);

  TEST_ASSERT_NOT_NULL(testNode);
  TEST_ASSERT_EQUAL(0, testNode->rank);
  TEST_ASSERT_EQUAL(0, testNode->numOfChild);
  TEST_ASSERT_EQUAL(0, testNode->numOfDom);
  TEST_ASSERT_EQUAL(0, testNode->numOfParent);
  TEST_ASSERT_NULL(testNode->parent);
  TEST_ASSERT_NULL(testNode->lastBrhDom);
  TEST_ASSERT_NULL(testNode->imdDom);
  TEST_ASSERT_NULL(testNode->children);
  TEST_ASSERT_NULL(testNode->doms);
}

/***************************************
 *  Given Node parentNode (a) and childNode (b)
 *  addChild should link them as shown below
 *
 *          [a] rank 0
 *           |
 *          [b] rank 1
 ***************************************/
void test_addChild_given_parentNode_and_childNode_should_link_it_as_tree_above(void){
  Node* parentNode  = createNode(0);
  Node* childNode   = createNode(1);

  addChild(&parentNode, &childNode);

  TEST_ASSERT_EQUAL(1, parentNode->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNode, parentNode->children[0]);
  TEST_ASSERT_EQUAL(1, childNode->numOfParent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNode->parent);
}

/***************************************
 *  Given Node parentNode (a) with child (b) add childNode (c)
 *  addChild should link them as shown below
 *
 *    [a]      [a] rank 0
 *     |  >>   / \
 *    [b]    [b] [c] rank 1
 ***************************************/
void test_addChild_given_parentNode_with_childB_and_childNodeC_should_link_it_as_tree_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);

  TEST_ASSERT_EQUAL(2, parentNode->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, parentNode->children[0]);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, parentNode->children[1]);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->parent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->parent);
}

/***************************************
 *  Forming the following tree with addChild function only
 *
 *           [parent]
 *            /   \
 *          [B]   [C]
 *            \   /
 *             [D]
 ***************************************/
void test_addChild_given_parentNode_add_3_childNode_should_link_it_as_tree_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);

  TEST_ASSERT_EQUAL(2, parentNode->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, parentNode->children[0]);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, parentNode->children[1]);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->parent);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->parent);
  TEST_ASSERT_EQUAL(1, childNodeB->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeD, childNodeB->children[0]);
  TEST_ASSERT_EQUAL(1, childNodeC->numOfChild);
  TEST_ASSERT_NODE_ADDRESS(childNodeD, childNodeC->children[0]);
  TEST_ASSERT_EQUAL(2, childNodeD->numOfParent);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, childNodeD->parent);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of true parent for the following tree
 *
 *      [parent]
 *         |
 *        [B]
 *
 **/
void test_setLastBrhDom_with_one_child_only(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);

  addChild(&parentNode, &childNodeB);

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *
 *      [parent]
 *        /  \
 *      [B]  [C]
 *
 **/
void test_setLastBrhDom_with_two_child_only(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *  TreeA
 *
 *     [parent]
 *       /  \
 *     [B]  [C]
 *       \  /
 *       [D]
 *
 **/
void test_setLastBrhDom_with_treeA_shown_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->lastBrhDom);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *  TreeB
 *
 *     [parent]
 *      /   \
 *    [B]   [C]
 *      \   / \
 *       [D]  [E]
 *
 **/
void test_setLastBrhDom_with_treeB_shown_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  Node* childNodeE  = createNode(2);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);
  addChild(&childNodeC, &childNodeE);

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeC, childNodeE->lastBrhDom);
}

/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *  TreeC
 *
 *     [parent]
 *      /   \
 *    [B]   [C]
 *    / \   /
 *  [E]  [D]
 *   |    |
 *  [F]  [G]
 *   \   /
 *    [H]
 **/
void test_setLastBrhDom_with_treeC_shown_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  Node* childNodeE  = createNode(2);
  Node* childNodeF  = createNode(3);
  Node* childNodeG  = createNode(3);
  Node* childNodeH  = createNode(4);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeE);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);
  addChild(&childNodeE, &childNodeF);
  addChild(&childNodeD, &childNodeG);
  addChild(&childNodeF, &childNodeH);
  addChild(&childNodeG, &childNodeH);

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeD->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeE->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeF->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeG->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeH->lastBrhDom);
}


/**
 *  setParent will assign lastBrhDom to all the node in the tree
 *  This test test assign of lastBrhDom for the following tree
 *  TreeC
 *
 *        [A]
 *       /   \
 *    [B]    [C]
 *    / \     |
 *  [D] [E]  [F]
 *  /  \ /    |
 *[G]--[H]---[I]
 * |    ^
 * |    ^
 *[J]--[K]
 *
 **/
void test_setLastBrhDom_with_treeD_shown_above(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  Node* childNodeE  = createNode(2);
  Node* childNodeF  = createNode(2);
  Node* childNodeG  = createNode(3);
  Node* childNodeH  = createNode(3);
  Node* childNodeI  = createNode(3);
  Node* childNodeJ  = createNode(4);
  Node* childNodeK  = createNode(5);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeE);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeF);
  addChild(&childNodeD, &childNodeG);
  addChild(&childNodeD, &childNodeH);
  addChild(&childNodeE, &childNodeH);
  addChild(&childNodeF, &childNodeI);
  addChild(&childNodeI, &childNodeH);
  addChild(&childNodeG, &childNodeH);
  addChild(&childNodeG, &childNodeJ);
  addChild(&childNodeJ, &childNodeK);
  addChild(&childNodeK, &childNodeH);

  setLastBrhDom(&parentNode);

  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeD->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, childNodeE->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeF->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeD, childNodeG->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeH->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(parentNode, childNodeI->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeG, childNodeJ->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(childNodeG, childNodeK->lastBrhDom);
}

/**       ControlFlowGraph1      s
 *
 *          | Entry              
 *         \/                     
 *        [ A ]<<<<              
 *       |   |    /\             
 *      \/  \/    /\
 *     [B]  [C]   /\
 *      |    |    /\
 *     \/   \/    /\
 *       [D] >>>>>>>
 *
 **/
void test_setLastBrhDom_give_CFG1_should_assign_lastBranchDominator_of_each_node_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  addChild(&nodeD, &nodeA);

  setLastBrhDom(&nodeA);
  
  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeB->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeC->lastBrhDom);
  TEST_ASSERT_NODE_ADDRESS(nodeA, nodeD->lastBrhDom);
}

//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************
//********************TEST for getImdDom ******************************
//*********************************************************************
/**
 *  getImdDom will find imdDom of that node and return
 *
 *       TREE A:  ImdDom
 *      [parent]  {parent}
 *         |
 *        [B]     {parent}
 *         |
 *        [C]     {B}
 *     
 **/
void test_getImdDom_given_nodeC_should_return_nodeB(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(2);

  addChild(&parentNode, &childNodeB);
  addChild(&childNodeB, &childNodeC);
  setLastBrhDom(&parentNode);
  
  Node* testNode = getImdDom(childNodeC)

  TEST_ASSERT_NODE_ADDRESS(childNodeB, testNode);
}


/**
 *  getImdDom will find imdDom of that node and return
 *
 *       TREE A:  ImdDom
 *      [parent]  {parent}
 *       /    \    
 *     [B]    [C] {parent}
 *       \   /
 *        [D]
 *     
 *     
 **/
void test_getImdDom_given_nodeD_should_return_parentNode(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(2);
  Node* childNodeD  = createNode(3);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeD);
  setLastBrhDom(&parentNode);
  
  Node* testNode = getImdDom(childNodeD);

  TEST_ASSERT_NODE_ADDRESS(parentNode, testNode);
}

/**
 *  getImdDom will find imdDom of that node and return
 *
 *        [A]
 *       /   \
 *    [B]    [C]
 *    / \     |
 *  [D] [E]  [F]
 *  /  \ /    |
 *[G]--[H]---[I]
 * |    ^
 * |    ^
 *[J]--[K]
 *
 **/
void test_getImdDom_given_nodeJ_should_return_nodeG(void){
  Node* parentNode  = createNode(0);
  Node* childNodeB  = createNode(1);
  Node* childNodeC  = createNode(1);
  Node* childNodeD  = createNode(2);
  Node* childNodeE  = createNode(2);
  Node* childNodeF  = createNode(2);
  Node* childNodeG  = createNode(3);
  Node* childNodeH  = createNode(3);
  Node* childNodeI  = createNode(3);
  Node* childNodeJ  = createNode(4);
  Node* childNodeK  = createNode(5);

  addChild(&parentNode, &childNodeB);
  addChild(&parentNode, &childNodeC);
  addChild(&childNodeB, &childNodeE);
  addChild(&childNodeB, &childNodeD);
  addChild(&childNodeC, &childNodeF);
  addChild(&childNodeD, &childNodeG);
  addChild(&childNodeD, &childNodeH);
  addChild(&childNodeE, &childNodeH);
  addChild(&childNodeF, &childNodeI);
  addChild(&childNodeI, &childNodeH);
  addChild(&childNodeG, &childNodeH);
  addChild(&childNodeG, &childNodeJ);
  addChild(&childNodeJ, &childNodeK);
  addChild(&childNodeK, &childNodeH);
  setLastBrhDom(&parentNode);
  
  Node* testNode = getImdDom(childNodeK);
  TEST_ASSERT_NODE_ADDRESS(childNodeJ, testNode);
  
  testNode = getImdDom(childNodeJ);
  TEST_ASSERT_NODE_ADDRESS(childNodeG, testNode);
  
  testNode = getImdDom(childNodeG);
  TEST_ASSERT_NODE_ADDRESS(childNodeD, testNode);
  
  testNode = getImdDom(childNodeH);
  TEST_ASSERT_NODE_ADDRESS(parentNode, testNode);
  
  testNode = getImdDom(childNodeI);
  TEST_ASSERT_NODE_ADDRESS(childNodeF, testNode);
  
  testNode = getImdDom(childNodeD);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, testNode);
  
  testNode = getImdDom(childNodeE);
  TEST_ASSERT_NODE_ADDRESS(childNodeB, testNode);
}

/**       ControlFlowGraph1           DomFrontiers
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
 **/
void test_getNodeDomFrontiers_given_CFG1_then_find_domFrontiers_of_each_node_should_return_domFrontiers_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(2);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeD);
  addChild(&nodeD, &nodeA);

  setLastBrhDom(&nodeA);
  nodeA->imdDom = NULL;
  nodeB->imdDom = getImdDom(nodeB);
  nodeC->imdDom = getImdDom(nodeC);
  nodeD->imdDom = getImdDom(nodeD);

  LinkedList* domFrontiersA = createLinkedList();
  LinkedList* domFrontiersB = createLinkedList();
  LinkedList* domFrontiersC = createLinkedList();
  LinkedList* domFrontiersD = createLinkedList();
  addListLast(domFrontiersA, nodeA);
  addListLast(domFrontiersB, nodeD);
  addListLast(domFrontiersC, nodeD);
  addListLast(domFrontiersD, nodeA);

  nodeA->domFrontiers = getNodeDomFrontiers(nodeA);
  nodeB->domFrontiers = getNodeDomFrontiers(nodeB);
  nodeC->domFrontiers = getNodeDomFrontiers(nodeC);
  nodeD->domFrontiers = getNodeDomFrontiers(nodeD);
  
  TEST_ASSERT_LINKED_LIST(domFrontiersA, nodeA->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersB, nodeB->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersC, nodeC->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersD, nodeD->domFrontiers);
}

/**    ControlFlowGraph2         DomFrontiers
 *
 *          | Entry
 *         \/
 *      [[[A]]]                DF(A) = { }
 *      |  |  \                DF(B) = { E }
 *     \/ \/  \/               DF(C) = { E } 
 *    [B] [C]  [D]             DF(D) = { F }
 *     |   |    |              DF(E) = { F }
 *    \/  \/   |               DF(F) = { }
 *     [E]    |
 *       \   |
 *       \/ \/
 *        [F]
 **/
void test_getNodeDomFrontiers_given_CFG2_then_find_domFrontiers_of_each_node_should_return_domFrontiers_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(1);
  Node* nodeD = createNode(1);
  Node* nodeE = createNode(2);
  Node* nodeF = createNode(2);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeC);
  addChild(&nodeA, &nodeD);
  addChild(&nodeB, &nodeE);
  addChild(&nodeC, &nodeE);
  addChild(&nodeD, &nodeF);
  addChild(&nodeE, &nodeF);

  setLastBrhDom(&nodeA);
  
  // nodeA->imdDom = getImdDom(nodeA);
  nodeB->imdDom = getImdDom(nodeB);
  nodeC->imdDom = getImdDom(nodeC);
  nodeD->imdDom = getImdDom(nodeD);
  nodeE->imdDom = getImdDom(nodeE);
  nodeF->imdDom = getImdDom(nodeF);

  LinkedList* domFrontiersA = createLinkedList();
  LinkedList* domFrontiersB = createLinkedList();
  LinkedList* domFrontiersC = createLinkedList();
  LinkedList* domFrontiersD = createLinkedList();
  LinkedList* domFrontiersE = createLinkedList();
  LinkedList* domFrontiersF = createLinkedList();
  addListLast(domFrontiersB, nodeE);
  addListLast(domFrontiersC, nodeE);
  addListLast(domFrontiersD, nodeF);
  addListLast(domFrontiersE, nodeF);
  
  nodeA->domFrontiers = getNodeDomFrontiers(nodeA);
  nodeB->domFrontiers = getNodeDomFrontiers(nodeB);
  nodeC->domFrontiers = getNodeDomFrontiers(nodeC);
  nodeD->domFrontiers = getNodeDomFrontiers(nodeD);
  nodeE->domFrontiers = getNodeDomFrontiers(nodeE);
  nodeF->domFrontiers = getNodeDomFrontiers(nodeF);
    
  TEST_ASSERT_LINKED_LIST(domFrontiersA, nodeA->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersB, nodeB->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersC, nodeC->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersD, nodeD->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersE, nodeE->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersF, nodeF->domFrontiers);
}
 
/**       ControlFlowGraph3           DomFrontiers
 *
 *          | Entry                 DF(A) = { }
 *         \/                       DF(B) = { F } 
 *        [ A ]                     DF(C) = { E }
 *          |  \                    DF(D) = { E }
 *         \/   \                   DF(E) = { F }
 *       [ B ]   \                  DF(F) = { }
 *       /   \    \
 *     \/    \/   |
 *    [C]   [D]   |
 *     \     /    |
 *     \/  \/    /
 *     [ E ]    /
 *       |    /
 *      \/  \/
 *     [ F ]
 *
 **/
void test_getNodeDomFrontiers_given_CFG3_then_find_domFrontiers_of_each_node_should_return_domFrontiers_respectively(void){
  Node* nodeA = createNode(0);
  Node* nodeB = createNode(1);
  Node* nodeC = createNode(2);
  Node* nodeD = createNode(2);
  Node* nodeE = createNode(3);
  Node* nodeF = createNode(1);

  addChild(&nodeA, &nodeB);
  addChild(&nodeA, &nodeF);
  addChild(&nodeB, &nodeC);
  addChild(&nodeB, &nodeD);
  addChild(&nodeC, &nodeE);
  addChild(&nodeD, &nodeE);
  addChild(&nodeE, &nodeF);

  setLastBrhDom(&nodeA);
  
  // nodeA->imdDom = getImdDom(nodeA);
  nodeB->imdDom = getImdDom(nodeB);
  nodeC->imdDom = getImdDom(nodeC);
  nodeD->imdDom = getImdDom(nodeD);
  nodeE->imdDom = getImdDom(nodeE);
  nodeF->imdDom = getImdDom(nodeF);

  LinkedList* domFrontiersA = createLinkedList();
  LinkedList* domFrontiersB = createLinkedList();
  LinkedList* domFrontiersC = createLinkedList();
  LinkedList* domFrontiersD = createLinkedList();
  LinkedList* domFrontiersE = createLinkedList();
  LinkedList* domFrontiersF = createLinkedList();
  addListLast(domFrontiersB, nodeF);
  addListLast(domFrontiersC, nodeE);
  addListLast(domFrontiersD, nodeE);
  addListLast(domFrontiersE, nodeF);
  
  nodeA->domFrontiers = getNodeDomFrontiers(nodeA);
  nodeB->domFrontiers = getNodeDomFrontiers(nodeB);
  nodeC->domFrontiers = getNodeDomFrontiers(nodeC);
  nodeD->domFrontiers = getNodeDomFrontiers(nodeD);
  nodeE->domFrontiers = getNodeDomFrontiers(nodeE);
  nodeF->domFrontiers = getNodeDomFrontiers(nodeF);
  
  TEST_ASSERT_LINKED_LIST(domFrontiersA, nodeA->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersB, nodeB->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersC, nodeC->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersD, nodeD->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersE, nodeE->domFrontiers);
  TEST_ASSERT_LINKED_LIST(domFrontiersF, nodeF->domFrontiers);
}