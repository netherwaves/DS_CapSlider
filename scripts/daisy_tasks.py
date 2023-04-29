Import("env")

import glob
import os

BOOT_BIN = glob.glob(os.path.join(env.get("PROJECT_DIR"), 'lib/libDaisy/core/dsy_bootloader*'))[0]
USBPID = 'df11'
FLASH_ADDRESS = '0x08000000'

env.AddCustomTarget(
    name="boot",
    dependencies=None,
    actions=[
        "dfu-util -a 0 -s %(address)s:leave -D %(boot_bin)s -d ,0483:%(pid)s" %
        {'address': FLASH_ADDRESS, 'boot_bin': BOOT_BIN, 'pid': USBPID},
    ],
    title="Upload bootloader",
    description="Upload bootloader on flash memory"
)