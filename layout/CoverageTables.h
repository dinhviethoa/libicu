/*
 * @(#)CoverageTables.h	1.4 00/03/15
 *
 * (C) Copyright IBM Corp. 1998, 1999, 2000, 2001 - All Rights Reserved
 *
 */

#ifndef __COVERAGETABLES_H
#define __COVERAGETABLES_H

/**
 * \file
 * \internal
 */

#include "LETypes.h"
#include "OpenTypeTables.h"

U_NAMESPACE_BEGIN

struct CoverageTable
{
    le_uint16 coverageFormat;

    le_int32 getGlyphCoverage(LEGlyphID glyphID) const;
};

struct CoverageFormat1Table : CoverageTable
{
    le_uint16  glyphCount;
    LEGlyphID glyphArray[ANY_NUMBER];

    le_int32 getGlyphCoverage(LEGlyphID glyphID) const;
};

struct CoverageFormat2Table : CoverageTable
{
    le_uint16              rangeCount;
    GlyphRangeRecord    rangeRecordArray[ANY_NUMBER];

    le_int32 getGlyphCoverage(LEGlyphID glyphID) const;
};

U_NAMESPACE_END
#endif
