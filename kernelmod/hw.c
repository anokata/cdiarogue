#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

void sayhi(void) {
    printk(KERN_INFO"Hi, from mod\n");
}

EXPORT_SYMBOL(sayhi);

static int __init hi_init(void) {
    sayhi();
    return 0;
}

static void __exit hi_exit(void) {
    printk(KERN_INFO"goodby\n");
}

module_init(hi_init);
module_exit(hi_exit);
MODULE_LICENSE("GPL");
