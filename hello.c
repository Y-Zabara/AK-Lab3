
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list_node of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list_node of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("y_zabara_io14");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint howmany = 1;

module_param(howmany, uint, S_IRUGO);
MODULE_PARM_DESC(howmany, "Number of printed lines 'Hello, world!'");

struct hello_list {
	struct list_head list_node;
	ktime_t time;
};

static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{
	uint i;
	struct hello_list *md;

	if (howmany == 0 || (howmany > 5 && howmany <= 10))
		printk(KERN_EMERG "Warning: howmany is 0 or between 5 and 10.\n");

	if (howmany > 10) {
		printk(KERN_EMERG "Error: howmany is greater than 10. Module cannot be loaded.\n");
		return -EINVAL;
	}

	for (i = 0; i < howmany; ++i) {
		md = kmalloc(sizeof(struct hello_list), GFP_KERNEL);

		md->time = ktime_get();
		list_add_tail(&md->list_node, &my_list_head);
		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_list *md, *tmp;

	list_for_each_entry_safe(md, tmp, &my_list_head, list_node)	{
		printk(KERN_EMERG "md time: %lld ns\n", ktime_to_ns(md->time));
		list_del(&md->list_node);
		kfree(md);
	}

	BUG_ON(!list_empty(&my_list_head));
	printk(KERN_EMERG "Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
