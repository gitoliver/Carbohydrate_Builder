#include "carbohydratebuilder.h"
#include <QApplication>
#include "/home/oliver/Programs/gems/gmml/includes/gmml.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace MolecularModeling;
using namespace CondensedSequenceSpace;
typedef std::vector<Atom*> AtomVector;
typedef std::vector<GeometryTopology::Coordinate*> CoordinateVector;
typedef std::vector<Residue*> ResidueVector;
typedef std::vector<Assembly*> AssemblyVector;
typedef GeometryTopology::Coordinate Vector;
typedef std::vector<std::pair<std::string, std::string> > stringPairVector;
typedef std::map<int, std::string> IndexNameMap;
typedef std::pair<std::string, RotamersAndGlycosidicAnglesInfo*> RotamerNameInfoPair;
typedef std::vector<RotamerNameInfoPair> CondensedSequenceRotamersAndGlycosidicAnglesInfo;

//Splitting a string by delimiter
template<typename Out>
void split(const std::string &s, char delim, Out result);
std::vector<std::string> split(const std::string &s, char delim);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CarbohydrateBuilder w;
    w.show();

    //************************************************//
    // Read the list of glycans from the input file   //
    //************************************************//

    stringPairVector pairList_ID_Sequence;

    //std::char delim = argv[1]; // test later
    char delim = ',';
    std::ifstream inf ("/home/oliver/Programs/Cplusplus/CarbohydrateBuilder/CarbohydrateBuilder/inputs/CarbBuilderInput.txt");
    if (!inf)
    {
        std::cerr << "Problem opening input file; check path and filename" << std::endl;
        std::exit(1);
    }
    bool split_switch = true;
    std::string buffer;
    while (inf) // While there's still stuff left to read from the file
    {
        std::string strInput;
        while (getline(inf, strInput)) // Read file line by line
        {
            std::stringstream ss;
            ss.str(strInput); // Put line into a stringstream
            std::string line_fragment;
            while (std::getline(ss, line_fragment, delim)) // split it using a delimiter
            {
                if (split_switch)
                {
                    buffer = line_fragment; // Save the ID in buffer
                    split_switch = false;
                }
                else // This should be the sequence when split_switch is false.
                {
                    pairList_ID_Sequence.push_back(std::make_pair(buffer,line_fragment));
                    split_switch = true;
                }
            }
        }
    }

    for (stringPairVector::const_iterator pos = pairList_ID_Sequence.begin(); pos != pairList_ID_Sequence.end(); ++pos)
    {

       std::cout << pos->first << " " << pos->second << std::endl;

    }


    //std::cout << proteinPDB << ", " << glycanDirectory << std::endl;

    //************************************************//
    // Details for loading in a PDB file              //
    //************************************************//

    std::vector<std::string> amino_libs, glycam_libs, other_libs, prep;
    amino_libs.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/amino12.lib");
    amino_libs.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/aminoct12.lib");
    amino_libs.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/aminont12.lib");

    glycam_libs.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_amino_06j_12SB.lib");
    glycam_libs.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_aminoct_06j_12SB.lib");
    glycam_libs.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_aminont_06j_12SB.lib");

    other_libs.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/nucleic12.lib");
    other_libs.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/nucleic12.lib");
    other_libs.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/other/solvents.lib");

    prep.push_back("/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_06j-1.prep");

    std::string pdb_file_path = "/home/oliver/Programs/Cplusplus/CarbohydrateBuilder/CarbohydrateBuilder/outputs/test.pdb";
    std::string parameter_file_path = "/home/oliver/Programs/gems/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_06j.dat";
    std::string ion_parameter_file_path = "/home/oliver/Programs/gems/gmml/dat/CurrentParams/other/atomic_ions.lib";

    //************************************************//
    // Build PDB file from condensed sequence         //
    //************************************************//

    std::string sequence;
    std::string id;
    std::string outputFileName;

    for (stringPairVector::const_iterator it = pairList_ID_Sequence.begin(); it != pairList_ID_Sequence.end(); ++it)
    {
        id = it->first;
        sequence = it->second;

        outputFileName = "/home/oliver/Programs/Cplusplus/CarbohydrateBuilder/CarbohydrateBuilder/outputs/" + id + ".pdb";

        Assembly assembly;

        CondensedSequence *condensedSequence = new CondensedSequence(sequence);
        CondensedSequenceRotamersAndGlycosidicAnglesInfo rotamers_info = condensedSequence->GetCondensedSequenceRotamersAndGlycosidicAnglesInfo(condensedSequence->GetCondensedSequenceResidueTree());
        assembly.BuildAssemblyFromCondensedSequence(sequence, prep.at(0) ,parameter_file_path, true);
        IndexNameMap names;

        AssemblyVector structures = assembly.BuildAllRotamersFromCondensedSequence(sequence, prep.at(0), parameter_file_path, rotamers_info, names);
        std::cout << "We are here, check out " << names[0] << std::endl;

        int i = 0;
        for (AssemblyVector::iterator it = structures.begin(); it != structures.end(); ++it)
        {
           //(*it)->Print();
           Assembly *sub_Assembly = *it;
          // sub_Assembly->Print();
           PdbFileSpace::PdbFile *outputPdbFile = sub_Assembly->BuildPdbFileStructureFromAssembly();
           outputFileName = "/home/oliver/Programs/Cplusplus/CarbohydrateBuilder/CarbohydrateBuilder/outputs/" + id + "_" + names[i] + ".pdb";
           std::cout << "Attempting to write " << outputFileName << std::endl;
           outputPdbFile->Write(outputFileName);
           ++i;
        }



   // TopologyFileSpace::TopologyFile *outputTopologyFile1 = assembly.BuildTopologyFileStructureFromAssembly(parameter_file_path, ion_parameter_file_path);
   // outputTopologyFile1->Write("/home/oliver/Desktop/test-BuildFromSequence.parm7");

   // CoordinateFileSpace::CoordinateFile *outputCoordinateFile = assembly.BuildCoordinateFileStructureFromAssembly();
   // outputCoordinateFile->Write("/home/oliver/Desktop/test-BuildFromSequence.rst7");
    }


    return a.exec();
}


template<typename Out>

void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
