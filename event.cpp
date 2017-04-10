#include <iostream>
#include "event.h"
using namespace std;

// Insert an Event into an EventList sorted by time index
// time - The time at which the event takes place
// type - The type of event
void EventList::insert(double time, int type, int priority, int queue, int goback)
{
  event_count++;                        // Increment number of events in list.
  Event* eptr = new Event(time, type, priority, queue, goback);
  if (head == 0) {                      // If EventList is empty, 
    head = eptr;                        // put new event at head of list.
    eptr->next = 0;
  }
  else if (head->time >= eptr->time) {  // If the event is earlier than
    eptr->next = head;                  // all existing events, place it
    head = eptr;                        // at the head of the list.
  }
  else {                                // Otherwise, search for the
    Event* eindex;                      // correct location sorted by time. 
    eindex = head;
    while (eindex->next != 0) {
      if (eindex->next->time < eptr->time) {
        eindex = eindex->next;
      }
      else {
        break;
      }
    }
    eptr->next = eindex->next;
    eindex->next = eptr;
  }
}

// Return the Event from the head of the EventList
Event* EventList::get()
{
  if (event_count == 0) {
    return 0;
  }
  else {
    event_count--;
    Event* eptr;
    eptr = head;
    head = head->next;
    eptr->next = 0;
    return eptr;
  }
}

// Clear all Events from the EventList
void EventList::clear()
{
  Event* eptr;
  while(head)
  {
    eptr = head;
    head = head->next;
    eptr->next = 0;
    delete eptr;
  }
  event_count = 0;
}

// Remove and return first event of given type
Event* EventList::remove(int type)
{
  if (event_count == 0 || head ==0) {
    return 0;
  }
  else {
    Event* eptr;
    Event* eptr_prev = 0;
    eptr = head;

    while(eptr){
      if (eptr->type == type) {
        if (eptr_prev == 0) {
          head = eptr->next;
          eptr->next = 0;
          return eptr;
        }
        else {
          eptr_prev->next = eptr->next;
          eptr->next = 0;
          return eptr;
        }
      }
      else {
        eptr_prev = eptr;
        eptr = eptr->next;
      }
    }
    return 0;
  }
}

