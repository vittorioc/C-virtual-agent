#ifndef _LIST_H
#define	_LIST_H

typedef void *element;
typedef struct list_t *list;
typedef struct list_iterator_t *list_iterator;

list allocList();
void freeList(list _F_list);
void addElementToList(list _list, element _element);
list_iterator allocListIterator(list _list);
void freeListIterator(list_iterator _F_list_iterator);
int hasNextList(list_iterator _list_iterator);
element nextElementFromList(list_iterator _list_iterator);
void removeLastReturnedElementFromList(list_iterator _list_iterator);
void removeElementFromList(list _list, element _element);

#endif	/* _LIST_H */

