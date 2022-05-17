#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>

/*
The purpose of this program to define matrices to maximize memory. While these "sparse matrices"	
* have already been created, our job was to implement them through C. The add and multiply functions were implemented based on the 	
* pseudocode provided on the wiki. The save, gv, and destroy functions were short and relatively simple, and done to the parameters  
* as described in the program details.
*/



sp_tuples * load_tuples(char* input_file)
{
    int rows;
    int cols;

    sp_tuples* ptr = (sp_tuples*)malloc(sizeof(sp_tuples));

    FILE* fptr = fopen(input_file, "r");
    if (fptr == NULL) {
        printf("Error: %s not found", input_file);
        return NULL;
    }

    fscanf(fptr, "%d %d\n", &rows, &cols);

    ptr->m = rows;
    ptr->n = cols;
    ptr->nz = 0;
    ptr->tuples_head = NULL;

    int x;
    int y;
    double value;
    while (fscanf(fptr, "%d %d %lf\n", &y, &x, &value) == 3) {
        set_tuples(ptr, y, x, value);
    }

    //if we're not at the end of the file, something went wrong
    if (!feof(fptr)) {
        return NULL;
    }

    fclose(fptr);
    return ptr;
}



double gv_tuples(sp_tuples * mat_t,int row,int col)

{
    int item_index = row* (mat_t->n) + col; //set item index using given row and col
	sp_tuples_node* head = (mat_t->tuples_head); //set head pointer to point to head of list
	
	while( head != NULL){ //check that the head is not NULL
		if( (head->row)*(mat_t->n) + (head->col) == item_index) //compare indexes and return value
			return head->value;
		
		else if((head->row)*(mat_t->n) + (head->col) > item_index) //if the index specified has been passed, the object is not in list
			return 0;

		head = head->next; 	//update head to next pointer
	}
	return 0; //if not found and list exhausted, return 0;
}

//if node1 < node2: return 0
//if node1 > node2: return 1
int sort_tuples(sp_tuples_node* node1, sp_tuples_node* node2) {
    if ((node1->row) != (node2->row)) {
        return ((node1->row) > (node2->row));
    }
    else {
        return ((node1->col) > (node2->col));
    }
    //something went very wrong
    return -1;
}


void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{

    if (mat_t->tuples_head == NULL) {
        if (value != 0) {
            //allocate a new node
            sp_tuples_node* head = (sp_tuples_node*)malloc(sizeof(sp_tuples_node));
            //fill the values
            head->row = row;
            head->col = col;
            head->value = value;
            head->next = NULL;
            //point to the new head and incriment non-zero count
            mat_t->tuples_head = head;
            mat_t->nz += 1;
        }
        return;
    }

    //if the list has some nodes, there are four cases:
    //1) node exists and value is zero --> delete node
    //2) node exists and value is non-zero --> update node
    //3) node does not exist and value is zero --> do nothing
    //4) node does not exist and value is non-zero --> add node

    if (gv_tuples(mat_t, row, col) != 0) {
        //Case 1: delete node
        if (value == 0) {
            sp_tuples_node* previous = NULL;
            sp_tuples_node* node = mat_t->tuples_head;

            while(node != NULL) {
                if (node->row == row && node->col == col) {
                    //Case 1: deleting the head node,
                    //set the new head to be the next node
                    if (previous == NULL) {
                        mat_t->tuples_head = node->next;
                    }
                    //Case 2: deleting middle node,
                    //link the previous node to the next node
                    else {
                        previous->next = node->next;
                    }

                    free(node);
                    mat_t->nz -= 1;
                    return;
                }
                previous = node;
                node = node->next;
            }
        }
        //Case 2: update node
        else {
            sp_tuples_node* node = mat_t->tuples_head;

            while(node != NULL) {
                if (node->row == row && node->col == col) {
                    node->value = value;
                    return;
                }
                node = node->next;
            }
        }
    }
    else {
        //Case 3: do nothing
        if (value == 0) {
            return;
        }
        //Case 4: add node
        else {
            sp_tuples_node* previous = NULL;
            sp_tuples_node* node = mat_t->tuples_head;
            sp_tuples_node* new = (sp_tuples_node*)malloc(sizeof(sp_tuples_node));

            new->row = row;
            new->col = col;
            new->value = value;
            new->next = NULL;
            mat_t->nz += 1;

            while(node != NULL) {
                //if node comes after new...
                if (sort_tuples(node, new)) {
                    //if we are replacing the head...
                    if (previous == NULL) {
                        new->next = node;
                        mat_t->tuples_head = new;
                    }
                    //otherwise insert between nodes
                    else {
                        previous->next = new;
                        new->next = node;
                    }
                    return;
                }
                previous = node;
                node = node->next;
            }
            //append new node to end of list
            previous->next = new;
            return;
        }
    }
    return;
}



