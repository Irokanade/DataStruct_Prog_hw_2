//
//  main.cpp
//  DataStruct_Prog_hw_2
//
//  Created by Michael Leong on 18/10/2022.
//

#include <iostream>

using namespace std;

typedef struct _polyNode{
    double coeff;
    int expon;
    _polyNode *link;
} polyNode;

polyNode *listArr[1024];
int endOfList = 0; //index of the end of listArr

polyNode *createEmptyNode() {
    polyNode *node = new polyNode;
    
    node->coeff = 0;
    node->expon = 0;
    node->link = NULL;
    
    return node;
}

void deleteNode(polyNode **node, polyNode **startNode) {
    polyNode *currNode = *startNode;
    polyNode *prevNode = NULL;
    while(currNode) {
        if(currNode == *node) {
            if(prevNode == NULL) {
                //if delete node is head
                polyNode *temp = (*startNode)->link;
                delete *startNode;
                *startNode = temp;
                return;
                
            } else {
                prevNode->link = currNode->link;
                delete *node;
                return;
            }
        }
        prevNode = currNode;
        currNode = currNode->link;
    }
}

void deleteList(polyNode **startNode) {
    while(*startNode) {
        polyNode *temp = (*startNode)->link;
        delete *startNode;
        *startNode = temp;
    }
    
    *startNode = NULL;
}

void printLinkedList(polyNode *startNode) {
    //first node don't need to print '+'
    if(startNode) {
        cout << startNode->coeff << "x^" << startNode->expon;
        startNode = startNode->link;
    }
    
    while(startNode) {
        if(startNode->coeff >= 0) {
            cout << '+';
        }
        cout << startNode->coeff << "x^" << startNode->expon;
        startNode = startNode->link;
    }
    //cout << '\n';
}

//search for a specific power and print its coefficient
void printCoeff(polyNode *startNode, int searchPow) {
    while(startNode) {
        if(startNode->expon == searchPow) {
            cout << "exponent: " << startNode->expon << " coefficient: " << startNode->coeff << '\n';
            return;
        }
        startNode = startNode->link;
    }
    cout << "The term with exponent " << searchPow << "has a coefficient of 0\n";
}

//function to insert node infront of given node and update given node to new node
void insertLeftLink(polyNode **node, double newCoeff, int newExpon) {
    polyNode *temp = new polyNode;
    temp->coeff = newCoeff;
    temp->expon = newExpon;
    temp->link = *node;
    
    *node = temp;
}

void insertRightLink(polyNode **node, double newCoeff, int newExpon) {
    polyNode *temp = new polyNode;
    temp->coeff = newCoeff;
    temp->expon = newExpon;
    temp->link = (*node)->link;
    
    (*node)->link = temp;
}

//function to insert to back of list
void insertRear(polyNode **rearNode, double newCoeff, int newExpon) {
    polyNode *temp = new polyNode;
    temp->coeff = newCoeff;
    temp->expon = newExpon;
    temp->link = NULL;
    
    (*rearNode)->link = temp;
    *rearNode = temp;
}

//add newCoeff to given expon
void addCoeff(polyNode *node, double addCoeff, int expon) {
    //cout << "add coeff\n";
    while(node) {
        if(node->expon == expon) {
            //cout << "found\n";
            node->coeff = addCoeff + node->coeff;
            //cout << "add: " << node->coeff << '\n';
            return;
        }
        node = node->link;
    }
}

