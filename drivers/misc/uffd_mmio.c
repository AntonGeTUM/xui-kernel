#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/io.h>

#define UFFD_MMIO_SIZE 0x1000

struct uffd_mmio_dev {
    void __iomem *regs;
    struct resource *res;
};

static int uffd_mmio_probe(struct platform_device *pdev)
{
    struct uffd_mmio_dev *dev;
    struct resource *res;

    dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res)
        return -ENODEV;

    dev->regs = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(dev->regs))
        return PTR_ERR(dev->regs);

    platform_set_drvdata(pdev, dev);

    pr_info("uffd_mmio: probed at %pa\n", &res->start);
    return 0;
}

static int uffd_mmio_remove(struct platform_device *pdev)
{
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
