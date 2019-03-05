#include "buffer.h"

/* the buffer */
buffer_item buffer[BUFFER_SIZE];

int insert_item(buffer_item item) {
/* insert item into buffer
return 0 if successful, otherwise
return -1 indicating an error condition */
  int f;
  sem_getvalue(&full, &f);
  if(f >= BUFFER_SIZE or f < 0)
    return -1;
  buffer[f] = item;
  return 0;
}

int remove_item(buffer_item *item) {
/* remove an object from buffer
placing it in item
return 0 if successful, otherwise
return -1 indicating an error condition */
  int e;
  sem_getvalue(&empty, &e);
  if(e >= BUFFER_SIZE or e < 0)
    return -1;
  *item = buffer[BUFFER_SIZE - e];
  //buffer[BUFFER_SIZE - empty] = -1;
  return 0;
}
