# Daisy Seed STM32Cube Template for PlatformIO

this is a starter workspace for using libDaisy + DaisySP on the Daisy Seed using STM32Cube on PlatformIO.
was largely uplifted from [Zetaohm's repository](https://github.com/Zetaohm/DaisyPod_Platformio_Example), except with better compatibility and out-of-the-box user friendliness.

more info on : [PlatformIO](https://platformio.org/) &nbsp;|&nbsp; [Daisy Seed](https://www.electro-smith.com/daisy/daisy)

## how to install

since this project was specifically meant to faciliate development in VS Code with PlatformIO, you will need to install [Visual Studio Code](https://code.visualstudio.com/) as well as the [PlatformIO IDE extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide).

currently this repo uses versions of [libDaisy](https://github.com/netherwaves/libDaisy) and [DaisySP]() that were locally forked since they required adding some PlatformIO-relevant files. thus, they were added as Git submodules.

to install them, simply run the following commands:

```
git submodule init
git submodule update --recurse-submodules --sync
```

once this is done, you can compile the whole program like you would in PlatformIO by pressing the checkmark in the toolbar.

## bootloader

upon first compilation, you will have access to a custom build task that lets you upload the [Daisy bootloader](https://electro-smith.github.io/libDaisy/md_doc_md__a7__getting__started__daisy__bootloader.html) on the device directly from the library folder. it will be available in the Browser sidebar by clicking on the PlatformIO icon, navigating to the Project Tasks tab down in the `electrosmith_daisy` environment, in the Custom folder. the task is named "Upload bootloader".

additionally, you can change the boot type (like you would in the regular Daisy toolchain) by editing the platformio.ini file, at line 41:

```ini
# BOOT TYPE
# flash | sram | qspi
boot_type = sram
```

memory sectors will automatically be reassigned depending on your choice.
