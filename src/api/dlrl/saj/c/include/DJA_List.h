/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2013 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class DDS_List */

#ifndef _Included_DDS_List
#define _Included_DDS_List
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     DDS_List
 * Method:    jniLength
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_DDS_List_jniLength
  (JNIEnv *, jobject);

/*
 * Class:     DDS_List
 * Method:    jniClear
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_DDS_List_jniClear
  (JNIEnv *, jobject);

/*
 * Class:     DDS_List
 * Method:    jniAddedElements
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_DDS_List_jniAddedElements
  (JNIEnv *, jobject);

/*
 * Class:     DDS_List
 * Method:    jniModifiedElements
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_DDS_List_jniModifiedElements
  (JNIEnv *, jobject);

/*
 * Class:     DDS_List
 * Method:    jniRemove
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_DDS_List_jniRemove
  (JNIEnv *, jobject);

/*
 * Class:     DDS_List
 * Method:    jniDeleteList
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_DDS_List_jniDeleteList
  (JNIEnv *, jobject);

/*
 * Class:     DDS_List
 * Method:    jniRemovedElements
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_DDS_List_jniRemovedElements
  (JNIEnv *, jobject);

/*
 * Class:     DDS_List
 * Method:    jniGetValues
 * Signature: ()[LDDS/ObjectRoot;
 */
JNIEXPORT jobjectArray JNICALL Java_DDS_List_jniGetValues
  (JNIEnv *, jobject);

/*
 * Class:     DDS_List
 * Method:    jniGet
 * Signature: (I)LDDS/ObjectRoot;
 */
JNIEXPORT jobject JNICALL Java_DDS_List_jniGet
  (JNIEnv *, jobject, jint);

/*
 * Class:     DDS_List
 * Method:    jniPut
 * Signature: (ILDDS/ObjectRoot;)V
 */
JNIEXPORT void JNICALL Java_DDS_List_jniPut
  (JNIEnv *, jobject, jint, jobject);

/*
 * Class:     DDS_List
 * Method:    jniAdd
 * Signature: (LDDS/ObjectRoot;)V
 */
JNIEXPORT void JNICALL Java_DDS_List_jniAdd
  (JNIEnv *, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif
