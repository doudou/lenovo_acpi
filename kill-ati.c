#include <linux/device.h>
#include <linux/pci.h>
#include <linux/pcieport_if.h>

// directly the card
static const int PCIE_VENDOR = 0x1002;
static const int PCIE_ID     = 0x95c4;
// The PCIe port
//static const int PCIE_VENDOR = 0x8086;
//static const int PCIE_ID     = 0x2a41;

MODULE_LICENSE("GPL");

static int __init kill_ati(void)
{
    struct pci_dev* device = pci_get_device(PCIE_VENDOR, PCIE_ID, NULL);
    pm_message_t msg = { PM_EVENT_SUSPEND };
    int ret;
    if (!device)
    {
        printk("device not found\n");
	return -EINVAL;
    }
    if (!device->dev.bus)
    {
        printk("no driver for this device\n");
	return -EINVAL;
    }
    if (!device->dev.bus->shutdown)
    {
        printk("no suspend method in driver %llx\n", device->dev.driver);
	return -EINVAL;
    }
    //if (!device->dev.driver->bus->suspend)
    //{
    //    printk("no suspend method on bus %llx\n", device->dev.driver->bus);
    //    return -EINVAL;
    //}

    //device->dev.bus->shutdown(&device->dev);
    ret = pci_set_power_state(device, PCI_D3cold);
    ////if (!pci_set_power_state(device, PCI_D0))
    ////    return -EINVAL;
    ////if (!pci_set_power_state(device, PCI_D2))
    ////    return -EINVAL;
    if (ret == -EINVAL)
        printk("invalid power state\n");
    else if (ret == -EIO)
        printk("unsupported power state\n");
    else if (ret != 0)
        printk("failed to change power state\n");
    else
        printk("power state is now %d\n", device->current_state);
    return 0;
}

static void dummy(void)
{
}

module_init(kill_ati);
module_exit(dummy);


