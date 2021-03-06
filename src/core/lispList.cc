/*
    File: lispList.cc
*/

/*
Copyright (c) 2014, Christian E. Schafmeister
 
CLASP is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.
 
See directory 'clasp/licenses' for full details.
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/* -^- */
#define	DEBUG_LEVEL_FULL

#include "core/common.h"
#include "core/environment.h"
#include "core/ql.h"
#include "lispList.h"
#include "symbolTable.h"
#include "core/wrappers.h"
namespace core
{

// ----------------------------------------------------------------------
//


#define ARGS_cl_nth "(idx arg)"
#define DECL_cl_nth ""
#define DOCS_cl_nth "See CLHS nth"
    T_sp cl_nth(int idx, T_sp arg)
    {_G();
	if (arg.nilp()) return _Nil<T_O>();
	if ( Cons_sp list = arg.asOrNull<Cons_O>() )
	{
	    return list->onth(idx);
	}
	TYPE_ERROR(arg,cl::_sym_list);
    };


#define ARGS_cl_nthcdr "(idx arg)"
#define DECL_cl_nthcdr ""
#define DOCS_cl_nthcdr "See CLHS nth"
    T_sp cl_nthcdr(int idx, T_sp arg)
    {_G();
	if (arg.nilp()) return arg;
	if ( Cons_sp list = arg.asOrNull<Cons_O>() )
	{
	    return list->onthcdr(idx);
	}
	TYPE_ERROR(arg,cl::_sym_list);
    };




    
    
#define ARGS_cl_copyList "(arg)"
#define DECL_cl_copyList ""
#define DOCS_cl_copyList "copyList"
    T_sp cl_copyList(T_sp arg)
    {_G();
	if ( arg.nilp() ) return arg;
	if ( Cons_sp l = arg.asOrNull<Cons_O>() )
	{
	    return l->copyList();
	}
	TYPE_ERROR(arg,cl::_sym_list);
    };


    
    
#define ARGS_af_butlast "(list &optional (n 1))"
#define DECL_af_butlast ""
#define DOCS_af_butlast "butlast"
    T_mv af_butlast(T_sp list, Integer_sp n)
    {_G();
	int ni = n->as_int();
	int keepi = cl_length(list)-ni;
	if ( keepi <= 0 ) return(Values(_Nil<Cons_O>()));
	ql::list res;
	Cons_sp cur = list.as_or_nil<Cons_O>();
	for ( int i=0; i<keepi; i++ )
	{
	    res << oCar(cur);
	    cur = cCdr(cur);
	}
	return(Values(res.cons()));
    };



#define ARGS_cl_nbutlast "(list &optional (n 1))"
#define DECL_cl_nbutlast ""
#define DOCS_cl_nbutlast "butlast"
    T_sp cl_nbutlast(T_sp list, Integer_sp n)
    {_G();
	int ni = n->as_int();
	int keepi = cl_length(list)-ni;
	if ( keepi <= 0 ) return (_Nil<T_O>());
	Cons_sp cur = list.as<Cons_O>();
	Cons_sp prev = _Nil<Cons_O>();;
	for ( int i=0; i<keepi; i++ )
	{
	    prev = cur;
	    cur = cCdr(cur);
	}
	prev->setCdr(_Nil<Cons_O>());
	return list;
    };


    
    
#define ARGS_af_list "(&rest objects)"
#define DECL_af_list ""
#define DOCS_af_list "See CLHS: list"
    T_sp af_list(T_sp objects)
    {_G();
	return objects;
    };




    
    

#define DOCS_af_listSTAR "list* see CLHS"
#define LOCK_af_listSTAR 0
#define ARGS_af_listSTAR "(&rest objects)"
#define DECL_af_listSTAR ""
    T_sp af_listSTAR(T_sp tobjects)
    {_G();
	T_sp objects = tobjects;
	if ( objects.nilp() ) return(Values(_Nil<Cons_O>()));
	if (cCdr(objects).nilp() ) return(oCar(objects));
	Cons_sp cur;
	ql::list result(_lisp);
	for ( ; oCdr(objects).notnilp(); objects=oCdr(objects))
	{
//	    printf("Adding %s\n", _rep_(oCar(objects)).c_str());
	    result << oCar(objects);
	}
//	printf("dotting %s\n", _rep_(objects).c_str());
	result.dot(oCar(objects));
	return result.cons();
    }


#define DOCS_cl_last "last - see CLHS"
#define LOCK_cl_last 1
#define ARGS_cl_last "(list &optional (on 1))"
#define DECL_cl_last ""    
    T_sp cl_last(T_sp list, int n)
    {_G();
	if ( list.nilp() ) return list;
	if ( n < 0 )
	{
	    CELL_ERROR(Fixnum_O::create(n));
	}
        if ( Cons_sp clist = list.asOrNull<Cons_O>() ) {
            return clist->last(n);
        }
        TYPE_ERROR(list,cl::_sym_list);
    };


/* Adapted from ECL list.d nconc function */

    
    
#define ARGS_cl_nconc "(&rest lists)"
#define DECL_cl_nconc ""
#define DOCS_cl_nconc "tnconc"
    T_sp cl_nconc(Cons_sp lists)
    {_G();
        T_sp head = _Nil<T_O>();
        T_sp tail = _Nil<T_O>();
        for ( Cons_sp cur = lists; cur.notnilp(); cur=cCdr(cur) ) {
            T_sp new_tail;
            T_sp other = oCar(cur);
            if (other.nilp()) {
                new_tail = tail;
            } else if (Cons_sp cother = other.asOrNull<Cons_O>()) {
                new_tail = cl_last(other,1);
            } else {
                if (cur->cdr().notnilp()) {
                    TYPE_ERROR_LIST(other);
                }
                new_tail = tail;
            }
            if (head.nilp()) {
                head = other;
            } else {
                tail.as<Cons_O>()->rplacd(other);
            }
            tail = new_tail;
        }
        return head;
    }

