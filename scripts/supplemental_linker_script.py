from os.path import join, isfile
Import("env")

# BOOTLOADER CONFIGS
board = env.BoardConfig()

# LINKER STUFF
BUILD_DIR = env.get('BUILD_DIR')
TARGET = env.get('PROGNAME')

# manage boot type and update RAM + flash sizes
# TODO: find way to display "QSPI" instead of "Flash"?
BOOT_TYPE = env.GetProjectOption("boot_type")
if BOOT_TYPE == 'flash':
    board.update("upload.offset_address", '0x08000000')
    board.update("upload.maximum_size", 131072)             # 128KB
    board.update("upload.maximum_ram_size", 524288)         # 512KB

elif BOOT_TYPE in {'sram', 'qspi'}:
    board.update("upload.offset_address", '0x90040000')
    env.Append(CCFLAGS=['-DBOOT_APP'])

    if BOOT_TYPE == 'sram':
        board.update('upload.maximum_size', 491520)         # 480KB

    if BOOT_TYPE == 'qspi':
        board.update('upload.maximum_ram_size', 524288)     # 512KB
        board.update('upload.maximum_size', 8126464)        # 7.75MB

# link flags
# otherwise there are errors. Do Not Remove !!!!!!!
print("running supplimental linker script . . .")
env.Append(
    LINKFLAGS=[
        "-static",
        "-mfloat-abi=hard",
        "-mfpu=fpv4-sp-d16",

        '-Wl,-Map=%(build_dir)s/%(target)s.map,--cref' % {
            'build_dir': BUILD_DIR, 'target': TARGET},
        '-Wl,--gc-sections',
        '-Wl,--print-memory-usage',
        '-Wl,-Tlib/libDaisy/core/STM32H750IB_%s.lds' % BOOT_TYPE
    ]
)
env.Append(
    CFLAGS=[
        '-std=gnu11',
    ]
)
env.Append(
    CXXFLAGS=[
        "-fno-exceptions",
        "-fasm",
        "-finline",
        "-finline-functions-called-once",
        "-fshort-enums",
        "-fno-move-loop-invariants",
        "-fno-unwind-tables",
        "-fno-rtti",
        "-Wno-register",
        "-std=gnu++14"
    ]
)

# patches
FRAMEWORK_DIR = env.PioPlatform().get_package_dir("framework-stm32cubeh7")
patchflag_path = join(FRAMEWORK_DIR, ".patching-done")

USBD_CLASS = join(FRAMEWORK_DIR, "Middlewares", "ST", "STM32_USB_Device_Library")

patches = [
    {
    "original": join(USBD_CLASS, "Class", "CDC", "Inc", "usbd_cdc.h"),
    "patch": join("patches", "usbd_cdc.h.patch")
    },
    {
    "original": join(USBD_CLASS, "Core", "Inc", "usbd_core.h"),
    "patch": join("patches", "usbd_core.h.patch")
    }
]

if not isfile(join(FRAMEWORK_DIR, ".patching-done")):
    print("patching USB library . . .")
    for patch in patches:
        original_file = patch["original"]
        patched_file = patch["patch"]

        assert isfile(original_file) and isfile(patched_file)

        env.Execute("patch %s %s" % (original_file, patched_file))

        # env.Execute("touch " + patchflag_path)


    def _touch(path):
        with open(path, "w") as fp:
            fp.write("")

    env.Execute(lambda *args, **kwargs: _touch(patchflag_path))