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
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/bug.h>

MODULE_AUTHOR("Maksim Kochenkov <mkochenkov@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int repeat_count = 1;
module_param(repeat_count, uint, 0444);
MODULE_PARM_DESC(repeat_count, "Amount of 'Hello, World!' that will be printed");

struct event_list {
    struct list_head list;
    ktime_t event_time;
};

static LIST_HEAD(event_list_head);

static int __init hello_init(void)
{
    int i;
    struct event_list *event;

    if (repeat_count == 0 || (repeat_count >= 5 && repeat_count <= 10))
        pr_warn("Parameter is either 0 or in range 5-10\n");

    if (repeat_count > 10) {
        BUG_ON(1);
    }

    for (i = 0; i < repeat_count; i++) {
        if (i == 4) {
            event = NULL;
        } else {
            event = kmalloc(sizeof(*event), GFP_KERNEL);
        }

        BUG_ON(!event);

        event->event_time = ktime_get();
        list_add_tail(&event->list, &event_list_head);
	pr_info("Hello, World!\n");
    }
    return 0;
}

static void __exit hello_exit(void)
{
    struct event_list *event, *tmp;

    list_for_each_entry_safe(event, tmp, &event_list_head, list) {
        pr_info("Time: %lld\n", event->event_time);
        list_del(&event->list);
        kfree(event);
    }
}

module_init(hello_init);
module_exit(hello_exit);
