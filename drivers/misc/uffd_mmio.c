#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/of_device.h>

#include <linux/uffd_mmio.h>

#define UFFD_MMIO_SIZE 0x10

struct mmio_dev *global_uffd_mmio_dev;
EXPORT_SYMBOL_GPL(global_uffd_mmio_dev);

static int uffd_mmio_probe(struct platform_device *pdev)
{
    struct mmio_dev *dev;
    struct resource *res;

    dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM; // out of memory for struct

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res)
        return -ENODEV; // no physical memory for device

    dev->mmio_base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(dev->mmio_base)) // could not map to virtual address space
        return PTR_ERR(dev->mmio_base);

    platform_set_drvdata(pdev, dev);

    global_uffd_mmio_dev = dev;

    pr_info("uffd_mmio: probed at %pa\n", &res->start);
    return 0;
}

static int uffd_mmio_remove(struct platform_device *pdev)
{
    // don't expect to actually remove device
    pr_info("uffd_mmio: removed\n");
    return 0;
}

static const struct of_device_id uffd_mmio_of_match[] = {
    { .compatible = "gem5,uffd-mmio", },
    { }
};
MODULE_DEVICE_TABLE(of, uffd_mmio_of_match);

static struct platform_driver uffd_mmio_driver = {
    .probe  = uffd_mmio_probe,
    .remove = uffd_mmio_remove,
    .driver = {
        .name           = "uffd_mmio",
        .of_match_table = uffd_mmio_of_match,
    },
};

module_platform_driver(uffd_mmio_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anton Ge");
MODULE_DESCRIPTION("Minimal uffd_mmio platform driver");
