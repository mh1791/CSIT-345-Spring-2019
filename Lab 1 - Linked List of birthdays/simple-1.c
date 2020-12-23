#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

/* Birthday struct. */
struct birthday
{
 int month;
 int day;
 int year;
 char *name;
 struct list_head list;
};

/* Declare and init the head of the linked list. */
static LIST_HEAD(birthday_list);

int simple_init(void)
{
 printk(KERN_INFO "Loading Module\n");
 /* Allocate 5 birthdays from kernel */
 int i=0;
 int day = 10, month = 8 ,year = 1995;
 char* name[] = {"John","Sarah","Kevin","Luis","Veronica"};
 struct birthday *temp = NULL;

 while(i<5)
	{
  /* Request malloc to the kernel and assign value to struct. */
	temp = kmalloc(sizeof(*temp),GFP_KERNEL);
	temp->day = --day;
	temp->month = --month;
	temp->year = year++;
	temp->name = name[i];
  	/* Init the list within the struct and add this struct to the tail of the list. */
	INIT_LIST_HEAD(&(temp->list));
	list_add_tail(&(temp->list) , &(birthday_list));

	i++;
	}
 temp = NULL;
 printk(KERN_INFO "Showing list:\n");
 /* Go through list and print. */
 list_for_each_entry(temp, &birthday_list, list)
	{

 	printk(KERN_INFO "Name: %s\n",temp->name);
 	printk(KERN_INFO "Birthday: %d/%d/%d\n",temp->month,temp->day,temp->year);
	}
 printk(KERN_INFO "List done\n");
 return 0;
}

/* This function is called when the module is removed. */
int simple_exit(void)
{
 printk(KERN_INFO "Removing Module\n");
 struct birthday *ptr, *next;

 list_for_each_entry_safe(ptr, next, &birthday_list, list)
	{

 	printk(KERN_INFO "Removing %s: month: %d, day: %d, year: %d \n", ptr->name, ptr->month, ptr->day, ptr->year);
	list_del(&ptr->list);
 	kfree(ptr);
	}
 printk(KERN_INFO "Memory freeing done\n");
 return 0;
}
/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("Michael");
