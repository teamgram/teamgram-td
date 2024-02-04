//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2023
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/telegram/MessageId.h"
#include "td/telegram/SavedMessagesTopicId.h"
#include "td/telegram/td_api.h"

#include "td/actor/actor.h"

#include "td/utils/common.h"
#include "td/utils/Promise.h"

namespace td {

class Td;

class SavedMessagesManager final : public Actor {
 public:
  SavedMessagesManager(Td *td, ActorShared<> parent);

  void get_pinned_saved_messages_topics(Promise<td_api::object_ptr<td_api::foundSavedMessagesTopics>> &&promise);

  void get_saved_messages_topics(const string &offset, int32 limit,
                                 Promise<td_api::object_ptr<td_api::foundSavedMessagesTopics>> &&promise);

  void on_get_saved_messages_topics(telegram_api::object_ptr<telegram_api::messages_SavedDialogs> &&saved_dialogs_ptr,
                                    Promise<td_api::object_ptr<td_api::foundSavedMessagesTopics>> &&promise);

  void get_saved_messages_topic_history(SavedMessagesTopicId saved_messages_topic_id, MessageId from_message_id,
                                        int32 offset, int32 limit,
                                        Promise<td_api::object_ptr<td_api::messages>> &&promise);

  void delete_saved_messages_topic_history(SavedMessagesTopicId saved_messages_topic_id, Promise<Unit> &&promise);

  void get_saved_messages_topic_message_by_date(SavedMessagesTopicId saved_messages_topic_id, int32 date,
                                                Promise<td_api::object_ptr<td_api::message>> &&promise);

  void delete_saved_messages_topic_messages_by_date(SavedMessagesTopicId saved_messages_topic_id, int32 min_date,
                                                    int32 max_date, Promise<Unit> &&promise);

  void toggle_saved_messages_topic_is_pinned(SavedMessagesTopicId saved_messages_topic_id, bool is_pinned,
                                             Promise<Unit> &&promise);

  void set_pinned_saved_messages_topics(vector<SavedMessagesTopicId> saved_messages_topic_ids, Promise<Unit> &&promise);

  void on_update_pinned_saved_messages_topics();

 private:
  static constexpr int32 MAX_GET_HISTORY = 100;  // server side limit

  void tear_down() final;

  Td *td_;
  ActorShared<> parent_;
};

}  // namespace td