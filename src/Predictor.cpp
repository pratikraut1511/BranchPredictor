/*
 * Predictor.cpp
 *
 *  Created on: 02-Nov-2019
 *      Author: pratik
 */

#include "Predictor.h"
#include "defination.h"
#include "BranchBuffer.h"

//Interface class
Predictor* Predictor::get_predictor(char *predictor_type)
{
    if (_instance == NULL)
    {
        if (strcmp(predictor_type, "bimodal") == 0)
             (_instance = new Bimodal);
        else if (strcmp(predictor_type, "gshare") == 0)
             (_instance = new Gshare);
        else
             (_instance = new Hybrid);
    }

    return _instance;
}

//Functions for Bimodal predictor
Bimodal::Bimodal()
{
    bimodalTableSize = 0;
    bimodalTable = NULL;
    indexMask = 0;
    totalPredictions = 0;
    missPredicts = 0;
    missRate = 0;
}

Bimodal::~Bimodal()
{
    delete[] bimodalTable;
#if DEBUG
    cout << "Bimodal object destoryed" << endl;
#endif
}

void Bimodal::setBimodalTable(unsigned int bits)
{
    bimodalTableSize = pow(2, bits);
    indexMask = bimodalTableSize - 1;
    bimodalTable = new int[bimodalTableSize];
    for (unsigned int i = 0; i < bimodalTableSize; i++)
    {
        bimodalTable[i] = 3;
    }
}

char Bimodal::prediction(unsigned int addr, char outCome, bool isValid)
{
    char predictVal;
    unsigned int indexVal = (addr >> 2) & indexMask;
    totalPredictions++;
#if DEBUG
    if (isValid)
    {
        cout << "PC: " << hex << addr << " " << outCome << endl;
        cout << "BIMODAL index: " << dec << indexVal << " old value: "
                << bimodalTable[indexVal] << " new value ";
    }
#endif
    bimodalTable[indexVal] >= 2 ? predictVal = 't' : predictVal = 'n';
    if (isValid)
    {
        if (outCome == 't' && bimodalTable[indexVal] < 3)
            bimodalTable[indexVal]++;
        else if (outCome == 'n' && bimodalTable[indexVal] > 0)
            bimodalTable[indexVal]--;
#if DEBUG
        if (isValid)
            cout << bimodalTable[indexVal] << endl;
#endif
        if (predictVal != outCome)
            missPredicts++;
    }
    return predictVal;
}

void Bimodal::printResult(int isValid)
{
    // 0-> call from hybrid no cout
    // 1-> only bimodal mode
    // 2-> buffer is present
    if (isValid == 1)
    {
        cout << " number of predictions: " << dec << totalPredictions << endl;
        cout << " number of mispredictions: " << dec << missPredicts << endl;
        float num = missPredicts;
        float dem = totalPredictions;
        missRate = (num / dem);
        cout << " misprediction rate: ";
        printf("%.2f%%\n", missRate * 100);
    }
    else if (isValid == 2)
    {
        cout << endl;
    }
    cout << "FINAL BIMODAL CONTENTS" << endl;
    for (unsigned int i = 0; i < bimodalTableSize; i++)
    {
        cout << dec << i << " " << bimodalTable[i] << endl;
    }
}

unsigned int Bimodal::getMissPredict()
{
    return missPredicts;
}
//Functions for gshare predictor
Gshare::Gshare()
{
    totalPredictions = 0;
    missPredicts = 0;
    missRate = 0;
    pcBitMask = 0;
    pcBits = 0;
    gshareBits = 0;
    gshareBitsMask = 0;
    gshareTableSize = 0;
    gshareTable = NULL;
    gshareGlobalRegister = 0;
    mBitnBitMaskVal = 0;
}

Gshare::~Gshare()
{
    delete[] gshareTable;
#if DEBUG
    cout << "object destroyed" << endl;
#endif
}

void Gshare::setGshareTable(unsigned int pc_bits, unsigned int counterBits)
{
    pcBits = pc_bits;
    gshareBits = counterBits;
    gshareTableSize = pow(2, pcBits);
    gshareBitsMask = gshareTableSize - 1;
    pcBitMask = pow(2, pcBits) - 1;
    gshareTable = new int[gshareTableSize];
    mBitnBitMaskVal = pow(2, (pcBits - gshareBits)) - 1;
    //set initial counter value to 2
    for (unsigned int i = 0; i < gshareTableSize; i++)
    {
        gshareTable[i] = 2;
    }
}

char Gshare::prediction(unsigned int addr, char outCome, bool isValid)
{
    char predictVal;
    totalPredictions++;
    //PC masking
    unsigned int pcMaskedVal = (addr >> 2) & pcBitMask;

    //gshare global history register masking with n bits
    unsigned int globalMaskedVal = gshareGlobalRegister & gshareBitsMask;
    //unsigned int globalTemp = gshareGlobalRegister;
    //refer to specification pdf for clear idea
    // n bits of PC xor with n bits from global history register
    unsigned int xorResult = (pcMaskedVal >> (pcBits - gshareBits))
            ^ (globalMaskedVal);
    //unsigned int xorResult = pcMaskedVal ^ (globalTemp << (pcBits - gshareBits));

    //Concatenate XOR result with PC
    unsigned int indexVal = (xorResult << (pcBits - gshareBits))
            | (pcMaskedVal & mBitnBitMaskVal);

#if DEBUG
    if (isValid)
    {
        // cout << "PC: " << hex << addr << " " << outCome << endl;
        cout << "GSHARE index: " << dec << indexVal << " old value: "
                << gshareTable[indexVal] << " new value ";
    }
#endif
    gshareTable[indexVal] >= 2 ? predictVal = 't' : predictVal = 'n';

    if (isValid)
    {
        if (outCome == 't' && gshareTable[indexVal] < 3)
            gshareTable[indexVal]++;
        else if (outCome == 'n' && gshareTable[indexVal] > 0)
            gshareTable[indexVal]--;
#if DEBUG
        if (isValid)
            cout << gshareTable[indexVal] << endl;
#endif
        if (predictVal != outCome)
            missPredicts++;
    }

    //update global register
    if (outCome == 't')
    {
        gshareGlobalRegister = gshareGlobalRegister >> 1;
        if (gshareBits)
            gshareGlobalRegister |= (1 << (gshareBits - 1));
    }
    else
    {
        gshareGlobalRegister = gshareGlobalRegister >> 1;
    }
#if DEBUG
    cout << "BHR UPDATED:" << dec << gshareGlobalRegister << endl;
#endif
    return predictVal;
}

