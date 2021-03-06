#ifndef OMG_TDDS_PUB_PUBLISHER_HPP_
#define OMG_TDDS_PUB_PUBLISHER_HPP_

/* Copyright 2010, Object Management Group, Inc.
 * Copyright 2010, PrismTech, Corp.
 * Copyright 2010, Real-Time Innovations, Inc.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dds/core/types.hpp>
#include <dds/core/TEntity.hpp>
#include <dds/core/cond/StatusCondition.hpp>
#include <dds/pub/qos/DataWriterQos.hpp>
#include <dds/pub/qos/PublisherQos.hpp>
#include <dds/domain/DomainParticipant.hpp>



namespace dds { namespace pub {
  template <typename DELEGATE>
  class TPublisher;

  class PublisherListener;
} }

/**
 * The Publisher acts on the behalf of one or several DataWriter objects
 * that belong to it. When it is informed of a change to the data associated
 * with one of its DataWriter objects, it decides when it is appropriate
 * to actually send the data-update message. In making this decision, it
 * considers any extra information that goes with the data (timestamp,
 * writer, etc.) as well as the QoS of the Publisher and the DataWriter.
 */
template <typename DELEGATE>
class dds::pub::TPublisher : public dds::core::TEntity<DELEGATE> {
public:
  typedef dds::pub::PublisherListener                 Listener;

public:
  OMG_DDS_REF_TYPE(TPublisher, dds::core::TEntity, DELEGATE)


  /**
   * Create a new <code>Publisher</code>.
   * @param dp the domain participant
   */
  TPublisher(const dds::domain::DomainParticipant& dp);

  /**
   * Create a <code>Publisher</code> with the desired QoS policies
   * and attaches to it the specified PublisherListener.
   * If the specified QoS policies are not consistent, the operation
   * will fail and an exception will be thrown.
   *
   * @param dp the domain participant.
   * @param qos the publisher qos policies.
   * @param listener the publisher listener
   * @param mask the mask of events notified to the listener.
   */
  TPublisher(const dds::domain::DomainParticipant& dp,
      const dds::pub::qos::PublisherQos& qos,
      dds::pub::PublisherListener* listener = NULL,
      const dds::core::status::StatusMask& mask = dds::core::status::StatusMask::all());

  ~TPublisher();

  //==========================================================================

  /**
   * Get the publisher qos policies.
   */
  const dds::pub::qos::PublisherQos qos() const;


  /**
   * Set the new qos policies for this publisher.
   *
   * @param pqos the new publisher QoS
   */
  void qos(const dds::pub::qos::PublisherQos& pqos);


  /**
   * Set the new qos policies for this publisher.
   *
   * @param pqos the new publisher QoS
   */
  TPublisher& operator <<(const dds::pub::qos::PublisherQos& pqos);

  /**
   * Get the publisher qos policies.
   */
  TPublisher& operator >> (dds::pub::qos::PublisherQos& qos);

  /**
   * This operation sets a default value of the <code>DataWriterQos</code>
   * which will be used for newly created <code>DataWriter</code>entities
   * for which no <code>DataWriterQos</code> is provided in the constructor.
   * This operation will check that the resulting policies are self consistent;
   * if they are not, the operation will have no effect and raise an
   * InconsistentPolicyError.
   */
  TPublisher& default_writer_qos(const dds::pub::qos::DataWriterQos& dwqos);

  /**
   * This operation retrieves the default value of the <code>DataWriterQos</code>,
   * that is, the QoS policies which will be used for newly created DataWriter
   * that don't provide a QoS parameter in the constructor.
   */
  dds::pub::qos::DataWriterQos default_writer_qos() const;

  //==========================================================================

  /**
   * Set/Reset the listener associated with this publisher.
   * Listener un-registration is performed by setting the listener to NULL.
   *
   * @param plistener the publisher listener.
   */
  void listener(Listener* plistener,
      const dds::core::status::StatusMask& event_mask);

  /**
   * Get the currently registered listener.
   */
  Listener* listener() const;

  //==========================================================================

  /**
   * This operation blocks the calling thread until either all data written
   * by the reliable DataWriter entities is acknowledged by all matched
   * reliable DataReader entities, or else the duration specified by the
   * max_wait parameter elapses, whichever happens first.
   * A normal return indicates that all the samples written have been
   * acknowledged by all reliable matched data readers; A TimeoutError
   * indicates that max_wait elapsed before all the data was acknowledged.
   */
  void wait_for_acknowledgments(const dds::core::Duration& timeout);



  //==========================================================================

  /**
   * Return the <code>DomainParticipant</code> that owns this Publisher.
   */
  const dds::domain::DomainParticipant& participant() const;

};


#endif /* OMG_TDDS_PUB_PUBLISHER_HPP_ */