//insert node in correct order
//high power to low power
void sortedInsertNode(polyNode **startNode, double newCoeff, int newExpon) {
    polyNode *lastNode = NULL;
    polyNode *currNode = *startNode;
    
    //if pointer is empty create new node
    if(*startNode == NULL) {
        *startNode = createEmptyNode();
        (*startNode)->coeff = newCoeff;
        (*startNode)->expon = newExpon;
        return;
    }
    
    //if coeff is zero don't need to insert
    if(newCoeff == 0) {
        return;
    }
    
    //if new node has same expon then add it in
    if(newExpon == (*startNode)->expon) {
        addCoeff(*startNode, newCoeff, newExpon);
        return;
    }
    
    //if head node need to update head node
    if(newExpon > (*startNode)->expon) {
        insertLeftLink(startNode, newCoeff, newExpon);
        return;
    }
    
    //if only one node but need to insert to the right
    if((*startNode)->link == NULL && (newExpon < (*startNode)->expon)) {
        //else insert right
        insertRightLink(startNode, newCoeff, newExpon);
        return;
    }
    
    while(currNode && (newExpon < currNode->expon)) {
        lastNode = currNode;
        currNode = currNode->link;
        
        //if new node has same expon then add it in
        if(currNode && newExpon == currNode->expon) {
            addCoeff(currNode, newCoeff, newExpon);
            return;
        }
    }
    
    //if end of list
    if(currNode == NULL) {
        //pull back currNode
        currNode = lastNode;
        if(newExpon > currNode->expon) {
            //cout << "left\n";
            insertLeftLink(&currNode, newCoeff, newExpon);
        } else {
            //else insert right
            insertRightLink(&currNode, newCoeff, newExpon);
        }
    } else {
        //else insert right link
        insertRightLink(&currNode, newCoeff, newExpon);
    }
}

//modify specific term's coefficient
void modCoeff(polyNode **startNode) {
    polyNode *currNode = *startNode;
    
    int choice = 1;
    cout << "Enter 0 to delete value or enter 1 to insert value: ";
    cin >> choice;
    if(choice) {
        //insert new node
        int expon = 0;
        int coeff = 0;
        cout << "Enter new exponent: ";
        cin >> expon;
        cout << "Enter new coefficient: ";
        cin >> coeff;
        sortedInsertNode(startNode, coeff, expon);
        return;
    } else {
        int searchPow;
        printLinkedList(*startNode);
        cout << "\nEnter exponent of term to delete: ";
        cin >> searchPow;
        
        while(currNode) {
            if(currNode->expon == searchPow) {
                deleteNode(&currNode, startNode);
                return;
            }
            currNode = currNode->link;
        }
    }
    
    
    
}

int compareExpon(int a, int b) {
    if(a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    }
    //else if a == b
    return 0;
}

//polynomial addition
//polynomial must be stored in form of high power to low power
polyNode *polyAdd(polyNode *a, polyNode *b) {
    polyNode *c = NULL;
    int sum;
    polyNode *rear = new polyNode;
    c = rear;
    
    while(a && b) {
        switch (compareExpon(a->expon, b->expon)) {
            case -1:
                insertRear(&rear, b->coeff, b->expon);
                b = b->link;
                break;
            case 0:
                sum = a->coeff+b->coeff;
                if(sum) {
                    insertRear(&rear, sum, a->expon);
                }
                a = a->link;
                b = b->link;
                break;
            case 1:
                insertRear(&rear, a->coeff, a->expon);
                a = a->link;
                break;
            default:
                break;
        }
    }
    
    //add remaining nodes
    //insert a
    for(; a; a = a->link) {
        insertRear(&rear, a->coeff, a->expon);
    }
    //insert b
    for(; b; b = b->link) {
        insertRear(&rear, b->coeff, b->expon);
    }
    
    polyNode *temp = c;
    c = c->link;
    delete temp;
    
    return c;
}

//polynomial subtraction poly a - poly b
//polynomial must be stored in form of high power to low power
polyNode *polySub(polyNode *a, polyNode *b) {
    polyNode *c = NULL;
    int sum;
    polyNode *rear = new polyNode;
    c = rear;
    
    while(a && b) {
        switch (compareExpon(a->expon, b->expon)) {
            case -1:
                //b must multiply -1
                insertRear(&rear, b->coeff*-1, b->expon);
                b = b->link;
                break;
            case 0:
                //a coeff -b coeff
                sum = a->coeff-b->coeff;
                if(sum) {
                    insertRear(&rear, sum, a->expon);
                }
                a = a->link;
                b = b->link;
                break;
            case 1:
                //a coeff remains the same
                insertRear(&rear, a->coeff, a->expon);
                a = a->link;
                break;
            default:
                break;
        }
    }
    
    //add remaining nodes
    //insert a
    for(; a; a = a->link) {
        //a coeff remains the same
        insertRear(&rear, a->coeff, a->expon);
    }
    //insert b
    for(; b; b = b->link) {
        //b coeff must multiply -1
        insertRear(&rear, b->coeff*-1, b->expon);
    }
    
    polyNode *temp = c;
    c = c->link;
    delete temp;
    
    return c;
}

