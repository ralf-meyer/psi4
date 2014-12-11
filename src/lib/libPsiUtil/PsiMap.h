/*
 *@BEGIN LICENSE
 *
 * PSI4: an ab initio quantum chemistry software package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *@END LICENSE
 */
#ifndef SRC_LIB_LIBPSIUTIL_PSIMAP_H_
#define SRC_LIB_LIBPSIUTIL_PSIMAP_H_

#include<map>
#include "exception.h"//For PSIEXCEPTION
namespace psi{

/** \brief The purpose of the PsiMap class is to provide an associative
 *         container, with some slight modifications
 *
 *
 *  For the most part the PsiMap class wraps the std::map class, one
 *  big difference is it allows for the elements to be accessed with
 *  const modifiers.  Unfortunately, due to the nuances of template
 *  inheritance the base class members need to be scoped (i.e. a call
 *  to the normal operator[], would be std::map<T1,T2>::operator[](i)).
 *  To avoid this I have wrapped the common calls.
 *
 *  \param[in] T1 The type of the key
 *  \param[in] T2 The type of the associated element
 */
template<typename T1, typename T2>
class PsiMap:public std::map<T1,T2>{
   private:
      ///The actual function that does the const deref
      const T2& MapDeRef(const T1& Key)const;

   public:
      ///Wrapper to the underlying copy constructor
      PsiMap<T1,T2>(const PsiMap<T1,T2>& other);

      ///Wrapper to default constructor
      PsiMap<T1,T2>(){}

      ///Our new function that can access our key const
      const T2& operator[](const T1& key)const;

      ///Wrapper to the original accessor
      T2& operator[](const T1& key);

      ///Wrapper to the number of times a key appears in the container
      int count(const T1& key)const;

      ///Wrapper to the size of the container
      int size()const;



};

/*******************Implementations are below*********************/
template<typename T1,typename T2>
T2& PsiMap<T1,T2>::operator[](const T1& key){
   return std::map<T1,T2>::operator[](key);
}

template<typename T1,typename T2>
int PsiMap<T1,T2>::count(const T1& key)const{
   return std::map<T1,T2>::count(key);
}

template<typename T1,typename T2>
int PsiMap<T1,T2>::size()const{
   return std::map<T1,T2>::size();
}

template<typename T1,typename T2>
PsiMap<T1,T2>::PsiMap<T1,T2>(const PsiMap<T1,T2>& other):
   std::map<T1,T2>(other){
}

template<typename T1,typename T2>
const T2& PsiMap<T1,T2>::operator[](const T1& key)const{
   return MapDeRef(key);
}

template<typename T1,typename T2>
const T2& PsiMap<T1,T2>::MapDeRef(const T1& Key)const{
   T2* returnvalue;
   if(this->count(Key)==1){//We know we aren't changing it
      typedef const std::map<T1,T2> cMap;
      cMap* temp=dynamic_cast<cMap*>(this);
      returnvalue=&(const_cast<std::map<T1,T2>* >(temp))->operator[](Key);
   }
   else //We are changing it, that's a no-no...
      throw PSIEXCEPTION("This Map doesn't contain an element:"+Key);
   return *returnvalue;
}
}//End psi namespace




#endif /* SRC_LIB_LIBPSIUTIL_PSIMAP_H_ */