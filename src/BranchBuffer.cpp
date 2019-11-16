/*
 * BranchBuffer.cpp
 *
 *  Created on: 04-Nov-2019
 *      Author: prati
 */

#include "BranchBuffer.h"

BranchBuffer::BranchBuffer(unsigned int sizeIn, unsigned int assocIn)
{
    indexMask = 0;
    bits_index = 0;
    bits_offset = 0;
    bits_tag = 0;
    blockMask = 0;
    blockValReq = 0;
    block_size = 0;
    num_of_sets = 0;
    size = 0;
    missRate = 0;
    assoc = 0;
    buffer_memory_details = NULL;
    //hit miss
    totalPredictionBuffer = 0;
    hits = 0;
    miss = 0;
    missTaken = 0;
    //Internal variables
    tagValReq = 0;
    IndexValReq = 0;
    blockValReq = 0;
    jValIndex = 0;
    block_size = 4;
    assoc = assocIn;
    size = sizeIn;
    num_of_sets = round((size / (assoc * block_size * 1.0)));

    //round or ceil for round off of value
    bits_offset = ceil(log2(block_size));
    bits_index = ceil(log2(num_of_sets));
    bits_tag = NUM_OF_BITS - bits_index - bits_offset;
    indexMask = (unsigned int) pow(2, bits_index) - 1;
    blockMask = (unsigned int) pow(2, bits_offset) - 1;

    buffer_memory_details = new buffer_info*[num_of_sets];
    for (unsigned int i = 0; i < num_of_sets; i++)
        buffer_memory_details[i] = new buffer_info[assoc];

    for (unsigned long int i = 0; i < num_of_sets; i++)
    {
        for (unsigned long int j = 0; j < assoc; j++)
        {
            buffer_memory_details[i][j].valid = 0;
            buffer_memory_details[i][j].tag = 0;
            buffer_memory_details[i][j].LRUVal = 0;
        }
    }
}


BranchBuffer::~BranchBuffer()
{
    //free memory
    for (unsigned int i = 0; i < num_of_sets; i++)
    {
        delete[] buffer_memory_details[i];
    }
    delete[] buffer_memory_details;
}

void BranchBuffer::updateLRU(unsigned int jIndex)
{
    //Update LRU value
    for (unsigned int j = 0; j < assoc; j++)
    {
        if (jIndex != j)
        {
            buffer_memory_details[IndexValReq][j].LRUVal++;
        }
    }
}

bool BranchBuffer::bufferPredict(unsigned int addr, char outcome)
{
    bool retVal = false;
    unsigned int oldLURVal = 0;
    unsigned int refLURVal = 0;
    unsigned int LRUIndex = 0;
    bool isInvalid = false;
    bool isFound = false;

    tagValReq = (unsigned int) addr >> (bits_index + bits_offset);
    blockValReq = (unsigned int) addr >> (bits_offset);
    IndexValReq = (unsigned int) (blockValReq & indexMask);

    //increment total prediction
    totalPredictionBuffer++;
    //Find index in cache
    for (unsigned int j = 0; j < assoc; j++)
    {
        if (buffer_memory_details[IndexValReq][j].valid == 1)
        {
            if (buffer_memory_details[IndexValReq][j].tag == tagValReq)
            {
                isFound = true;
                jValIndex = j;
                break;
            }
        }
    }
    //Update LRU if isFound == true
    if (isFound)
    {
        hits++;
        oldLURVal = buffer_memory_details[IndexValReq][jValIndex].LRUVal;
        buffer_memory_details[IndexValReq][jValIndex].LRUVal = 0;
        for (unsigned int j = 0; j < assoc; j++)
        {
            if (jValIndex != j
                    && buffer_memory_details[IndexValReq][j].LRUVal < oldLURVal)
            {
                buffer_memory_details[IndexValReq][j].LRUVal++;
            }
        }
        retVal = true;
    }
    else
    {
        miss++;
        if (outcome == 't')
            missTaken++;
        //check for any invalid field
        for (unsigned int j = 0; j < assoc; j++)
        {
            if (buffer_memory_details[IndexValReq][j].valid == 0)
            {
                isInvalid = true;
                jValIndex = j;
                break;
            }
        }
        if (isInvalid)
        {
            //update tag value
            buffer_memory_details[IndexValReq][jValIndex].tag = tagValReq;
            buffer_memory_details[IndexValReq][jValIndex].valid = 1;
            buffer_memory_details[IndexValReq][jValIndex].LRUVal = 0;
            //Update LRU value
            updateLRU(jValIndex);
        }
        else
        {
            //Remove the Least freq used tag value and replace it with new tag value
            for (unsigned int j = 0; j < assoc; j++)
            {
                if (buffer_memory_details[IndexValReq][j].LRUVal > refLURVal)
                {
                    refLURVal = buffer_memory_details[IndexValReq][j].LRUVal;
                    LRUIndex = j;
                }
            }
            //update tag value
            buffer_memory_details[IndexValReq][LRUIndex].valid = 1;
            buffer_memory_details[IndexValReq][LRUIndex].LRUVal = 0;
            buffer_memory_details[IndexValReq][LRUIndex].tag = tagValReq;
            //Update LRU
            updateLRU(LRUIndex);
        }
    }
    return retVal;
}

