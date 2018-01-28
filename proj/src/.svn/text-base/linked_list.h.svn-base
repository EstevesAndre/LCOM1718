#ifndef LINKED_LIST_H
#define LINKED_LIST_H


/**
 * @brief struct list_element
 *
 * Element from list
 */
struct list_element
{
	void *element; 				//element
	struct list_element *next; 		//pointer to next element
	struct list_element *before;    //pointer to element before this one
};


/**
 * @brief struct list_t
 *
 * Double linked list with pointer to front and back
 */
typedef struct
{
	struct list_element *front;		//pointer to first element
	struct list_element *back;		//pointer to last element

} list_t;


/**
 * @brief Creates new list
 *
 * @return Pointer to memory upon succes, NULL pointer otherwise
 */
list_t *new_list();


/**
 * @brief Deletes list ll
 *
 * @param ll list to be deleted
 */
void delete_list(list_t *ll);


/**
 * @brief Inserts element in the back of list ll
 *
 * @param ll list where element will be inserted
 *
 * @param element element to be inserted
 *
 */
void push_back_list(list_t *ll, void *element);


/**
 * @brief Removes front element from list ll
 *
 * @param ll list which front element will be removed
 */
void pop_front_list(list_t *ll);

#endif
