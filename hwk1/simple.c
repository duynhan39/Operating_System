// Nhan Cao
// CS 470: Homework 1

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday {
  int day;
  int month;
  int year;
  struct list_head list;
};

static LIST_HEAD(birthday_list);

int simple_init(void)
{
  // Make 5 people's birthday 
  struct birthday *person1, *person2, *person3, *person4, *person5;
  int count=0;
  
  person1 = kmalloc(sizeof(*person1), GFP_KERNEL);
  person1->day = 6;
  person1->month = 5;
  person1->year = 1988;
  list_add_tail(&person1->list, &birthday_list);

  person2 = kmalloc(sizeof(*person2), GFP_KERNEL);
  person2->day = 9;
  person2->month = 3;
  person2->year = 1998;
  list_add_tail(&person2->list, &birthday_list);

  person3 = kmalloc(sizeof(*person3), GFP_KERNEL);
  person3->day = 7;
  person3->month = 7;
  person3->year = 1960;
  list_add_tail(&person3->list, &birthday_list);

  person4 = kmalloc(sizeof(*person4), GFP_KERNEL);
  person4->day = 20;
  person4->month = 2;
  person4->year = 1910;
  list_add_tail(&person4->list, &birthday_list);	

  person5 = kmalloc(sizeof(*person5), GFP_KERNEL);
  person5->day = 30;
  person5->month = 1;
  person5->year = 1989;
  list_add_tail(&person5->list, &birthday_list);


  printk("===============================================\n");
  
  struct birthday *ptr;
	
  // Iterating through the birthday list and print them out
  list_for_each_entry(ptr, &birthday_list, list){
    printk("Birthday: %d/%d/%d\n", ptr->month, ptr->day, ptr->year);
    count++;
  }

  printk("Total: %d\n", count);

  return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {

  struct birthday *ptr, *next;
  int count = 0;
  // Iterating through the birthday list, print and delete them out
  list_for_each_entry_safe(ptr, next, &birthday_list, list){
    printk("Deleting birthday: %d/%d/%d\n", ptr->month, ptr->day, ptr->year);
    list_del(&ptr->list);
    kfree(ptr);
    count++;
  }
  printlk("Removed: %d", count);

}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
