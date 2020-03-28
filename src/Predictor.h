/*
 * Predictor.h
 *
 *  Created on: 02-Nov-2019
 *      Author: pratik
 */

#ifndef PREDICTOR_H_
#define PREDICTOR_H_

class Predictor
{
public:
    // singleton method
    static Predictor *get_predictor(char *predictor_type);
    virtual char prediction(unsigned int addr, char outCome, bool isValid) = 0;
    virtual void printResult(int isValid) = 0;
    virtual unsigned int getMissPredict() = 0;
    virtual ~Predictor() {};
private:
    static Predictor* _instance;
};

class Bimodal: public Predictor
{
public:
    //functions
    Bimodal();
    virtual ~Bimodal();
    void setBimodalTable(unsigned int bits);
    void createBimodalTable();
    char prediction(unsigned int addr, char outCome, bool isValid);
    void printResult(int isValid);
    unsigned int getMissPredict();

private:
    //variable related to bimodal predictor
    unsigned int bimodalTableSize;
    int *bimodalTable;
    unsigned int indexMask;
    unsigned int totalPredictions;
    unsigned int missPredicts;
    float missRate;
};

class Gshare: public Predictor
{
public:
    //functions
    Gshare();
    virtual ~Gshare();
    char prediction(unsigned int addr, char outCome, bool isValid);
    void printResult(int isValid);
    void setGshareTable(unsigned int pc_bits, unsigned int counterBits);
    unsigned int getMissPredict();

private:
    //variable related to gshare predictor
    unsigned int gshareGlobalRegister;
    unsigned int gshareTableSize;
    int *gshareTable;
    unsigned int totalPredictions;
    unsigned int missPredicts;
    float missRate;
    unsigned int pcBits;
    unsigned int gshareBits;
    unsigned int pcBitMask;
    unsigned int gshareBitsMask;
    unsigned int mBitnBitMaskVal;
};

class Hybrid: public Predictor
{
public:
    //functions
    Hybrid();
    virtual ~Hybrid();
    char prediction(unsigned int addr, char outCome, bool isValid);
    void setHybridParams(unsigned int kbitCounter, unsigned int gsharePcBits, unsigned int gShareCounterBits,
            unsigned int bimodalPCbits);
    void printResult(int isValid);
    unsigned int getMissPredict();

private:
    //variable related to hybrid predictor
    Bimodal bimodalObj;
    Gshare gshareObj;
    unsigned int totalPredictions;
    unsigned int missPredicts;
    unsigned int counterBits;
    unsigned int gsharePCBits;
    unsigned int gshareGlobalRegisterBits;
    unsigned int bimodalPCBits;
    unsigned int hybridCounterTableSize;
    int *hybridCounterTable;
    unsigned int indexMask;
    float missRate;
};

#endif /* PREDICTOR_H_ */