void Gshare::printResult(int isValid)
{
    // 0-> call from hybrid no cout just table
    // 1-> only gshare mode
    // 2-> buffer is present
    if (isValid == 1)
    {
        cout << " number of predictions: " << totalPredictions << endl;
        cout << " number of mispredictions: " << missPredicts << endl;
        float num = missPredicts;
        float dem = totalPredictions;
        missRate = (num / dem);
        cout << " misprediction rate: ";
        printf("%.2f%%\n", missRate * 100);
    }
    else if (isValid == 2)
    {
        cout << endl;
    }
    cout << "FINAL GSHARE CONTENTS" << endl;
    for (unsigned int i = 0; i < gshareTableSize; i++)
    {
        cout << dec << i << " " << gshareTable[i] << endl;
    }
}

unsigned int Gshare::getMissPredict()
{
    return missPredicts;
}

//Functions for hybrid predictor
Hybrid::Hybrid()
{
    totalPredictions = 0;
    missPredicts = 0;
    counterBits = 0;
    gsharePCBits = 0;
    gshareGlobalRegisterBits = 0;
    bimodalPCBits = 0;
    hybridCounterTableSize = 0;
    hybridCounterTable = NULL;
    indexMask = 0;
    missRate = 0;
}

Hybrid::~Hybrid()
{
    delete[] hybridCounterTable;
#if DEBUG
    cout << "object destroyed" << endl;
#endif
}

void Hybrid::setHybridParams(unsigned int kbitCounter,
        unsigned int gsharePcBits, unsigned int gShareCounterBits,
        unsigned int bimodalPCbits)
{
    gshareObj.setGshareTable(gsharePcBits, gShareCounterBits);
    bimodalObj.setBimodalTable(bimodalPCbits);
    counterBits = kbitCounter;
    hybridCounterTableSize = pow(2, counterBits);
    indexMask = hybridCounterTableSize - 1;
    hybridCounterTable = new int[hybridCounterTableSize];
    for (unsigned int i = 0; i < hybridCounterTableSize; i++)
    {
        hybridCounterTable[i] = 1;
    }
}

char Hybrid::prediction(unsigned int addr, char outCome, bool isValid)
{
    char predictVal;
    char bimodalPredictVal, gsharePredictVal;

    unsigned int index = (addr >> 2) & indexMask;
#if DEBUG
    cout << "PC: " << hex << addr << " " << outCome << endl;
    cout << "CHOOSER index: " << dec << index << endl;

#endif
    if (hybridCounterTable[index] >= 2)
    {
        bimodalPredictVal = bimodalObj.prediction(addr, outCome, false);
        gsharePredictVal = gshareObj.prediction(addr, outCome, true);
        predictVal = gsharePredictVal;
    }
    else
    {
        bimodalPredictVal = bimodalObj.prediction(addr, outCome, true);
        gsharePredictVal = gshareObj.prediction(addr, outCome, false);
        predictVal = bimodalPredictVal;
    }
#if DEBUG
    if (isValid)
    {
        //cout << "Actual prediction " << outCome << "  bimodal " << bimodalPredictVal
        //<< "  gshare  " << gsharePredictVal << endl;
        cout << "CHOOSER: " << " old value: " << hybridCounterTable[index]
                << " new value ";
    }
#endif
    if ((gsharePredictVal == outCome) && (bimodalPredictVal != outCome))
    {
        if (hybridCounterTable[index] < 3)
            hybridCounterTable[index]++;
    }
    else if ((gsharePredictVal != outCome) && (bimodalPredictVal == outCome))
    {
        if (hybridCounterTable[index] > 0)
            hybridCounterTable[index]--;
    }
#if DEBUG
    if (isValid)
        cout << hybridCounterTable[index] << endl;
#endif
    //Update the total number of predictions
    totalPredictions++;

    //Update miss - predicts
    if (predictVal != outCome)
        missPredicts++;
    return predictVal;
}

void Hybrid::printResult(int isValid)
{
    // 1-> only hybrid mode
    // 2-> buffer is present
    if (isValid == 1)
    {
        cout << " number of predictions: " << totalPredictions << endl;
        cout << " number of mispredictions: " << missPredicts << endl;
        float num = missPredicts;
        float dem = totalPredictions;
        missRate = (num / dem);
        cout << " misprediction rate: ";
        printf("%.2f%%\n", missRate * 100);
    }
    else if (isValid == 2)
    {
        cout << endl;
    }
    cout << "FINAL CHOOSER CONTENTS" << endl;
    for (unsigned int i = 0; i < hybridCounterTableSize; i++)
    {
        cout << i << " " << hybridCounterTable[i] << endl;
    }
    gshareObj.printResult(0);
    bimodalObj.printResult(0);
}

unsigned int Hybrid::getMissPredict()
{
    return missPredicts;
}
