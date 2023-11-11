#ifndef EVENT_HANDLER_H

#define EVENT_HANDLER_H

/**
 * Called at the very start of each event handler to initialize any buffers
 * which may contain stale data and to run any scripts which need to be.
 */
void initialize_event_handler();

#endif
