# Copyright (c) 1999-2003, International Business Machines Corporation and
# others. All Rights Reserved.
# A list of UCM's to build
# Note: 
#
#   If you are thinking of modifying this file, READ THIS. 
#
# Instead of changing this file [unless you want to check it back in],
# you should consider creating a 'ucmlocal.mk' file in this same directory.
# Then, you can have your local changes remain even if you upgrade or re
# configure the ICU.
#
# Example 'ucmlocal.mk' files:
#
#  * To add an additional converter to the list: 
#    _____________________________________________________
#    |  UCM_SOURCE_LOCAL =  myconverter.ucm ...
#
#  * To REPLACE the default list and only build with a few
#     converters:
#    _____________________________________________________
#    |  UCM_SOURCE = ibm-913.ucm ibm-949.ucm ibm-37.ucm
#
# If you are planning to exclude EBCDIC mappings in you data then please delete
# ucmebcdic.mk from the <icu>/source/data directory
#

UCM_SOURCE_FILES = ibm-367_P100-1995.ucm\
ibm-437_P100-1995.ucm\
ibm-813_P100-1995.ucm\
ibm-850_P100-1995.ucm\
ibm-851_P100-1995.ucm\
ibm-852_P100-1995.ucm\
ibm-855_P100-1995.ucm\
ibm-856_P100-1995.ucm\
ibm-857_P100-1995.ucm\
ibm-858_P100-1997.ucm\
ibm-860_P100-1995.ucm\
ibm-861_P100-1995.ucm\
ibm-862_P100-1995.ucm\
ibm-863_P100-1995.ucm\
ibm-864_X110-1999.ucm\
ibm-865_P100-1995.ucm\
ibm-866_P100-1995.ucm\
ibm-867_P100-1998.ucm\
ibm-868_P100-1995.ucm\
ibm-869_P100-1995.ucm\
ibm-874_P100-1995.ucm\
ibm-878_P100-1996.ucm\
ibm-901_P100-1999.ucm\
ibm-902_P100-1999.ucm\
ibm-921_P100-1995.ucm\
ibm-922_P100-1999.ucm\
ibm-1006_P100-1995.ucm\
ibm-1051_P100-1995.ucm\
ibm-1098_P100-1995.ucm\
ibm-1124_P100-1996.ucm\
ibm-1125_P100-1997.ucm\
ibm-1129_P100-1997.ucm\
ibm-1131_P100-1997.ucm\
ibm-1133_P100-1997.ucm\
ibm-1162_P100-1999.ucm\
ibm-1250_P100-1995.ucm\
ibm-1251_P100-1995.ucm\
ibm-1252_P100-2000.ucm\
ibm-1253_P100-1995.ucm\
ibm-1254_P100-1995.ucm\
ibm-1255_P100-1995.ucm\
ibm-1256_P110-1997.ucm\
ibm-1257_P100-1995.ucm\
ibm-1258_P100-1997.ucm\
ibm-1275_P100-1995.ucm\
ibm-1276_P100-1995.ucm\
ibm-1277_P100-1995.ucm\
ibm-1280_P100-1996.ucm\
ibm-1281_P100-1996.ucm\
ibm-1282_P100-1996.ucm\
ibm-1283_P100-1996.ucm\
ibm-5351_P100-1998.ucm\
ibm-5353_P100-1998.ucm\
ibm-942_P12A-1999.ucm\
ibm-943_P130-1999.ucm\
ibm-949_P110-1999.ucm\
ibm-950_P110-1999.ucm\
ibm-964_P110-1999.ucm\
ibm-1363_P110-1997.ucm\
ibm-1381_P110-1999.ucm\
ibm-33722_P120-1999.ucm\
macos-2566-10.2.ucm\
jisx-201.ucm jisx-208.ucm jisx-212.ucm\
iso-ir-165.ucm cns-11643-1992.ucm\
ibm-5478_P100-1995.ucm\
icu-internal-25546.ucm lmb-excp.ucm

