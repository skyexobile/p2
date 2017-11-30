
// Combining the tuples only based on names results in ambiguity when the tuples share attributes with the same name. Rather than trying to resolve this by modifying the field_names, it makes sense to just pass both the tuples (implicitly disambiguated) into the evaluate function.
Let R be the smaller relation and S be the larger relation → getNumOfBlocks()
Read 9 blocks of R and 1 block of S  into memory → getBlocks()
For each block bR of R  // loop from 0 to numBlocks
	For each tuple tR in bR  // iterate over getTuples()
		For each tuple tS in bS  // getTuples()
			If evalBool(tS, tR)
				Combine tS and tR into tRes
Output tRes

#include "common.h"
#include "searchCondition.h"