void save_tuples(char * file_name, sp_tuples * mat_t)
{
    //check if matrix input is null (needed for unimplemented lil methods)
    if(mat_t != NULL) {
        FILE * file = fopen(file_name, "w");
        fprintf(file, "%d %d\n", mat_t->m, mat_t->n);

        sp_tuples_node * current = mat_t->tuples_head;

        //loop through the entire linked list and print out the values properly
        while(current != NULL) {
            //print the values from the currentent node
            fprintf(file, "%d %d %lf\n", current->row, current->col, current->value);

            //iterate
            current = current->next;
        }
        //close the file for writing
        fclose(file);
    }
	return;
}

sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
	//allocate and initialize new matrix C
    sp_tuples* matC = (sp_tuples*)malloc(sizeof(sp_tuples));
    matC->m = matA->m;
    matC->n = matA->n;
    matC->nz = 0;
    matC->tuples_head = NULL;

    //nodes for traversing A, and B
    sp_tuples_node* nodeA = matA->tuples_head;
    sp_tuples_node* nodeB = matB->tuples_head;

    while (nodeA != NULL) {
        //set the values of matrix C to those of matrix A
        //note that set_tuples handles the case of a null head
        set_tuples(matC, nodeA->row, nodeA->col, nodeA->value);
        nodeA = nodeA->next;
    }
    while (nodeB != NULL) {
        double valueC = gv_tuples(matC, nodeB->row, nodeB->col);
        set_tuples(matC, nodeB->row, nodeB->col, nodeB->value + valueC);
        nodeB = nodeB->next;
    }
	return matC;
}



sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){ 
    if( matA->n != matB->m) //if multiplication is not possible, return NULL
		return NULL;

	sp_tuples* matC = (sp_tuples*)malloc(sizeof(sp_tuples)); //declare and initialize matC
	matC->m = matA->m; //set rows by mat A
	matC->n = matB->n; //cols by mat B
	matC->nz = 0; 					
	matC->tuples_head = NULL;

	int rowA;
	int colA;
	int rowB;
	int colB;
	double value; 

	sp_tuples_node* A_ptr = matA->tuples_head; //initialize pointers to loop over matrices
	sp_tuples_node* B_ptr = matB->tuples_head;

	while( A_ptr != NULL) {
		rowA = A_ptr->row; 	//set current row and col of element from A
		colA = A_ptr->col; 		
		B_ptr = matB->tuples_head; //reinitialize B_ptr to start of list for next loop over matrix B

		while( B_ptr != NULL){
			rowB = B_ptr->row;
			colB = B_ptr->col;
			if(rowB == colA){
				value = gv_tuples(matC,rowA,colB) + A_ptr->value * B_ptr->value;
				set_tuples(matC,rowA,colB,value);		
			}
			B_ptr = B_ptr->next;
		}
		A_ptr = A_ptr->next;
	}
	return matC;
}
	
void destroy_tuples(sp_tuples * mat_t){
	if(mat_t != NULL) {
        sp_tuples_node * current = mat_t->tuples_head;
        sp_tuples_node * murica = NULL;

        while(current != NULL) { //loops through all nodes and deallocated memory one by one
            murica = current; 
            current = current->next;
            free(murica); //just for the memes so we could free murica
        }
        free(mat_t);
    }
    return;
}  






