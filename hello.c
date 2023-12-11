
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
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

// Оголошення структури для елемента списку
struct hello_event {
	struct list_head list;
	ktime_t timestamp;
};

// Голова списку
static LIST_HEAD(hello_list);

static uint howmany = 1;

module_param(howmany, uint, S_IRUGO);

// Опис параметра для modinfo
MODULE_PARM_DESC(howmany, "Number of printed lines 'Hello, world!'");

static int __init hello_init(void)
{
	uint i;
	struct hello_event *event;

	if (howmany == 0 || (howmany > 5 && howmany <= 10))
		printk(KERN_EMERG "Warning: howmany is 0 or between 5 and 10.\n");

	if (howmany > 10) {
		printk(KERN_EMERG "Error: howmany is greater than 10. Module cannot be loaded.\n");
		return -EINVAL;
	}

	for (i = 0; i < howmany; ++i) {
		event = kmalloc(sizeof(struct hello_event), GFP_KERNEL);

//		if (!event) {
//			pr_err("Error allocating memory for hello_event.\n");
//			return -ENOMEM;
//		}
		event->timestamp = ktime_get();
		list_add_tail(&event->list, &hello_list);
		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_event *event, *tmp;

	list_for_each_entry_safe(event, tmp, &hello_list, list)	{
		printk(KERN_EMERG "Event time: %lld ns\n", ktime_to_ns(event->timestamp));
		list_del(&event->list);
		kfree(event);
	}

	printk(KERN_EMERG "Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
