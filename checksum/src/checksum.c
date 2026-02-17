#include "inttypes.h"
#include "stdio.h"
#include <stdlib.h>
#include <time.h>

uint16_t compute_checksum(uint8_t *udp_packet, uint32_t len) {
  uint32_t sum = 0;

  for (uint32_t i = 0; i < len; i++) {
    sum += udp_packet[i];
  }

  return sum;
}

void print_packet(uint8_t *udp_packet, uint32_t len) {
  for (uint32_t i = 8; i < len; i++) {
    printf("%c", udp_packet[i]);
  }
}

void find_checksum_failure(uint8_t *udp_packet, uint32_t len){
  srand(time(NULL));

  uint16_t curr_checksum = compute_checksum(udp_packet, 21);
  uint16_t prev_checksum = compute_checksum(udp_packet, 21);
  while (curr_checksum != prev_checksum){
    uint16_t rand_packet = (rand() %20)+8;
    udp_packet[rand_packet] ^= (1 << ((rand()%(udp_packet[rand_packet]-1))+0));
  }
  print_packet(udp_packet, 21);
  printf("%c", prev_checksum);
}

int main(void) {
  uint8_t udp_packet[21] = {0x0B, 0xB8, // Source Port (3000),
                            0x00, 0x50, // Destination Port (80),
                            0x00,
                            0x15, // Length (8 header + 13 data = 21 bytes)
                            0x00, 0x00, // Checksum - needs to be calculated
                            // Data Portion - "Hello CPS344!"
                            0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x43, 0x50,
                            0x53, 0x33, 0x34, 0x34, 0x21};

  uint16_t checksum = compute_checksum(udp_packet, 21);
  uint16_t checksum2 = compute_checksum(udp_packet, 21);

  // Add the checksum to the udp_packet
  udp_packet[6] = (checksum >> 8) & 0xff;
  udp_packet[7] = checksum & 0xff;
  // Now, we could send it!
  printf("Checksum: %u\n", checksum);

  find_checksum_failure(udp_packet, 21);
  
  // Print data portion as char*
  printf("Data: ");
  print_packet(udp_packet, 21);
  printf("\n");

  return 0;
}
