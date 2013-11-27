#include <stdio.h>
#include <stdint.h>

#define CMD_SIZE 5

//dst bits must be 0 where they will be written
//TODO improve
void write_bit_value( char * dst, int offset, int16_t val, int val_size ){
  int bits_remaining = val_size;
  int available_bits = 8 - offset;
  int bit_no = 0;
  while (bits_remaining > available_bits){
    int shift = bits_remaining - available_bits;
    int16_t to_write = val >> shift;
    printf("old val : %d\n", val);
    val -= (to_write << shift);
    printf("shift : %d\n", shift);
    printf("to write : %d\n", to_write);
    printf("new val : %d\n", val);
    dst[bit_no] |= (char)to_write;
    bits_remaining -= available_bits;
    available_bits = 8;
    offset = 0;
    bit_no++;
  }
  printf("bits_remaining : %d\n", bits_remaining);
  int shift = 8 - bits_remaining + offset;
  int16_t to_write = val << shift;
  printf("shift : %d\n", shift);
  printf("to write : %d\n", to_write);
  dst[bit_no] |= (char)to_write;
} 

void writeCmd(char * dst, int cmdNo){
}

void dumpBinaryChar(char * src, int size){
}

int main(int argc, char * argv){
  char test[CMD_SIZE];
  for (int i = 0; i < CMD_SIZE; i++) test[i] = 0;
  write_bit_value(test, 0, 2, 4);
  write_bit_value(test, 4, 5, 6);
  printf("test[0] : %d\n", test[0]);
  printf("test[1] : %d\n", test[1]);
  printf("test[2] : %d\n", test[2]);

}