polyNode *polyMult(polyNode *a, polyNode *b) {
    polyNode *c = NULL;
    
    polyNode *ptr1 = a;
    polyNode *ptr2 = b;
    
    while(ptr1) {
        while(ptr2) {
            int totalCoeff = ptr1->coeff*ptr2->coeff;
            int totalExpon = ptr1->expon+ptr2->expon;
            
            sortedInsertNode(&c, totalCoeff, totalExpon);
            
            ptr2 = ptr2->link;
        }
        ptr2 = b;
        ptr1 = ptr1->link;
    }
    
    return c;
}

//function to copy one list to another
//copy a to b
void copyList(polyNode *a, polyNode **b) {
    deleteList(b);
    
    while(a) {
        sortedInsertNode(b, a->coeff, a->expon);
        a = a->link;
    }
}

//update quotient by inc expon and multiplying coeff
void updateQuo(polyNode *node, double multiple, int diff) {
    while(node) {
        node->expon = node->expon+diff;
        node->coeff = node->coeff*multiple;
        node = node->link;
    }
}

//a divide b
void polyDiv(polyNode *a, polyNode *b, polyNode **q, polyNode **r) {
    copyList(a, q);
    copyList(b, r);
    polyNode *quo = NULL;
    
    //perfrom subtraction until expon of q > highest power of divisor
    while(*q && (*q)->expon >= b->expon) {
        int diff = (*q)->expon - b->expon;
        
        //multiple
        double multiple = (*q)->coeff / b->coeff;
        
        //store quotient
        sortedInsertNode(&quo, multiple, diff);
        //cout << "insert quotient\n";
        //printLinkedList(quo);
        
        polyNode *q2 = NULL;
        copyList(*r, &q2);
        
        
        updateQuo(q2, multiple, diff);
        
        polyNode *store = NULL;
        store = polySub(*q, q2);
        copyList(store, q);
        deleteList(&store);
        delete store;
        //cout << "curr quotient\n";
        //printLinkedList(quo);
    }
    
    *r = *q;
    *q = quo;
    
    /*cout << "quotient\n";
    printLinkedList(*q);
    cout << "remainder\n";
    printLinkedList(*r);*/
}

polyNode *setList() {
    int noOfTerms;
    cout << "Enter number of terms to insert: ";
    cin >> noOfTerms;
    polyNode *node = createEmptyNode();
    //set first term
    //cout << "Input polynomial must start from high exponent to low exponent\n";
    cout << "If input exponent has an existing exponent the new coefficient will be added to the existing one\n";
    cout << "Enter exponent: ";
    cin >> node->expon;
    cout << "Enter coefficient: ";
    cin >> node->coeff;
    
    
    for(int i = 1; i < noOfTerms; i++) {
        int expon, coeff;
        //cout << "Input polynomial must start from high exponent to low exponent\n";
        cout << "If input exponent has an existing exponent the new coefficient will be added to the existing one\n";
        cout << "Enter exponent: ";
        cin >> expon;
        cout << "Enter coefficient: ";
        cin >> coeff;
        
        
        //insertRear(&node, coeff, expon);
        sortedInsertNode(&node, coeff, expon);
        //printLinkedList(node);
    }
    
    return node;
}

void setListUI() {
    listArr[endOfList++] = setList();
}

void printAllListUI() {
    cout << "------current lists------\n";
    for(int i = 0; i < endOfList; i++) {
        cout << '[' << i << "] ";
        printLinkedList(listArr[i]);
        cout << '\n';
    }
    cout << "-------------------------\n";
}

void printChoiceUI() {
    cout << "1: Input a polynomial\n";
    cout << "2: Print a polynomial\n";
    cout << "3: Print coefficient in a polynomial\n";
    cout << "4: Delete given term or add new term given coeff in polynomial\n";
    cout << "5: Choose two polynomials to perform addition\n";
    cout << "6: Choose two polynomials to perform subtraction\n";
    cout << "7: Choose two polynomials to perform multiplication\n";
    cout << "8: Choose two polynomials to perform division\n";
    cout << "-1: To quit\n";
}

