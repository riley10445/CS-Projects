// RILEY FEE CWID 12148611

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PAGE_TABLE_SIZE 256
#define PHYSICAL_FRAMES 256
#define FRAME_SIZE 256
#define TLB_SIZE 16

unsigned char physical_memory[PHYSICAL_FRAMES][FRAME_SIZE];

uint8_t page_table[PAGE_TABLE_SIZE];
uint8_t next_free_frame = 0;
int num_faults = 0;
int num_translated = 0;
int num_tlb_misses = 0;
int tlb_replace = 0;

bool free_frames[PHYSICAL_FRAMES];

void wipeMem();
void wipePageTable();
void wipeBoolArray();
void wipeTLB();
void addToTLB(uint8_t page_num, uint8_t frame_num);
int allocate_frame();
uint8_t checkTLB(uint8_t page_num);

typedef struct {
    uint8_t page_num;
    uint8_t frame_num;
    bool valid;
} TLB_Entry;

TLB_Entry tlb[TLB_SIZE];

int main(int argc, char* argv[]) {

    wipeMem();
    wipePageTable();
    wipeBoolArray();
    wipeTLB();

    FILE *virtAdd = fopen(argv[1], "r");
    FILE *backing = fopen("BACKING_STORE.bin", "rb");
    FILE *output = fopen("output.txt", "w");

    char line[256];
    while (fgets(line, sizeof(line), virtAdd)) {
        uint32_t virtNum = (uint32_t)strtoul(line, NULL, 10);
        fprintf(output, "Virtual address: %u ", virtNum);

        uint8_t pagenum = (virtNum >> 8) & 0xFF;
        uint8_t offset = virtNum & 0xFF;

        uint8_t framenum = checkTLB(pagenum);
        if (framenum == 0xFF) {
            num_tlb_misses++;
            if (page_table[pagenum] == 0xFF) {
                num_faults++;
                framenum = allocate_frame();
                page_table[pagenum] = framenum;
                fseek(backing, pagenum * FRAME_SIZE, SEEK_SET);
                fread(physical_memory[framenum], 1, FRAME_SIZE, backing);
            } 
            else {
                framenum = page_table[pagenum];
            }
            addToTLB(pagenum, framenum);
        }
        
        uint16_t phys_addr = (framenum << 8) | offset;
        int8_t signed_value = (int8_t)physical_memory[framenum][offset];
        fprintf(output, "Physical address: %u Value: %d\n", phys_addr, signed_value);
        num_translated++;
    }

    double page_fault_rate = (double)num_faults/(double)num_translated;
    int num_tlb_hits = (num_translated - num_tlb_misses);
    double tlb_hit_rate = (double)num_tlb_hits/(double)num_translated;
    fprintf(output, "Number of Translated Addresses = %d\n"
                    "Page Faults = %d\n"
                    "Page Fault Rate = %.3f\n"
                    "TLB Hits = %d\n"
                    "TLB Hit Rate = %.3f\n",
                    num_translated, num_faults, page_fault_rate, num_tlb_hits, tlb_hit_rate);

    fclose(virtAdd);
    fclose(backing);
    fclose(output);

    return 0;
}

void wipeMem() {
    for (int i = 0; i < PHYSICAL_FRAMES; i++) {
        for (int j = 0; j < FRAME_SIZE; j++) {
            physical_memory[i][j] = 0;
        }
    }
}

void wipePageTable() {
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        page_table[i] = 0xFF;
    }
}

void wipeBoolArray() {
    for (int i = 0; i < PHYSICAL_FRAMES; i++) {
        free_frames[i] = true;
    }
}

void wipeTLB() {
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].valid = false;
    }
}

int allocate_frame() {
    int frame = next_free_frame;
    free_frames[frame] = false;
    next_free_frame++;
    return frame;
}

// ALL TLB CODE BELOW
uint8_t checkTLB(uint8_t page_num) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid && tlb[i].page_num == page_num) {
            return tlb[i].frame_num;
        }
    }
    return 0xFF;
}

void addToTLB(uint8_t page_num, uint8_t frame_num) {
    int slot = tlb_replace;
    tlb[slot].page_num = page_num;
    tlb[slot].frame_num = frame_num;
    tlb[slot].valid = true;
    tlb_replace = (tlb_replace + 1) % TLB_SIZE;
}