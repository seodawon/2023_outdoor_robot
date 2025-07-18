/*
 * ringbuffer.c
 *
 *  Created on: Aug 24, 2018
 *      Author: sadeesh
 */
#include "ringbuffer.h"
#include "roboClaw.h"
/**
 * @file
 * Implementation of ring buffer functions.
 */

void ring_buffer_init(ring_buffer_t *buffer) {
  buffer->tail_index = 0;
  buffer->head_index = 0;

  /** debug features initialize. */
  buffer->byte_enqueue_hit = 0;
  buffer->byte_dequeue_hit = 0;

//  	for(int i=0;i<512;i++){
//  	  uint8_t a = buffer->buffer[i];
//  	  HAL_UART_Transmit(&huart1, &a, 1, 10);
//  	  HAL_Delay(1000);
//  	}
//  uint8_t a = buffer[i];
//  HAL_UART_Transmit(&huart1, &a, 1, 10);
//  HAL_Delay(1000);
}

void ring_buffer_queue(ring_buffer_t *buffer, uint8_t data) {
  /* Is buffer full? */
  if(ring_buffer_is_full(buffer)) {
    /* Is going to overwrite the oldest byte */
    /* Increase tail index */
    buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK);
  }

  /* Place data in buffer */
  buffer->buffer[buffer->head_index] = data;
  buffer->head_index = ((buffer->head_index + 1) & RING_BUFFER_MASK);

  /** debug features update. */
  buffer->byte_enqueue_hit ++;

}

void ring_buffer_queue_arr(ring_buffer_t *buffer, uint8_t *data, ring_buffer_size_t size) {
  /* Add bytes; one by one */
  ring_buffer_size_t i;
  for(i = 0; i < size; i++) {
    ring_buffer_queue(buffer, data[i]);
  }
}

uint8_t ring_buffer_dequeue(ring_buffer_t *buffer, uint8_t *data) {
  if(ring_buffer_is_empty(buffer)) {
    /* No items */
    return 0;
  }

  *data = buffer->buffer[buffer->tail_index];
  buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK);

  /** debug features update. */
  buffer->byte_dequeue_hit ++;

  return 1;
}

uint8_t ring_buffer_dequeue_arr(ring_buffer_t *buffer, uint8_t *data, ring_buffer_size_t len) {
  if(ring_buffer_is_empty(buffer)) {
    /* No items */
    return 0;
  }

  uint8_t *data_ptr = data;
  ring_buffer_size_t cnt = 0;
  while((cnt < len) && ring_buffer_dequeue(buffer, data_ptr)) {
    cnt++;
    data_ptr++;
  }
  return cnt;
}

ring_buffer_size_t ring_buffer_peek(ring_buffer_t *buffer, uint8_t *data, ring_buffer_size_t index) {
  if(index >= ring_buffer_num_items(buffer)) {
    /* No items at index */
    return 0;
  }

  /* Add index to pointer */
  ring_buffer_size_t data_index = ((buffer->tail_index + index) & RING_BUFFER_MASK);
  *data = buffer->buffer[data_index];
  return 1;
}

extern inline uint8_t ring_buffer_is_empty(ring_buffer_t *buffer);
extern inline uint8_t ring_buffer_is_full(ring_buffer_t *buffer);
extern inline uint8_t ring_buffer_num_items(ring_buffer_t *buffer);


