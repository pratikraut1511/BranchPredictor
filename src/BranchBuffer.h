/*
 * BranchBuffer.h
 *
 *  Created on: 04-Nov-2019
 *      Author: prati
 */

#ifndef BRANCHBUFFER_H_
#define BRANCHBUFFER_H_

#include "defination.h"

#define NUM_OF_BITS 32

//structure to hold tag, valid and LRU
typedef struct
{
    unsigned int tag;
    unsigned int valid;
    unsigned int LRUVal;
} buffer_info;

class BranchBuffer
{
public:
    BranchBuffer(unsigned int sizeIn, unsigned int assocIn);
    virtual ~BranchBuffer();
    bool bufferPredict(unsigned int addr, char outcome);
    void updateLRU(unsigned int jIndex);
    void printBufferVal();
    void printBufferContet(unsigned int predictorMiss);
    void swap(buffer_info *val1, buffer_info *val2);
    bool sortVal(buffer_info *val);
private:
    //Common params
    unsigned long int size;
    unsigned long int assoc;
    unsigned long int block_size;
    unsigned long int num_of_sets;
    //Bits related to params
    unsigned int bits_index;
    unsigned int bits_offset;
    unsigned int bits_tag;
    unsigned int indexMask;
    unsigned int blockMask;
    //2d dynamic array
    buffer_info **buffer_memory_details;

    //Friend Class
    friend class Bimodal;
    friend class Gshare;
    friend class Hybrid;

    //hit/miss related variable
    unsigned int totalPredictionBuffer;
    unsigned int hits;
    unsigned int miss;
    unsigned int missTaken;
    float missRate;
    //Iternal Variable
    unsigned int tagValReq;
    unsigned int IndexValReq;
    unsigned int blockValReq;
    unsigned int jValIndex;

};

#endif /* BRANCHBUFFER_H_ */
