#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

#define UFFD_MMIO_ADDR 0x500000000ULL
#define UFFD_MMIO_SIZE 0x10

static struct resource uffd_mmio_res[] = {
    {
        .start = UFFD_MMIO_ADDR,
        .end = UFFD_MMIO_ADDR + UFFD_MMIO_SIZE - 1,
        .flags = IORESOURCE_MEM,
    },
};

static struct platform_device uffd_mmio_dev = {
    .name = "uffd_mmio",
    .id = -1,
    .num_resources = ARRAY_SIZE(uffd_mmio_res),
    .resource = uffd_mmio_res,
};

static int __init uffd_mmio_dev_init(void)
{
    return platform_device_register(&uffd_mmio_dev);
}

static void __exit uffd_mmio_dev_exit(void)
{
    platform_device_unregister(&uffd_mmio_dev);
}

module_init(uffd_mmio_dev_init);
module_exit(uffd_mmio_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anton Ge");
MODULE_DESCRIPTION("uffd_mmio platform device registration");
