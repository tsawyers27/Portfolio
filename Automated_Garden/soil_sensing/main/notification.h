#pragma once

bool send_ntfy_message(const char *message);
void ntfy_task(void *task_parameter);

enum class NotificationStatus {IDLE, IN_PROGRESS, SUCCESS, FAILED};
NotificationStatus get_notification_status();
void reset_notification_status();
void mark_notification_in_progress();