    T_sp clasp_nconc(T_sp l, T_sp y)
    {
        if (l.nilp()) {
            return y;
        }
        if ( Cons_sp conslist = l.asOrNull<Cons_O>()) {
            Cons_sp last = cl_last(conslist,1).as<Cons_O>();
            last->rplacd(y);
            return conslist;
        }
        TYPE_ERROR(l,cl::_sym_list);
    }






#define DOCS_af_revappend "revappend"
#define LOCK_af_revappend 1
#define ARGS_af_revappend "(list tail)"
#define DECL_af_revappend ""
    T_sp af_revappend(T_sp list, T_sp tail)
    {_G();
	if ( list.nilp() ) return(tail);
        if ( Cons_sp clist = list.asOrNull<Cons_O>() ) {
            return clist->revappend(tail);
        }
        TYPE_ERROR(list,cl::_sym_list);
    };



#define DOCS_cl_nreconc "nreconc"
#define LOCK_cl_nreconc 1
#define ARGS_cl_nreconc "(list tail)"
#define DECL_cl_nreconc ""
    T_sp cl_nreconc(T_sp list, T_sp tail)
    {_G();
	if ( list.nilp() ) return(tail);
        if ( Cons_sp clist = list.asOrNull<Cons_O>() ) {
            return clist->nreconc(tail);
        }
        TYPE_ERROR(list,cl::_sym_list);
    };



#if 0
    EXPOSE_CLASS(core,List_O);

    void List_O::exposeCando(Lisp_sp lisp)
    {_G();
	class_<List_O>()
	    //	    .def("revappend",&List_O::revappend)    I need to wrap this
	    //	    .def("nreconc",&List_O::nreconc)	I need to wrap this
	    //	.initArgs("(self)")
	    ;
    }
#endif

    void initialize_list()
    {
	SYMBOL_EXPORT_SC_(ClPkg,revappend);
 	Defun(revappend);
	SYMBOL_EXPORT_SC_(ClPkg,nreconc);
	ClDefun(nreconc);
	SYMBOL_EXPORT_SC_(ClPkg,list);
	Defun(list);
	SYMBOL_EXPORT_SC_(ClPkg,listSTAR);
	Defun(listSTAR);
	SYMBOL_EXPORT_SC_(ClPkg,butlast);
	Defun(butlast);
	SYMBOL_EXPORT_SC_(ClPkg,nbutlast);
	ClDefun(nbutlast);
	SYMBOL_EXPORT_SC_(ClPkg,nth);
	ClDefun(nth);
	SYMBOL_EXPORT_SC_(ClPkg,nthcdr);
	ClDefun(nthcdr);
	SYMBOL_EXPORT_SC_(ClPkg,copyList);
	ClDefun(copyList); 
	SYMBOL_EXPORT_SC_(ClPkg,last);
	ClDefun(last);
        ClDefun(nconc);
   }

#if 0
    void List_O::exposePython(::core::Lisp_sp lisp)
    {
#ifdef USEBOOSTPYTHON
	PYTHON_CLASS(Pkg(),List,"","",_LISP)
	    //	.initArgs("(self)")
	    ;
#endif
    }
#endif


#if 0
    void List_O::serialize(::serialize::SNodeP node)
    {
	IMPLEMENT_ME();
	this->Bases::serialize(node);
	// Archive other instance variables here
    }
#endif

#if 0
    void List_O::archiveBase(::core::ArchiveP node)
    {
	IMPLEMENT_ME();
	this->Base1::archiveBase(node);
	// Archive other instance variables here
    }
#endif


}; /* core */
