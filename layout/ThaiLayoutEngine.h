
/*
 * @(#)ThaiLayoutEngine.h	1.3 00/03/15
 *
 * (C) Copyright IBM Corp. 1998, 1999, 2000 - All Rights Reserved
 *
 */

#ifndef __THAILAYOUTENGINE_H
#define __THAILAYOUTENGINE_H

#include "LETypes.h"
#include "LEFontInstance.h"
#include "LayoutEngine.h"

#include "ThaiShaping.h"

/**
 * This class implements layout for the Thai script, using the ThaiShapingClass.
 * All existing Thai fonts use an encoding which assigns character codes to all
 * the variant forms needed to display accents and tone marks correctly in context.
 * This class can deal with fonts using the Microsoft, Macintosh, and WorldType encodings.
 */
class ThaiLayoutEngine : public LayoutEngine
{
public:
	/**
	 * This constructs an instance of ThaiLayoutEngine for the given font, script and
	 * language. It examines the font, using LEFontInstance::canDisplay, to set fGlyphSet
	 * and fErrorChar. (see below)
	 *
	 * @param fontInstance - the font
	 * @param scriptCode - the script
	 * @param languageCode - the language
	 *
	 * @see LEFontInstance
	 * @see ScriptAndLanguageTags.h for script and language codes
	 */
    ThaiLayoutEngine(const LEFontInstance *fontInstance, le_int32 scriptCode, le_int32 languageCode);

	/**
	 * The destructor, virtual for correct polymorphic invocation.
	 */
    virtual ~ThaiLayoutEngine();

protected:
	/**
	 * A small integer indicating which Thai encoding
	 * the font uses.
	 *
	 * @see ThaiShaping
	 */
    le_uint8 fGlyphSet;

	/**
	 * The character used as a base for vowels and
	 * tone marks that are out of sequence. Usually
	 * this will be Unicode 0x25CC, if the font can
	 * display it.
	 *
	 * @see ThaiShaping
	 */
    LEUnicode fErrorChar;

	/**
	 * This method performs Thai layout. It calls ThaiShaping::compose to
	 * generate the correct contextual character codes, and then calls
	 * mapCharsToGlyphs to generate the glyph indices.
	 *
	 * Input parameters:
	 * @param chars - the input character context
	 * @param offset - the index of the first character to process
	 * @param count - the number of characters to process
	 * @param max - the number of characters in the input context
	 * @param rightToLeft - true if the text is in a right to left directional run
	 *
	 * Output parameters:
	 * @param glyphs - the glyph index array
	 * @param charIndices - the character index array
	 * @param success - set to an error code if the operation fails
	 *
	 * @return the number of glyphs in the glyph index array
	 *
	 * @see ThaiShaping
	 */
    virtual le_int32 computeGlyphs(const LEUnicode chars[], le_int32 offset, le_int32 count, le_int32 max, le_bool rightToLeft,
        LEGlyphID *&glyphs, le_int32 *&charIndices, LEErrorCode &success);
};

#endif

