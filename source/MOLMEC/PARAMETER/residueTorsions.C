// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/MOLMEC/PARAMETER/residueTorsions.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	bool ResidueTorsions::Data::operator == (const Data& data) const
	{
		return  residue_name == data.residue_name && 
						atom_name_A == data.atom_name_A&& 
						atom_name_B == data.atom_name_B&& 
						atom_name_C == data.atom_name_C&& 
						atom_name_D == data.atom_name_D;
	}

	bool ResidueTorsions::Data::operator != (const Data& data) const
	{
		return !(*this == data);
	}

	ResidueTorsions::ResidueTorsions()
		:	ParameterSection()
	{
	}

	ResidueTorsions::~ResidueTorsions() 
	{
		clear();
	}

	void ResidueTorsions::clear() 
	{
		ParameterSection::clear();
	}

	bool ResidueTorsions::extractSection
		(Parameters& parameters, const String& section_name)
	{
		return ParameterSection::extractSection(parameters, section_name);
	}

	bool ResidueTorsions::extractSection
		(ForceFieldParameters& parameters, const String& section_name)
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// extract the basis information
		ParameterSection::extractSection(parameters, section_name);

		// iterate over all keys and construct the hash map of vectors
		for (Size i = 0; i < getNumberOfKeys(); i++)
		{
			String key = getKey(i);
			String residue = key.getField(0);
			
			// do we have an vector for this residue?
			if (!torsions_.has(residue))
			{	
				// create the vector
				torsions_.insert(pair<String, vector<Data> >(residue, vector<Data>()));
			} 

			// insert the torsions 
			torsions_[residue].push_back(Data(residue, key.getField(1), key.getField(2), key.getField(3), key.getField(4)));
			all_torsions_.insert(key);
		}

		return true;
	}

	Size ResidueTorsions::getNumberOfResidueTorsions(const String& residue_name) const
	{
		// if we know this residue...
		if (torsions_.has(residue_name))
		{
			// return the size of the corresponding array
			return (Size)torsions_[residue_name].size();
		}
		
		return 0;
	}


	bool ResidueTorsions::assignTorsion
		(const String& name, Position i, Data& torsion) const
	{
		// if we know this residue...
		if (torsions_.has(name))
		{
			// ...verify the index...
			Size max_index = (Size)torsions_[name].size();

			if (i < (Position)max_index)
			{
				// ...and assign the torsion structure
				torsion = torsions_[name][i];
				return true;
			}
		}		

		return false;
	}

	bool ResidueTorsions::hasTorsion
		(const String& residue, const String& atom_A, const String& atom_B,
		 const String& atom_C, const String& atom_D) const
	{
		String query = residue + " " + atom_A + " " 
			+ atom_B + " " + atom_C + " " + atom_D;
		
		// is this a known torsion?
		if (all_torsions_.has(query))
		{ 
			return true;
		}
		
		// reverse the order of the atoms and try again
		query = residue + " " + atom_D + " " 
			+ atom_C + " " + atom_B + " " + atom_A;
		
		return all_torsions_.has(query);
	}
 
	 
} // namespace BALL
