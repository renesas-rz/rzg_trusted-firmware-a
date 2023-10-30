#!/usr/bin/env python3
#
# Copyright (c) 2023, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

import os, sys, getopt
from struct import *


#Parameter block as dictionary of values
param_block_dict = {
                    "offset00h":0,
                    "offset04h":0,
                    "offset08h":0,
                    "offset0Ch":0,
                    "offset10h":0,
                    "offset14h":0,
                    "offset18h":0,
                    "offset1Ch":0,
                    "offset20h":0,
                    "offset24h":0,
                    "offset28h":0,
                    "offset2Ch":0,
                    "offset30h":0,
                    "offset34h":0,
                    "offset38h":0,
                    "offset3Ch":0,
                    "offset40h":0,
                    "offset44h":0,
                    "offset48h":0,
                    }


def compute_checksum():
    '''Create parameter block checksum'''
    global param_block_dict

    checksum = 0

    for value in param_block_dict.values():
        checksum += (value & 0x0000FFFF)
        checksum += ((value & 0xFFFF0000) >> 16)

    param_block_dict["offset48h"] = checksum
    return


def create_parameter_block():
    '''Create parameter block with given parameters'''
    global param_block_dict

    #Extract the command line arguments
    try:
        opts, args = getopt.gnu_getopt(sys.argv[1:], "", ["output=",
                                                          "offset00h=",  "cache_flag=",
                                                          "offset04h=",                     "xspi_wrapcfg_v=",
                                                          "offset08h=",                     "xspi_comcfg_v=",
                                                          "offset0Ch=",                     "xspi_bmcfg_v=",
                                                          "offset10h=", "bus_restore=",
                                                          "offset14h=", "loader_addr=",
                                                          "offset18h=", "loader_size=",
                                                          "offset1Ch=", "dest_addr=",
                                                          "offset24h=",                     "xspi_cmcfg1cs0_v=",
                                                          "offset28h=",                     "xspi_cssctl_v=",
                                                          "offset2Ch=",                     "xspi_liocfgcs0_v=",
                                                          "offset30h=", "pll0_ssc_ctr_v=",
                                                          "offset34h=", "bootcpu_flg=",
                                                          "offset44h=",                     "xspi_access_speed_v=",
                                                          "offset48h=", "checksum="
                                                          ])

    except getopt.GetoptError as err:
        # print error reason and exit:
        print(err)
        sys.exit(2)

    output_file = None
    checksum = None

    #Parse the command line arguments
    for option, value in opts:
        if option == "--output":
            output_file = value

        elif option in ("--offset00h", "--cache_flag"):
            param_block_dict["offset00h"] = int(value, base=16);

        elif option in ("--offset04h", "--xspi_wrapcfg_v"):
            param_block_dict["offset04h"] = int(value, base=16);

        elif option in ("--offset08h", "--xspi_comcfg_v"):
            param_block_dict["offset08h"] = int(value, base=16);

        elif option in ("--offset0Ch", "--xspi_bmcfg_v"):
            param_block_dict["offset0Ch"] = int(value, base=16);

        elif option in ("--offset10h", "--bus_restore"):
            param_block_dict["offset10h"] = int(value, base=16);

        elif option in ("--offset14h", "--loader_addr"):
            param_block_dict["offset14h"] = int(value, base=16);

        elif option in ("--offset18h", "--loader_size"):
            param_block_dict["offset18h"] = int(value, base=16);

        elif option in ("--offset1Ch", "--dest_addr"):
            param_block_dict["offset1Ch"] = int(value, base=16);

        elif option in ("--offset24h", "--xspi_cmcfg1cs0_v"):
            param_block_dict["offset24h"] = int(value, base=16);

        elif option in ("--offset28h", "--xspi_cssctl_v"):
            param_block_dict["offset28h"] = int(value, base=16);

        elif option in ("--offset2Ch", "--xspi_liocfgcs0_v"):
            param_block_dict["offset2Ch"] = int(value, base=16);

        elif option in ("--offset30h", "--pll0_ssc_ctr_v"):
            param_block_dict["offset30h"] = int(value, base=16);

        elif option in ("--offset34h", "--bootcpu_flg"):
            param_block_dict["offset34h"] = int(value, base=16);

        elif option in ("--offset44h", "--xspi_access_speed_v"):
            param_block_dict["offset44h"] = int(value, base=16);

        elif option in ("--offset48h", "--checksum"):
            param_block_dict["offset48h"] = int(value, base=16);
            checksum=True

        else:
            assert False, "unhandled option"

    if None == output_file:
        print("Error: No output file specified")
        sys.exit(3)

    if None == checksum:
        #Calculate the checksum
        compute_checksum()

    #Generate the parameter block as 'C' unsigned long in little endian form
    parameter_block_bin = pack('<LLLLLLLLLLLLLLLLLLL', param_block_dict["offset00h"], param_block_dict["offset04h"], param_block_dict["offset08h"], param_block_dict["offset0Ch"],
                                                       param_block_dict["offset10h"], param_block_dict["offset14h"], param_block_dict["offset18h"], param_block_dict["offset1Ch"],
                                                       param_block_dict["offset20h"], param_block_dict["offset24h"], param_block_dict["offset28h"], param_block_dict["offset2Ch"],
                                                       param_block_dict["offset30h"], param_block_dict["offset34h"], param_block_dict["offset38h"], param_block_dict["offset3Ch"],
                                                       param_block_dict["offset40h"], param_block_dict["offset44h"], param_block_dict["offset48h"])

    #Zero byte extend the file to be 512bytes in size
    parameter_block_bin += bytearray([0] * (0x200 -len(parameter_block_bin)))

    #Write the file
    try:
        with open(output_file, 'wb') as fh:
            fh.write(parameter_block_bin)
            print("Creation of", output_file, "is complete")

    except Exception as err:
        print("Error Writing parameter block")

    return


if __name__ == "__main__":
    create_parameter_block()
