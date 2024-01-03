#ifndef _LIST_H_
#define _LIST_H_

struct list_node
{
	int value;
	list_node *p_next;
};

class list
{
public:
	list();
	void insert(int value);
	void print();
	void reverse();
	void reverse_resursive();
private:
	void list::reverse_resursive_imp(list_node *, list_node *, list_node *);
	list_node *p_head;
};

#endif