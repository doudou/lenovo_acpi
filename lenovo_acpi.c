/* Linux kernel module that disables the discrete graphics board for Lenovo
 * U330. Other lenovo laptops could work, but I don't know.
 *
 * Copyright (c) 2009: Sylvain Joyeux <sylvain.joyeux@m4x.org>
 */
#include <acpi/acpi.h>

MODULE_LICENSE("GPL");

static acpi_handle root_handle;

static int __init kill_ati(void)
{
    int i;
    acpi_status status;
    // The device handle
    acpi_handle handle;
    // The package elements
    union acpi_object package_elements[3];
    // The arguments to ATPX
    union acpi_object atpx_arg_elements[2];
    struct acpi_object_list atpx_arg;
    // For the return value of ATPX
    struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };

    status = acpi_get_handle(root_handle, "\\_SB_.PCI0.OVGA.ATPX", &handle);
    if (ACPI_FAILURE(status))
    {
        status = acpi_get_handle(root_handle, "\\_SB_.PCI0.OVGA.XTPX", &handle);
        if (ACPI_FAILURE(status))
        {
            printk("lenovo_acpi: cannot get ACPI handle: %s\n", acpi_format_exception(status));
            return -ENOSYS;
        }
        printk("lenovo_acpi: in discrete graphics mode\n");
        return 0;
    }

    for (i = 0; i < 3; ++i)
    {
        package_elements[i].type = ACPI_TYPE_INTEGER;
        package_elements[i].integer.value = 0;
    }

    atpx_arg.count = 2;
    atpx_arg.pointer = &atpx_arg_elements[0];

    atpx_arg_elements[0].type = ACPI_TYPE_INTEGER;
    atpx_arg_elements[0].integer.value = 2;

    atpx_arg_elements[1].type = ACPI_TYPE_PACKAGE;
    atpx_arg_elements[1].package.count = 3;
    atpx_arg_elements[1].package.elements = &package_elements[0];
    
    status = acpi_evaluate_object(handle, NULL, &atpx_arg, &buffer);
    if (ACPI_FAILURE(status))
    {
        printk("lenovo_acpi: ATPX method call failed: %s\n", acpi_format_exception(status));
        return -ENOSYS;
    }
    kfree(buffer.pointer);

    printk("lenovo_acpi: disabled the discrete graphics card\n");
    return 0;
}

static void dummy(void)
{
}

module_init(kill_ati);
module_exit(dummy);


