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

package org.opensplice.dds.dcps;

/**
 * This interface is implemented by all DDS defined classes and holds the 
 * adress of the equivalent <code>gapi</code> object. The adress is stored as a 
 * <code>long</code> because the <code>gapi</code> can be compiled for any
 * platform using up to 64 bit adressing. 
 */
abstract public class SajSuperClass extends org.omg.CORBA.portable.ObjectImpl {
	
	/**
	 * The adress of the equivalent object in the <code>gapi</code> 
	 */
	private long gapiPeer = 0;
}
