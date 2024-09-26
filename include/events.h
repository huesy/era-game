#ifndef EVENTS_H
#define EVENTS_H

typedef enum {
  EVENT_NONE = 0,
  EVENT_PLAYER_MOVED,
  EVENT_ENEMY_SPAWNED,
  EVENT_ITEM_COLLECTED,
} EventType;

typedef struct {
  EventType type;
  void *data; // Pointer to event-specific data.
} Event;

#endif // EVENTS_H