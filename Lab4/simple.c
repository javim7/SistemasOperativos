// Javier Mombiela
// Carnet: 20067
// Seccion: 21

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

int simple_init(void)
{
    printk(KERN_INFO "Loading Module\nHello World!");
    return 0;
}

void simple_exit(void)
{
    printk(KERN_INFO "Removing Module\nGoodbye World!");
}

module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("No se me ocurrio nada bueno.");
MODULE_AUTHOR("Javier");