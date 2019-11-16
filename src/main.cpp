//============================================================================
// Name        : BranchPredictor.cpp
// Author      : Pratik Suresh Raut
// Version     :
// Copyright   : No issues
// Description : Branch Predictor, Ansi-style
//============================================================================

#include "defination.h"


int main(int argc, char *argv[])
{
    //Local variable declaration
    FILE *pFileHandler;                 // File handler for input trace file
    char *trace_file;                   // to store file name from command line
    char outCome;                       // t or n outcome
    unsigned int address;               // instruction
    unsigned int operationCount = 0;
    string output;
    unsigned int predictorMiss = 0;
    //Branch buffer
    BranchBuffer *bufferObj = NULL;

    //Flag for BTB
    bool isBuffer = false;
    bool isPresent;
    //Expected input pattern
    /*
     * For Bimodal predictor
     * sim bimodal <M2> <BTB size> <BTB assoc> <tracefile>
     */
    /*
     * For Gshare predictor
     * sim gshare <M1> <N> <BTB size> <BTB assoc> <tracefile>
     */
    /*
     * For hybrid predictor
     * sim hybrid <K> <M1> <N> <M2> <BTB size> <BTB assoc> <tracefile>
     * where K is the number of PC bits used to index the chooser
     * table, M1 and N are the number of PC bits and global branch history register bits used to index
     * the gshare table (respectively), and M2 is the number of PC bits used to index the bimodal table
     */
    if (!(argc == 6 || argc == 7 || argc == 9)) // Checks if correct number of inputs have been given.
    {
        // Throw error and exit if wrong
        cerr << "Error: Given inputs:" << argc - 1 << endl;
        return 1;
    }

    //command line arguments
    char *predictorTypeInput = argv[1];

    //Predictor class pointer
    Predictor *predictorObj = NULL;

    //get predictor object
    predictorObj = Predictor::get_predictor(predictorTypeInput);
    char fileName[10] = "gcc";
    trace_file = fileName;
    if (strcmp(predictorTypeInput, "bimodal") == 0)
    {
        static_cast<Bimodal*>(predictorObj)->setBimodalTable(atoi(argv[2]));

        if ((atoi(argv[3]) != 0) && (atoi(argv[4]) != 0))
        {
            bufferObj = new BranchBuffer(atoi(argv[3]), atoi(argv[4]));
            isBuffer = true;
        }
        trace_file = argv[5];
        output = argv[0];
        output = output + ' ' + argv[1] + ' ' + argv[2];
        output = output + ' ' + argv[3] + ' ' + argv[4] + ' ' + argv[5];
    }
    else if (strcmp(predictorTypeInput, "gshare") == 0)
    {
        static_cast<Gshare*>(predictorObj)->setGshareTable(atoi(argv[2]),
                atoi(argv[3]));
        if ((atoi(argv[4]) != 0) && (atoi(argv[5]) != 0))
        {
            bufferObj = new BranchBuffer(atoi(argv[4]), atoi(argv[5]));
            isBuffer = true;
        }
        trace_file = argv[6];
        output = argv[0];
        output = output + ' ' + argv[1] + ' ' + argv[2];
        output = output + ' ' + argv[3] + ' ' + argv[4] + ' ' + argv[5] + ' '
                + argv[6];
    }
    else if (strcmp(predictorTypeInput, "hybrid") == 0)
    {
        trace_file = argv[8];
        static_cast<Hybrid*>(predictorObj)->setHybridParams(atoi(argv[2]),
                atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        if ((atoi(argv[6]) != 0) && (atoi(argv[7]) != 0))
        {
            bufferObj = new BranchBuffer(atoi(argv[6]), atoi(argv[7]));
            isBuffer = true;
        }
        output = argv[0];
        output = output + ' ' + argv[1] + ' ' + argv[2];
        output = output + ' ' + argv[3] + ' ' + argv[4] + ' ' + argv[5] + ' '
                + argv[6];
        output = output + ' ' + argv[7] + ' ' + argv[8];
    }

    //open the trace file
    pFileHandler = fopen(trace_file, "r");

    //If FP == NULL then there is issue will opening trace_file
    if (pFileHandler == NULL)
    {
        // Throw error and exit if fopen() failed
        cout << "Error: Unable to open file " << trace_file << endl;
        return 1;
    }

    //read trace file
    while ((fscanf(pFileHandler, "%x %c", &address, &outCome)) != EOF)
    {
        operationCount++;
#if DEBUG
        cout << dec << operationCount << ". ";
#endif
        if (isBuffer)
        {
#if DEBUG
            cout << hex << "PC: " << address << " " << outCome << endl;
#endif
            isPresent = bufferObj->bufferPredict(address, outCome);
            if (isPresent)
            {
#if DEBUG
                cout << "BTB HIT" << endl;
#endif
                predictorObj->prediction(address, outCome, true);
            }
        }
        else
        {
            predictorObj->prediction(address, outCome, true);
        }
    }

    //close file
    fclose(pFileHandler);

#if !DEBUG
    //print command
    cout << "COMMAND" << endl;
    cout << output << endl;
    cout << "OUTPUT" << endl;
    if (isBuffer)
    {
        //print buffer content
        predictorMiss = predictorObj->getMissPredict();
        bufferObj->printBufferContet(predictorMiss);
    }
    //print results for a predictor
    if (isBuffer)
        predictorObj->printResult(2);
    else
        predictorObj->printResult(1);
#endif

    if (strcmp(predictorTypeInput, "bimodal") == 0)
    {
        static_cast<Bimodal*>(predictorObj)->~Bimodal();
    }
    else if (strcmp(predictorTypeInput, "gshare") == 0)
    {
        static_cast<Gshare*>(predictorObj)->~Gshare();
    }
    else if (strcmp(predictorTypeInput, "hybrid") == 0)
    {
        static_cast<Hybrid*>(predictorObj)->~Hybrid();
    }

    if (isBuffer)
        delete bufferObj;

    return 0;
}
