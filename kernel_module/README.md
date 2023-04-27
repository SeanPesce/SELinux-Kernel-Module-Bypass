# Kernel Module for SELinux Bypass  

**Author: Sean Pesce**  

When loaded, this kernel module will attempt to set SELinux to "permissive" mode.  

## Compiling  

**Requirements**

 * Kernel sources/headers for your target platform.
 * Compilation artifacts for the kernel
 * The kernel configuration for your target platform. If you have access to development resources, it should be a file called `.config`. Otherwise, [see here](https://blukat.me/2017/12/cross-compile-arm-kernel-module/#4-take-out-kernel-build-config) for some techniques to extract the kernel config from a running system.  
 * If you're cross-compiling for a different architecture, you'll need the correct cross-compilation toolchain installed.

To compile for the same architecture as your build environment, you only need to set the `KERNEL` variable and use the following command:

```
make KERNEL=<LINUX_SOURCE_DIR>
```

To cross-compile for another architecture, you need to set the `KERNEL`, `ARCH`, and `CROSS` variables.  An example of a command to cross-compile for 32-bit ARM might look like the following:

```
make KERNEL=/home/user/linux-build/src/linux-5.5.16/ ARCH=arm CROSS=/usr/bin/arm-linux-gnueabihf-  cross
```

The output should be a file called `disable_selinux.ko`.  


## Loading  

To load the kernel module, you can try any of the following:  

```
# insmod /tmp/disable_selinux.ko
```

```
# ln -s /tmp/disable_selinux.ko /lib/modules/`uname -r`

# depmod -a

# modprobe disable_selinux
```

You can also try the custom `modload` utility from this repository:

```
# /tmp/modload /tmp/disable_selinux.ko
```

If the module is loaded successfully, you should see some output in the kernel log:

```
# dmesg | grep SeanP
[   35.808464] [SeanP] SELinux disabler kernel module loaded
[   35.818566] [SeanP] [SUCCESS] Wrote 0 to SELinux enforce file
```

---------------------------------------------

For inquiries and/or information about me, visit my **[personal website](https://SeanPesce.github.io)**.  
