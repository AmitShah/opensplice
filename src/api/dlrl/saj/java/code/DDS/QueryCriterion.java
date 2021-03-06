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
package DDS;

/**
 * <P>The current implementation does not yet support the notion of a 
 * QueryCriterion, so no operations in this class are supported.</P>
 */
public interface QueryCriterion extends DDS.SelectionCriterion{
	
    String expression() throws DDS.AlreadyDeleted;
    String[] parameters() throws DDS.AlreadyDeleted;
    void set_query(String expression, String[] parameters) throws DDS.AlreadyDeleted, DDS.SQLError;
    void set_parameters(String[] parameters) throws DDS.AlreadyDeleted, DDS.SQLError;

} // interface QueryCriterion
