/*
*******************************************************************************
*
*   Copyright (C) 2003, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
*******************************************************************************
*   file name:  unorm_it.h
*   encoding:   US-ASCII
*   tab size:   8 (not used)
*   indentation:4
*
*   created on: 2003jan21
*   created by: Markus W. Scherer
*/

#ifndef __UNORM_IT_H__
#define __UNORM_IT_H__

#include "unicode/utypes.h"

#if !UCONFIG_NO_COLLATION

#include "unicode/uiter.h"
#include "unicode/unorm.h"

/**
 * Normalizing UCharIterator wrapper.
 * This internal API basically duplicates the functionality of the C++ Normalizer
 * but
 * - it actually implements a character iterator (UCharIterator)
 *   with few restrictions (see unorm_setIter())
 * - it supports UCharIterator getState()/setState()
 * - it uses lower-level APIs and buffers more text and states,
 *   hopefully resulting in higher performance
 *
 * @internal
 */
struct UNormIterator;
typedef struct UNormIterator UNormIterator;

/**
 * Open a normalizing iterator. Must be closed later.
 * Use unorm_setIter().
 *
 * @param pErrorCode ICU error code
 * @return an allocated and pre-initialized UNormIterator
 * @internal
 */
U_CAPI UNormIterator * U_EXPORT2
unorm_openIter(UErrorCode *pErrorCode);

/**
 * Close a normalizing iterator.
 *
 * @param uni UNormIterator from unorm_openIter()
 * @internal
 */
U_CAPI void U_EXPORT2
unorm_closeIter(UNormIterator *uni);

/**
 * Set a UCharIterator and a normalization mode for the normalizing iterator
 * to wrap. The normalizing iterator will read from the character iterator,
 * normalize the text, and in turn deliver it with its own wrapper UCharIterator
 * interface which it returns.
 *
 * The source iterator remains at its current position through the unorm_setIter()
 * call but will be used and moved as soon as the
 * the returned normalizing iterator is.
 *
 * The returned interface pointer is valid for as long as the normalizing iterator
 * is open and until another unorm_setIter() call is made on it.
 *
 * The normalizing iterator's UCharIterator interface has the following properties:
 * - getIndex() and move() will almost always return UITER_UNKNOWN_INDEX
 * - getState() will return UITER_NO_STATE for unknown states for positions
 *              that are not at normalization boundaries
 *
 * @param uni UNormIterator from unorm_openIter()
 * @param iter The source text UCharIterator to be wrapped. It is aliases into the normalizing iterator.
 *             Must support getState() and setState().
 * @param mode The normalization mode.
 * @param pErrorCode ICU error code
 * @return an alias to the normalizing iterator's UCharIterator interface
 * @internal
 */
U_CAPI UCharIterator * U_EXPORT2
unorm_setIter(UNormIterator *uni, UCharIterator *iter, UNormalizationMode mode, UErrorCode *pErrorCode);

#endif
#endif
