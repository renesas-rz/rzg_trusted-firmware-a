#! /bin/bash

#TO-DO: Change the following path to the location of the compiler on your PC
export CROSS_COMPILE=~/compiler/gcc-arm-8.3-2019.03-x86_64-aarch64-elf/bin/aarch64-elf-

#Check the correct number of arguments are passed
if [ "$#" -ne 2 ]; then
    echo "Incorrect arguments: The number of arguments is $#. There should be 2 arguments
    arg1 = Workspace path to store the BL2 and FIP bin files
    arg2 = BL33 file path (U-boot image path)"
    exit -1
fi

#TO-DO: Pass the following arguments to the script
workspace_path="$1"
bl33_file_path="$2"

tfa_project_path=$(pwd)
fip_tool_path="$tfa_project_path/tools/fiptool"
bp_tool_path="$tfa_project_path/tools/renesas/rz_boot_param"
bl2_file_path="$tfa_project_path/build/g3s/release/bl2.bin"
bl31_file_path="$tfa_project_path/build/g3s/release/bl31.bin"

#Parameter checks
parameter_check()
{
    if [ -d "$workspace_path" ]; then
        :
    else
        echo "$workspace_path" " :does not exist"
        exit -1
    fi
    if [ -f "$bl33_file_path" ]; then
        :
    else
        echo "$bl33_file_path" " :does not exist"
        exit -1
    fi
    return
}

# Commands
echo "Pre-build checks..."
parameter_check

echo "Cleanup for project..."
cd "$tfa_project_path"
make PLAT=g3s realclean BOARD=smarc
echo "Clearing workspace..."
rm -v ${workspace_path}/*

echo "Building project..."
make PLAT=g3s all BOARD=smarc
echo "Project has been built."

echo "Packaging BL31 and BL33 into fip file..."
cd "$fip_tool_path"
make fiptool
./fiptool create --align 16 --soc-fw "$bl31_file_path" --nt-fw "$bl33_file_path" "$workspace_path"/fip.bin
echo "Fip file has been created."

echo "Creating and adding block parameter to BL2 image (SPI mode)..."
cd "$bp_tool_path"
make PLAT=g3s bptool BOARD=smarc
${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_spi.bin" 0xA3000 spi
cat "$workspace_path"/bp_spi.bin "$bl2_file_path" > "$workspace_path"/bl2_bp_spi.bin
echo "Block parameter created and added for SPI mode."

echo "Creating and adding block parameter to BL2 image (eMMC mode)..."
${bp_tool_path}/bptool "$bl2_file_path" "$workspace_path/bp_mmc.bin" 0xA3000 mmc
cat "$workspace_path"/bp_mmc.bin "$bl2_file_path" > "$workspace_path"/bl2_bp_mmc.bin
echo "Block parameter created and added for eMMC mode."

echo "Converting fip bin to srec..."
cd "$tfa_project_path"
${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 "$workspace_path"/fip.bin build/g3s/release/fip.srec
echo "Fip srec created."

echo "Converting SPI BL2 bin to srec..."
${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$workspace_path"/bl2_bp_spi.bin build/g3s/release/bl2_bp_spi.srec
echo "SPI BL2 srec created."

echo "Converting eMMC BL2 bin to srec..."
${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$workspace_path"/bl2_bp_mmc.bin build/g3s/release/bl2_bp_mmc.srec
echo "eMMC BL2 srec created."