void printListUI() {
    int index = -1;
    printAllListUI();
    cout << "Enter index of list to print: ";
    cin >> index;
    printLinkedList(listArr[index]);
    cout << '\n';
}

void printCoeffUI() {
    int index = -1;
    int coeff = -1;
    printAllListUI();
    cout << "Enter index of list to print: ";
    cin >> index;
    cout << '[' << index << "] ";
    printLinkedList(listArr[index]);
    cout << '\n';
    cout << "Enter coeff to print: ";
    cin >> coeff;
    printCoeff(listArr[index], coeff);
}

void modCoeffUI() {
    int index = -1;
    printAllListUI();
    cout << "Enter index of list to modify: ";
    cin >> index;
    modCoeff(&listArr[index]);
    cout << '[' << index << "] ";
    printLinkedList(listArr[index]);
    cout << '\n';
}

void addPolyUI() {
    cout << "The function will print the addition of P1 + P2\n";
    int index1 = -1;
    int index2 = -1;
    printAllListUI();
    cout << "Enter index of list for P1: ";
    cin >> index1;
    cout << "Enter index of list for P2: ";
    cin >> index2;
    
    polyNode *result = NULL;
    result = polyAdd(listArr[index1], listArr[index2]);
    printLinkedList(listArr[index1]);
    cout << " + ";
    printLinkedList(listArr[index1]);
    cout << " = ";
    printLinkedList(result);
    cout << '\n';
    deleteList(&result);
}

void subPolyUI() {
    cout << "The function will print the subtraction of P1 - P2\n";
    int index1 = -1;
    int index2 = -1;
    printAllListUI();
    cout << "Enter index of list for P1: ";
    cin >> index1;
    cout << "Enter index of list for P2: ";
    cin >> index2;
    
    polyNode *result = NULL;
    result = polySub(listArr[index1], listArr[index2]);
    printLinkedList(listArr[index1]);
    cout << " - ";
    printLinkedList(listArr[index1]);
    cout << " = ";
    printLinkedList(result);
    cout << '\n';
    deleteList(&result);
}

void mulPolyUI() {
    cout << "The function will print the multiplication of P1 * P2\n";
    int index1 = -1;
    int index2 = -1;
    printAllListUI();
    cout << "Enter index of list for P1: ";
    cin >> index1;
    cout << "Enter index of list for P2: ";
    cin >> index2;
    
    polyNode *result = NULL;
    result = polyMult(listArr[index1], listArr[index2]);
    printLinkedList(listArr[index1]);
    cout << " * ";
    printLinkedList(listArr[index1]);
    cout << " = ";
    printLinkedList(result);
    cout << '\n';
    deleteList(&result);
}

void divPolyUI() {
    cout << "The function will print the division of P1 / P2\n";
    int index1 = -1;
    int index2 = -1;
    printAllListUI();
    cout << "Enter index of list for P1: ";
    cin >> index1;
    cout << "Enter index of list for P2: ";
    cin >> index2;
    
    polyNode *quotient = NULL;
    polyNode *remainder = NULL;
    polyDiv(listArr[index1], listArr[index2], &quotient, &remainder);
    printLinkedList(listArr[index1]);
    cout << " / ";
    printLinkedList(listArr[index1]);
    cout << "\nquotient: ";
    printLinkedList(quotient);
    cout << "\nremainder: ";
    printLinkedList(remainder);
    cout << '\n';
    deleteList(&quotient);
    deleteList(&remainder);
}

void mainPageUI() {
    int choice = 0;
    
    while(choice != -1) {
        printAllListUI();
        printChoiceUI();
        cout << "Input choice: ";
        cin >> choice;
        if(choice == -1) {
            cout << "exit\n";
            return;
        } else if(choice == 1) {
            setListUI();
        } else if(choice == 2) {
            printListUI();
        } else if(choice == 3) {
            printCoeffUI();
        } else if(choice == 4) {
            modCoeffUI();
        } else if(choice == 5) {
            addPolyUI();
        } else if(choice == 6) {
            subPolyUI();
        } else if(choice == 7) {
            mulPolyUI();
        } else if(choice == 8) {
            divPolyUI();
        }
    }
}

int main(int argc, const char * argv[]) {
    mainPageUI();
    
    return 0;
}
