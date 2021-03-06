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
#include "ccpp_IntMap_impl.h"
#include "ccpp_DlrlUtils.h"
#include "DLRL_Report.h"
#include "DLRL_Util.h"
#include "ccpp_ObjectRoot_impl.h"

DDS::IntMap_impl::IntMap_impl(DK_MapAdmin* collection)
{
    DLRL_INFO(INF_ENTER);

    DK_Entity_ts_duplicate(reinterpret_cast<DK_Entity*>(collection));
    map = collection;

    DLRL_INFO(INF_EXIT);
}

DDS::IntMap_impl::~IntMap_impl()
{
    DLRL_INFO(INF_ENTER);
    if(map){
        DK_Entity_ts_release(reinterpret_cast<DK_Entity*>(map));
        map = NULL;
    }
    DLRL_INFO(INF_EXIT);
}

CORBA::Long
DDS::IntMap_impl::length(
    ) THROW_VALUETYPE_ORB_EXCEPTIONS
{
    CORBA::Long length = 0;
    DLRL_Exception exception;

    DLRL_INFO(INF_ENTER);

    DLRL_Exception_init(&exception);

    length = static_cast<CORBA::Long>(DK_MapAdmin_ts_getLength(
        map,
        &exception));
    DLRL_Exception_PROPAGATE(&exception);

    DLRL_Exception_EXIT(&exception);
    DLRL_INFO(INF_EXIT);
    ccpp_DlrlUtils_us_handleException(&exception);
    return length;
}

void
DDS::IntMap_impl::clear(
    ) THROW_VALUETYPE_ORB_EXCEPTIONS
{
    DLRL_Exception exception;

    DLRL_INFO(INF_ENTER);

    DLRL_Exception_init(&exception);

    DK_MapAdmin_ts_clear(map, &exception, NULL);
    DLRL_Exception_PROPAGATE(&exception);

    DLRL_Exception_EXIT(&exception);
    DLRL_INFO(INF_EXIT);
    ccpp_DlrlUtils_us_handleException(&exception);
}

void
DDS::IntMap_impl::remove(
    ::CORBA::Long key) THROW_VALUETYPE_ORB_EXCEPTIONS
{
    DLRL_Exception exception;

    DLRL_INFO(INF_ENTER);

    DLRL_Exception_init(&exception);

    DK_MapAdmin_ts_remove(map, &exception, NULL, reinterpret_cast<void*>(&key));
    DLRL_Exception_PROPAGATE(&exception);

    DLRL_Exception_EXIT(&exception);
    DLRL_INFO(INF_EXIT);
    ccpp_DlrlUtils_us_handleException(&exception);
}

DDS::LongSeq *
DDS::IntMap_impl::keys(
    ) THROW_VALUETYPE_ORB_EXCEPTIONS
{
    DLRL_Exception exception;
    DDS::LongSeq* seq = NULL;
    DLRL_INFO(INF_ENTER);

    DLRL_Exception_init(&exception);

    DK_MapAdmin_ts_getLSElementsGeneric(
        map,
        &exception,
        NULL,
        reinterpret_cast<void**>(&seq),
        DK_MAPADMIN_ELEMENTTYPE_KEYS);
    DLRL_Exception_PROPAGATE(&exception);

    DLRL_Exception_EXIT(&exception);
    DLRL_INFO(INF_EXIT);
    ccpp_DlrlUtils_us_handleException(&exception);
    return seq;
}

DDS::LongSeq *
DDS::IntMap_impl::added_elements(
    ) THROW_VALUETYPE_ORB_EXCEPTIONS
{
    DLRL_Exception exception;
    DDS::LongSeq* seq = NULL;
    DLRL_INFO(INF_ENTER);

    DLRL_Exception_init(&exception);

    DK_MapAdmin_ts_getLSElementsGeneric(
        map,
        &exception,
        NULL,
        reinterpret_cast<void**>(&seq),
        DK_MAPADMIN_ELEMENTTYPE_ADDED);
    DLRL_Exception_PROPAGATE(&exception);

    DLRL_Exception_EXIT(&exception);
    DLRL_INFO(INF_EXIT);
    ccpp_DlrlUtils_us_handleException(&exception);
    return seq;
}

DDS::LongSeq *
DDS::IntMap_impl::removed_elements(
    ) THROW_VALUETYPE_ORB_EXCEPTIONS
{
    DLRL_Exception exception;
    DDS::LongSeq* seq = NULL;
    DLRL_INFO(INF_ENTER);

    DLRL_Exception_init(&exception);

    DK_MapAdmin_ts_getLSElementsGeneric(
        map,
        &exception,
        NULL,
        reinterpret_cast<void**>(&seq),
        DK_MAPADMIN_ELEMENTTYPE_REMOVED);
    DLRL_Exception_PROPAGATE(&exception);

    DLRL_Exception_EXIT(&exception);
    DLRL_INFO(INF_EXIT);
    ccpp_DlrlUtils_us_handleException(&exception);
    return seq;
}

DDS::LongSeq *
DDS::IntMap_impl::modified_elements(
    ) THROW_VALUETYPE_ORB_EXCEPTIONS
{
    DLRL_Exception exception;
    DDS::LongSeq* seq = NULL;
    DLRL_INFO(INF_ENTER);

    DLRL_Exception_init(&exception);

    DK_MapAdmin_ts_getLSElementsGeneric(
        map,
        &exception,
        NULL,
        reinterpret_cast<void**>(&seq),
        DK_MAPADMIN_ELEMENTTYPE_MODIFIED);
    DLRL_Exception_PROPAGATE(&exception);

    DLRL_Exception_EXIT(&exception);
    DLRL_INFO(INF_EXIT);
    ccpp_DlrlUtils_us_handleException(&exception);
    return seq;
}

CORBA::ValueBase*
DDS::IntMap_impl::_copy_value(
    )
{
    DLRL_INFO(INF_ENTER);
    /*dummy implementation*/

    DLRL_INFO(INF_EXIT);
    return NULL;
}

DK_ObjectAdmin*
DDS::IntMap_impl::getObjectRootAdmin(
    DLRL_Exception* exception,
    DDS::ObjectRoot* objectRoot,
    const char* name)
{
    DDS::ObjectRoot_impl* rootImpl;
    DK_ObjectAdmin* admin;

    DLRL_INFO(INF_ENTER);

    assert(exception);
    assert(objectRoot);
    assert(name);

    rootImpl = dynamic_cast<DDS::ObjectRoot_impl*>(objectRoot);
    if(!rootImpl)
    {
        DLRL_Exception_THROW(exception, DLRL_BAD_PARAMETER, "%s parameter does "
            "not represent a valid DLRL object. Invalid inheritance hierarchy.",
            name);
    }
    admin = rootImpl->object;
    if(!admin)
    {
        DLRL_Exception_THROW(exception, DLRL_BAD_PARAMETER, "%s parameter does "
            "not represent a valid DLRL object. Unable to locate DLRL kernel "
            "object.", name);
    }

    DLRL_Exception_EXIT(exception);
    DLRL_INFO(INF_EXIT);
    return admin;
}

