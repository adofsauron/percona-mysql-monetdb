/*
 Copyright (c) 2010, 2023, Oracle and/or its affiliates.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License, version 2.0,
 as published by the Free Software Foundation.

 This program is also distributed with certain software (including
 but not limited to OpenSSL) that is licensed under separate terms,
 as designated in a particular file or component or in included license
 documentation.  The authors of MySQL hereby grant you an additional
 permission to link the program and your derivative works with the
 separately licensed software that they have included with MySQL.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License, version 2.0, for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/
/*
 * mysql_utils_jtie.hpp
 */

#ifndef mysql_utils_jtie_hpp
#define mysql_utils_jtie_hpp

// API to implement against
#include "decimal_utils.hpp"
#include "CharsetMap.hpp"
#include "MysqlUtilsWrapper.hpp"

// libraries
#include "ndbjtie_defs.hpp"
#include "helpers.hpp"
#include "jtie.hpp"

// ---------------------------------------------------------------------------
// MySQL Utilities JTie Type Definitions
// ---------------------------------------------------------------------------

// type alias for mapping the 2-element int array in CharsetMap::recode();
// provides basic error checking by jtie requiring array length >= 2.
typedef ttrait< _jintArray *, Int32 *, _jtie_j_ArrayMapper< _jtie_j_BoundedArray< _jintArray, 2 > > * > ttrait_Int32_2p_a;

JTIE_DEFINE_PEER_CLASS_MAPPING(CharsetMap,
                               c_m_n_m_CharsetMap)

// ---------------------------------------------------------------------------

JTIE_DEFINE_JINT_ENUM_TYPE_MAPPING(CharsetMap::RecodeStatus,
                                   c_m_n_m_CharsetMap_RecodeStatus)

// ---------------------------------------------------------------------------
// MySQL Utilities JTie Function Stubs
// ---------------------------------------------------------------------------