void BranchBuffer::swap(buffer_info *val1, buffer_info *val2)
{
    buffer_info temp;

    temp.LRUVal = val1->LRUVal;
    temp.tag = val1->tag;

    val1->LRUVal = val2->LRUVal;
    val1->tag = val2->tag;

    val2->LRUVal = temp.LRUVal;
    val2->tag = temp.tag;
}

bool BranchBuffer::sortVal(buffer_info *val)
{
    buffer_info swappedVals[assoc];
    for (unsigned int i = 0; i < assoc; i++)
    {
        swappedVals[i].LRUVal = val[i].LRUVal;
        swappedVals[i].tag = val[i].tag;
    }

    for (unsigned int i = 0; i < assoc - 1; i++)
    {
        for (unsigned int j = 0; j < assoc - i - 1; j++)
        {
            if (swappedVals[j].LRUVal > swappedVals[j + 1].LRUVal)
            {
                swap(&swappedVals[j], &swappedVals[j + 1]);
            }
        }
    }

    for (unsigned int i = 0; i < assoc; i++)
    {
        if (swappedVals[i].tag == 0)
            cout << setw(4) << "    ";
        else
            cout << hex << setw(4) << swappedVals[i].tag << "    ";
    }
    cout << endl;
    return true;
}

void BranchBuffer::printBufferVal()
{
    cout << "size of BTB: " << size << endl;
    cout << "number of branches:     " << totalPredictionBuffer << endl;
    cout << "number of predictions from branch predictor:  " << hits << endl;
}

void BranchBuffer::printBufferContet(unsigned int predictorMiss)
{
    cout << "size of BTB: " << size << endl;
    cout << "number of branches: " << totalPredictionBuffer << endl;
    cout << "number of predictions from branch predictor: " << hits << endl;
    cout << "number of mispredictions from branch predictor: " << predictorMiss
            << endl;
    predictorMiss = predictorMiss + missTaken;
    cout << "number of branches miss in BTB and taken: " << missTaken << endl;
    cout << "total mispredictions: " << predictorMiss << endl;
    missRate = (predictorMiss / (totalPredictionBuffer * 1.0)) * 100;
    printf("misprediction rate:  %0.2f%%\n", missRate);
    cout << "FINAL BTB CONTENTS" << endl;

//    for (unsigned int i = 0; i < num_of_sets; i++)
//    {
//        cout << "Set  " << dec << i << ": " << "\t\t";
//        for (unsigned int j = 0; j < assoc; j++)
//        {
//            cout << hex << buffer_memory_details[i][j].tag;
//            cout << dec << "  " << buffer_memory_details[i][j].LRUVal;
//        }
//        cout << endl;
//    }

    for (unsigned int i = 0; i < num_of_sets; i++)
    {
        cout << setw(4) << "set" << dec << setw(4) << i << ":   ";
        sortVal(buffer_memory_details[i]);
    }
}
