/* pipeline.h
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PIPELINE_H 
#define PIPELINE_H 

#include <list>
#include <map>
#include <iterator>


typedef unsigned int uint;

namespace BALL
{
	namespace VIEW
	{
		template<typename X> class Pipeline
		{
			public:	
							
				typedef typename std::list<X>::iterator iterator;
				
				iterator begin()
				{
					return list_.begin();
				};
				
				iterator end()
				{
					return list_.end();
				};
				
				void insert(X object)
				{
					list_.push_back(object);
					iterator it = list_.end();
					it--; // iterator to the last cell of the list
					map_.insert(make_pair(object,it));
				};
				
				/** insert 'object' before 'it' */
				void insert(iterator it, X object)
				{
					iterator ins_it = list_.insert(it,object);
					map_.insert(make_pair(object,ins_it));
				};
				
				/** insert 'object' after the second object */
				void insertAfter(X object, X before_this_object)
				{
					iterator it = find(before_this_object);
					if(it!=list_.end()) it++;
					iterator ins_it = list_.insert(it,object);
					map_.insert(make_pair(object,ins_it));
				};
				
				iterator erase(X object)
				{
					map_iterator map_it = map_.find(object);
					if(map_it!=map_.end())
					{
						iterator list_it = map_it->second;
						map_.erase(map_it);
						return list_.erase(list_it);  // returns iterator to next field
					}
					return list_.end();
				};
				
				iterator erase(iterator it)
				{
					return erase(*it);
// 					iterator list_it = it->second;
// 					map_.erase(it);
// 					return list_.erase(list_it);
				};
				
				bool contains(X object)
				{
					if(map_.find(object)==map_.end())
					{
						return false;
					}
					return true;
				};
				
				
				iterator find(X object)
				{
					map_iterator map_it = map_.find(object);
					if(map_it==map_.end())
					{
						return list_.end();
					}
					return map_it->second;	
				};
				
				uint size()
				{
					return list_.size();
				};
				
				bool empty()
				{
					if(list_.size()==0) return true;
					return false;
				};
				
				void clear()
				{
					map_.clear();
					list_.clear();
				};
		
			
			private:
				/** all objects in the exact order that they were inserted into this Pipeline */
				std::list<X> list_;
				
				/** map holding all inserted object in order to enable fast searching */
				std::map<X,iterator> map_;
						
				typedef typename std::map<X,iterator>::iterator map_iterator;
		};
	}
}


#endif // PIPELINE_H 