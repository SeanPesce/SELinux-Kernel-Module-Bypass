// Modified by Sean Pesce from original source by Ciro Santilli:
// https://github.com/cirosantilli/linux-kernel-module-cheat/blob/91583552ba2c2d547c8577ac888ab9f851642b25/kernel_module/user/myinsmod.c
//
// Tool for loading kernel modules without dealing with SELinux filesystem contexts.
// This is an alternative to insmod/finit_module.
//
// Compile with the following command:
// gcc modload.c -o modload

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


#define init_module(mod, len, opts) syscall(__NR_init_module, mod, len, opts)


int main(int argc, char** argv)
{
    const char* params;
    int fd;
    size_t image_sz;
    struct stat st;
    void* image;
    ssize_t read_sz = 0;
    int err = 0;

    if (argc < 2)
    {
        printf("Usage %s <kernel module> [args]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (argc < 3)
    {
        params = "";
    }
    else
    {
        params = argv[2];
    }
    
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return EXIT_FAILURE;
    }
    
    err = fstat(fd, &st);
    if (err)
    {
        perror("fstat");
        close(fd);
        return EXIT_FAILURE;
    }
    
    image_sz = st.st_size;
    image = malloc(image_sz);
    if (image == NULL)
    {
        perror("malloc");
        close(fd);
        return EXIT_FAILURE;
    }
    
    read_sz = read(fd, image, image_sz);
    if (read_sz < 0)
    {
        perror("read");
        close(fd);
        free(image);
        return EXIT_FAILURE;
    }
    else if ((size_t)read_sz != image_sz)
    {
        printf("[WARNING] Expected %lu bytes but read returned %ld\n", image_sz, read_sz);
    }
    
    if (close(fd))
    {
        perror("close");
    }
    
    if (init_module(image, image_sz, params) != 0)
    {
        perror("init_module");
        free(image);
        return EXIT_FAILURE;
    }
    
    free(image);
    return EXIT_SUCCESS;
}