// The API stub functions in this file have mangled names that adhere
// to the JVM specification.  It is not necessary to include the
// function prototypes generated by the javah tool from the Java source,
// if they are declared to receive "C" linkage here.
extern "C" {

// A javah bug in JDK 5
//   http://forums.sun.com/thread.jspa?threadID=5115982&tstart=1499
// generates a wrong name for native methods in static nested classes:
//
// JDK 6 has this bug only partially fixed (nested classes as invocation
// targets but not as parameters).
//
// Outer$Inner is to be mangled as unicode escape: Outer_00024Inner, see:
//   http://java.sun.com/javase/6/docs/technotes/guides/jni/spec/design.html#wp615]Resolving%20Native%20Method%20Names

// ---------------------------------------------------------------------------

//#include "com_mysql_ndbjtie_mysql_CharsetMap.h"

/*
 * Class:     com_mysql_ndbjtie_mysql_CharsetMap
 * Method:    create
 * Signature: ()Lcom/mysql/ndbjtie/mysql/CharsetMap;
 */
JNIEXPORT jobject JNICALL
Java_com_mysql_ndbjtie_mysql_CharsetMap_create(JNIEnv * env, jclass cls)
{
    TRACE("jobject Java_com_mysql_ndbjtie_mysql_CharsetMap_create(JNIEnv *, jclass)");
    return gcreate< ttrait_c_m_n_m_CharsetMap_r >(env, cls);
}

/*
 * Class:     com_mysql_ndbjtie_mysql_CharsetMap
 * Method:    delete
 * Signature: (Lcom/mysql/ndbjtie/mysql/CharsetMap;)V
 */
JNIEXPORT void JNICALL
Java_com_mysql_ndbjtie_mysql_CharsetMap_delete(JNIEnv * env, jclass cls, jobject p0)
{
    TRACE("void Java_com_mysql_ndbjtie_mysql_CharsetMap_delete(JNIEnv *, jclass, jobject)");
    gdelete< ttrait_c_m_n_m_CharsetMap_r >(env, cls, p0);
}

/*
 * Class:     com_mysql_ndbjtie_mysql_CharsetMap
 * Method:    getName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_mysql_ndbjtie_mysql_CharsetMap_getName(JNIEnv * env, jobject obj, jint p0)
{
    TRACE("jstring Java_com_mysql_ndbjtie_mysql_CharsetMap_getName(JNIEnv *, jobject, jint)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    return gcall_mfr< ttrait_c_m_n_m_CharsetMap_ct, ttrait_char_cp_jutf8null, ttrait_int, &CharsetMap::getName >(env, obj, p0);
#else
    return gcall_fr< ttrait_char_cp_jutf8null, ttrait_c_m_n_m_CharsetMap_cr, ttrait_int, &MysqlUtilsWrapper::CharsetMap__getName >(env, NULL, obj, p0);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_CharsetMap
 * Method:    getMysqlName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_mysql_ndbjtie_mysql_CharsetMap_getMysqlName(JNIEnv * env, jobject obj, jint p0)
{
    TRACE("jstring Java_com_mysql_ndbjtie_mysql_CharsetMap_getMysqlName(JNIEnv *, jobject, jint)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    return gcall_mfr< ttrait_c_m_n_m_CharsetMap_ct, ttrait_char_cp_jutf8null, ttrait_int, &CharsetMap::getMysqlName >(env, obj, p0);
#else
    return gcall_fr< ttrait_char_cp_jutf8null, ttrait_c_m_n_m_CharsetMap_cr, ttrait_int, &MysqlUtilsWrapper::CharsetMap__getMysqlName >(env, NULL, obj, p0);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_CharsetMap
 * Method:    getCharsetNumber
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_mysql_ndbjtie_mysql_CharsetMap_getCharsetNumber(JNIEnv * env, jobject obj, jstring p0)
{
    TRACE("jint Java_com_mysql_ndbjtie_mysql_CharsetMap_getCharsetNumber(JNIEnv *, jobject, jstring)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    return gcall_mfr< ttrait_c_m_n_m_CharsetMap_ct, ttrait_int, ttrait_char_cp_jutf8null, &CharsetMap::getCharsetNumber >(env, obj, p0);
#else
    return gcall_fr< ttrait_int, ttrait_c_m_n_m_CharsetMap_cr, ttrait_char_cp_jutf8null, &MysqlUtilsWrapper::CharsetMap__getCharsetNumber >(env, NULL, obj, p0);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_CharsetMap
 * Method:    getUTF8CharsetNumber
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_com_mysql_ndbjtie_mysql_CharsetMap_getUTF8CharsetNumber(JNIEnv * env, jobject obj)
{
    TRACE("jint Java_com_mysql_ndbjtie_mysql_CharsetMap_getUTF8CharsetNumber(JNIEnv *, jobject)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    return gcall_mfr< ttrait_c_m_n_m_CharsetMap_ct, ttrait_int, &CharsetMap::getUTF8CharsetNumber >(env, obj);
#else
    return gcall_fr< ttrait_int, ttrait_c_m_n_m_CharsetMap_cr, &MysqlUtilsWrapper::CharsetMap__getUTF8CharsetNumber >(env, NULL, obj);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_CharsetMap
 * Method:    getUTF16CharsetNumber
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_com_mysql_ndbjtie_mysql_CharsetMap_getUTF16CharsetNumber(JNIEnv * env, jobject obj)
{
    TRACE("jint Java_com_mysql_ndbjtie_mysql_CharsetMap_getUTF16CharsetNumber(JNIEnv *, jobject)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    return gcall_mfr< ttrait_c_m_n_m_CharsetMap_ct, ttrait_int, &CharsetMap::getUTF16CharsetNumber >(env, obj);
#else
    return gcall_fr< ttrait_int, ttrait_c_m_n_m_CharsetMap_cr, &MysqlUtilsWrapper::CharsetMap__getUTF16CharsetNumber >(env, NULL, obj);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_CharsetMap
 * Method:    isMultibyte
 * Signature: (I)[Z
 */
JNIEXPORT jbooleanArray JNICALL
Java_com_mysql_ndbjtie_mysql_CharsetMap_isMultibyte(JNIEnv * env, jobject jobj, jint p0)
{
  TRACE("Java_com_mysql_ndbjtie_mysql_CharsetMap_isMultibyte(JNIEnv *, jobject, jint)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
  return gcall_mfr< ttrait_c_m_n_m_CharsetMap_ct, ttrait_bool_1cp_a, ttrait_int, &CharsetMap::isMultibyte >(env, jobj, p0);
#else
  return gcall_fr< ttrait_bool_1cp_a, ttrait_c_m_n_m_CharsetMap_cr, ttrait_int, &MysqlUtilsWrapper::CharsetMap__isMultibyte >(env, NULL, jobj, p0);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}
   
/*
 * Class:     com_mysql_ndbjtie_mysql_CharsetMap
 * Method:    recode
 * Signature: ([IIILjava/nio/ByteBuffer;Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL
Java_com_mysql_ndbjtie_mysql_CharsetMap_recode(JNIEnv * env, jobject obj, jintArray p0, jint p1, jint p2, jobject p3, jobject p4)
{
    TRACE("jint Java_com_mysql_ndbjtie_mysql_CharsetMap_recode(JNIEnv *, jobject, jintArray, jint, jint, jobject, jobject)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    return gcall_mfr< ttrait_c_m_n_m_CharsetMap_ct, ttrait_c_m_n_m_CharsetMap_RecodeStatus_iv/*_enum_*/, ttrait_Int32_2p_a, ttrait_int, ttrait_int, ttrait_void_0cp_bb, ttrait_void_0p_bb, &CharsetMap::recode >(env, obj, p0, p1, p2, p3, p4);
#else
    return gcall_fr< ttrait_c_m_n_m_CharsetMap_RecodeStatus_iv/*_enum_*/, ttrait_c_m_n_m_CharsetMap_cr, ttrait_Int32_2p_a, ttrait_int, ttrait_int, ttrait_void_0cp_bb, ttrait_void_0p_bb, &MysqlUtilsWrapper::CharsetMap__recode >(env, NULL, obj, p0, p1, p2, p3, p4);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

// ---------------------------------------------------------------------------

//#include "com_mysql_ndbjtie_mysql_Utils.h"

/*
 * Class:     com_mysql_ndbjtie_mysql_Utils
 * Method:    decimal_str2bin
 * Signature: (Ljava/nio/ByteBuffer;IIILjava/nio/ByteBuffer;I)I
 */
JNIEXPORT jint JNICALL
Java_com_mysql_ndbjtie_mysql_Utils_decimal_1str2bin(JNIEnv * env, jclass cls, jobject p0, jint p1, jint p2, jint p3, jobject p4, jint p5)
{
    TRACE("jint Java_com_mysql_ndbjtie_mysql_Utils_decimal_1str2bin(JNIEnv *, jclass, jobject, jint, jint, jint, jobject, jint)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    return gcall_fr< ttrait_int, ttrait_char_0cp_bb, ttrait_int, ttrait_int, ttrait_int, ttrait_void_0p_bb, ttrait_int, &::decimal_str2bin >(env, cls, p0, p1, p2, p3, p4, p5);
#else
    return gcall_fr< ttrait_int, ttrait_char_0cp_bb, ttrait_int, ttrait_int, ttrait_int, ttrait_void_0p_bb, ttrait_int, &MysqlUtilsWrapper::decimal_str2bin >(env, cls, p0, p1, p2, p3, p4, p5);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_Utils
 * Method:    decimal_bin2str
 * Signature: (Ljava/nio/ByteBuffer;IIILjava/nio/ByteBuffer;I)I
 */
JNIEXPORT jint JNICALL
Java_com_mysql_ndbjtie_mysql_Utils_decimal_1bin2str(JNIEnv * env, jclass cls, jobject p0, jint p1, jint p2, jint p3, jobject p4, jint p5)
{
    TRACE("jint Java_com_mysql_ndbjtie_mysql_Utils_decimal_1bin2str(JNIEnv *, jclass, jobject, jint, jint, jint, jobject, jint)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    return gcall_fr< ttrait_int, ttrait_void_0cp_bb, ttrait_int, ttrait_int, ttrait_int, ttrait_char_0p_bb, ttrait_int, &::decimal_bin2str >(env, cls, p0, p1, p2, p3, p4, p5);
#else
    return gcall_fr< ttrait_int, ttrait_void_0cp_bb, ttrait_int, ttrait_int, ttrait_int, ttrait_char_0p_bb, ttrait_int, &MysqlUtilsWrapper::decimal_bin2str >(env, cls, p0, p1, p2, p3, p4, p5);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_Utils
 * Method:    dbugPush
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_com_mysql_ndbjtie_mysql_Utils_dbugPush(JNIEnv * env, jclass cls, jstring p0)
{
    TRACE("void Java_com_mysql_ndbjtie_mysql_Utils_dbugPush(JNIEnv *, jclass, jstring)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    gcall_fv< ttrait_char_cp_jutf8null, &::dbugPush >(env, cls, p0);
#else
    gcall_fv< ttrait_char_cp_jutf8null, &MysqlUtilsWrapper::dbugPush >(env, cls, p0);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_Utils
 * Method:    dbugPop
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_mysql_ndbjtie_mysql_Utils_dbugPop(JNIEnv * env, jclass cls)
{
    TRACE("void Java_com_mysql_ndbjtie_mysql_Utils_dbugPop(JNIEnv *, jclass)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    gcall_fv< &::dbugPop >(env, cls);
#else
    gcall_fv< &MysqlUtilsWrapper::dbugPop >(env, cls);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_Utils
 * Method:    dbugSet
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_com_mysql_ndbjtie_mysql_Utils_dbugSet(JNIEnv * env, jclass cls, jstring p0)
{
    TRACE("void Java_com_mysql_ndbjtie_mysql_Utils_dbugSet(JNIEnv *, jclass, jstring)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    gcall_fv< ttrait_char_cp_jutf8null, &::dbugSet >(env, cls, p0);
#else
    gcall_fv< ttrait_char_cp_jutf8null, &MysqlUtilsWrapper::dbugSet >(env, cls, p0);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_Utils
 * Method:    dbugExplain
 * Signature: (Ljava/nio/ByteBuffer;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_mysql_ndbjtie_mysql_Utils_dbugExplain(JNIEnv * env, jclass cls, jobject p0, jint p1)
{
    TRACE("jstring Java_com_mysql_ndbjtie_mysql_Utils_dbugExplain(JNIEnv *, jclass, jobject, jint)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    return gcall_fr< ttrait_char_cp_jutf8null, ttrait_char_0p_bb, ttrait_int, &::dbugExplain >(env, cls, p0, p1);
#else
    return gcall_fr< ttrait_char_cp_jutf8null, ttrait_char_0p_bb, ttrait_int, &MysqlUtilsWrapper::dbugExplain >(env, cls, p0, p1);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

/*
 * Class:     com_mysql_ndbjtie_mysql_Utils
 * Method:    dbugPrint
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_com_mysql_ndbjtie_mysql_Utils_dbugPrint(JNIEnv * env, jclass cls, jstring p0, jstring p1)
{
    TRACE("void Java_com_mysql_ndbjtie_mysql_Utils_dbugPrint(JNIEnv *, jclass, jstring, jstring)");
#ifndef NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
    gcall_fv< ttrait_char_cp_jutf8null, ttrait_char_cp_jutf8null, &::dbugPrint >(env, cls, p0, p1);
#else
    gcall_fv< ttrait_char_cp_jutf8null, ttrait_char_cp_jutf8null, &MysqlUtilsWrapper::dbugPrint >(env, cls, p0, p1);
#endif // NDBJTIE_USE_WRAPPED_VARIANT_FOR_FUNCTION
}

// ---------------------------------------------------------------------------

} // extern "C"

#endif // mysql_utils_jtie_hpp
