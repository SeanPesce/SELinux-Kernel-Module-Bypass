// Author: Sean Pesce
//
// Kernel module that sets SELinux to "permissive" on load.

#ifndef MODULE
  #define MODULE
#endif // MODULE

#ifndef __KERNEL__
  #define __KERNEL__
#endif // __KERNEL__

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cred.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define LOG_TAG "[SeanP] "
#define SELINUX_ENFORCING_FPATH "/sys/fs/selinux/enforce"


static int sp_kmod_init(void)
{
    struct file *f;
    mm_segment_t oldfs;
    int err;

    printk(KERN_INFO LOG_TAG "SELinux disabler kernel module loaded\n");

    // Shed SELinux context
    commit_creds(prepare_kernel_cred(NULL));

    // Open the SELinux "enforce" file
    f = filp_open(SELINUX_ENFORCING_FPATH, O_WRONLY, 0);
    if (IS_ERR(f))
    { 
        printk(KERN_ALERT LOG_TAG "Failed to open SELinux enforce file (error code: %ld)\n", PTR_ERR(f));
        return PTR_ERR(f);
    }

    // Change the current segment descriptor to kernel space
    oldfs = get_fs();
    set_fs(KERNEL_DS);

    // Write "0" to the file to disable enforcing mode
    err = kernel_write(f, "0", 1, &f->f_pos);
    if (err < 0)
    {
        printk(KERN_ALERT LOG_TAG "Failed to write to SELinux enforce file\n");
    }
    else
    {
        printk(KERN_INFO LOG_TAG "[SUCCESS] Wrote 0 to SELinux enforce file\n");
    }

    // Restore the old segment descriptor
    set_fs(oldfs);
    // Close the file
    filp_close(f, NULL);

    return 0;
}


static void sp_kmod_exit(void)
{
    printk(KERN_INFO LOG_TAG "SELinux disabler kernel module unloaded\n");
}


module_init(sp_kmod_init);
module_exit(sp_kmod_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sean Pesce");
MODULE_ALIAS("SeanP_SELinux_Disable");
MODULE_DESCRIPTION("SELinux disabler by SeanP");
