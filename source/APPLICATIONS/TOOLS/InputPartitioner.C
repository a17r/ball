// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/featureSelection.h>
#include <BALL/QSAR/configIO.h>
#include <fstream>
#include "version.h"

using namespace BALL::QSAR;
using namespace BALL;
using namespace std;

void startInputPartitioning(InputPartitioningConfiguration& conf, QSARData* q, String* data_filename);


void startInputPartitioning(ifstream& in, QSARData* q, String* data_filename)
{
	InputPartitioningConfiguration conf = ConfigIO::readInputPartitioningConfiguration(&in);
	if(conf.done) return; // stop processing this section

	startInputPartitioning(conf,q,data_filename);
}


void startInputPartitioning(InputPartitioningConfiguration& conf, QSARData* q, String* data_filename)
{
	bool created_data_object=0;
	if(q==NULL || data_filename==NULL || conf.input_file!=*data_filename)
	{
		if(q==NULL)
		{
			q = new QSARData;
			created_data_object=1;
		}
		q->readFromFile(conf.input_file);
		if(data_filename) *data_filename = conf.input_file;
	}

	// -- create prefix for output-files ---
	String output_prefix = conf.input_file;
	size_t index = output_prefix.find_first_of(".");
	if(index != string::npos)
	{
		output_prefix=output_prefix.substr(0,index);
	}
	if(conf.ID>0)
	{
		output_prefix+="_"+String(conf.ID);
	}
	// -----------

	for(int fold_id=0; fold_id<conf.no_folds; fold_id++)
	{
		vector<QSARData*> sets;
		if(conf.even_split)
		{
			sets = q->evenSplit(conf.no_folds,fold_id);
		}
		else
		{
			sets = q->generateExternalSet(conf.validation_fraction); // length==2
		}

		if(q->isDataCentered())
		{
			bool center_y = q->isResponseCentered();
			sets[0]->centerData(center_y); // train-partition
			sets[1]->centerData(center_y); // test-partition
		}

		String train_name = output_prefix+"_TRAIN"+String(fold_id)+".dat";
		String test_name = output_prefix+"_TEST"+String(fold_id)+".dat";
		sets[0]->saveToFile(train_name);
		sets[1]->saveToFile(test_name);
	}

	if(created_data_object) delete q;
}


#ifndef EXT_MAIN
int main(int argc, char* argv[])
{
	CommandlineParser par("InputPartitioner","split QSAR data set     ", VERSION, String(__DATE__), "QuEasy (QSAR)");
	par.registerMandatoryInputFile("i", "input data-file");
	par.registerMandatoryOutputFile("o", "output dat-file");
	par.registerMandatoryIntegerParameter("n", "number of partitions");

	String man = "InputPartitioner partitions a given QSAR data set into n partitions with evenly distributed response values.\nThus, this tool can be useful as part of a nested validation pipeline.\nInput is a data file as generated by InputReader.\nOutput will be written to n files postfixed '_TRAIN<i>.dat' and '_TEST<i>.dat', where <i> is the ID of the resp. partition. For each of these partitions, the training set contains only those compounds that were not selected for the resp. test set.";
	par.setToolManual(man);
	par.setSupportedFormats("i","dat");
	par.setSupportedFormats("o","dat");
	par.parse(argc,argv);

	InputPartitioningConfiguration conf;
	conf.input_file = par.get("i");
	conf.no_folds = par.get("n").toInt();

	startInputPartitioning(conf,0,0);
}
#endif
