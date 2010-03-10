/*--------------------===FileString===--------------------------
 *
 * Provides a wraper around strings which checks for their applicability as paths 
 * and throws an exception if an operation renders a string inappropriate as a path.
 * Currently only checks for size() < MAX_PATH, but could be expanded to include a few
 * more things (maybe).
 *
 * Useful in interim before this is implemented using Allegro's file system 
 * (if that turns out to be any good).
 *
 * WARNING -- ONLY TESTED FOR A FEW SPECIFIC CASES (operater=() for example),
 * IF YOU WANT TO USE OTHERS FUNCTIONS IN OTHER WAYS DON'T BE SUPRISED,
 * CHECK FOR TYPOS, and COMPARE WITH WORKING CASES - IF YOU GET ERRORS.
 */
#include "filestring.h" // class's header file

void FileString::TestPathValidity()
{
    if (std::string::size() >= MAX_PATH) // >= as MAX_PATH includes terminating character not in strings
    {
        throw MaxPathLengthExceededException();
    }
}

FileString::FileString()
{
    
}
FileString::FileString ( const std::string& str ):
     std::string(str)
{
     TestPathValidity();
}
FileString::FileString ( const std::string& str, size_t pos, size_t n):
     std::string(str,pos,n)
{
    TestPathValidity();
}
FileString::FileString ( const char * s, size_t n ):
    std::string(s,n)
{
    TestPathValidity();
}
FileString::FileString ( const char * s ):
    std::string(s)
{
    TestPathValidity();
}
FileString::FileString ( size_t n, char c ):
    std::string(n,c)
{
    TestPathValidity();
}
template<class InputIterator> FileString::FileString (InputIterator begin, InputIterator end):
    std::string(begin,end)
{
    TestPathValidity();
}

FileString::~FileString()
{

}

std::string& FileString::operator=(std::string str)
{
    std::string::operator=(str);
    TestPathValidity();
    return(std::string::operator=(str));
}
std::string& FileString::operator= ( const char* s )
{
    std::string::operator=(s);
    TestPathValidity();
    return(std::string::operator=(s));
}
//very probably never required; but maybe, just maybe,
//there will be some filesystem for which MAX_PATH is 2.
std::string& FileString::operator= ( char c )
{
    std::string::operator=(c);
    TestPathValidity();
    return(std::string::operator=(c)); 
}

void FileString::resize ( size_t n, char c )
{
     std::string::resize(n,c);
     TestPathValidity();
     return;
}
void FileString::resize ( size_t n )
{
     std::string::resize(n);
     TestPathValidity();
     return;
}

std::string& FileString::operator+= ( const std::string& str )
{
    std::string::operator+=(str);
    TestPathValidity();
    return(std::string::operator=(str));
}
std::string& FileString::operator+= ( const char* s )
{
    std::string::operator+=(s);
    TestPathValidity();
    return(std::string::operator=(s));
}
std::string& FileString::operator+= ( char c )
{
    std::string::operator+=(c);
    TestPathValidity();
    return(std::string::operator=(c)); 
}

std::string& FileString::append ( const std::string& str )
{
    std::string::append(str);
    TestPathValidity();
    return(std::string::append(str));
}
std::string& FileString::append ( const std::string& str, size_t pos, size_t n )
{
    std::string::append(str,pos,n);
    TestPathValidity();
    return(std::string::append(str,pos,n));     
}
std::string& FileString::append ( const char* s, size_t n )
{
    std::string::append(s,n);
    TestPathValidity();
    return(std::string::append(s,n));
}
std::string& FileString::append ( const char* s )
{
    std::string::append(s);
    TestPathValidity();
    return(std::string::append(s));
}
std::string& FileString::append ( size_t n, char c )
{
    std::string::append(n,c);
    TestPathValidity();
    return(std::string::append(n,c));
}
template <class InputIterator>
    std::string& FileString::append ( InputIterator first, InputIterator last )
{
    std::string::append(first,last);
    TestPathValidity();
    return(std::string::append(first,last));
}

void FileString::push_back ( char c )
{
     std::string::push_back(c);
     TestPathValidity();
     return;
}

std::string& FileString::assign ( const std::string& str )
{
    std::string::assign(str);
    TestPathValidity();
    return(std::string::assign(str));
}
std::string& FileString::assign ( const std::string& str, size_t pos, size_t n )
{
    std::string::assign(str,pos,n);
    TestPathValidity();
    return(std::string::assign(str,pos,n));
}
std::string& FileString::assign ( const char* s, size_t n )
{
    std::string::assign(s,n);
    TestPathValidity();
    return(std::string::assign(s,n));
}
std::string& FileString::assign ( const char* s )
{
    std::string::assign(s);
    TestPathValidity();
    return(std::string::assign(s));
}
std::string& FileString::assign ( size_t n, char c )
{
    std::string::assign(n,c);
    TestPathValidity();
    return(std::string::assign(n,c));
}
template <class InputIterator>
    std::string& FileString::assign ( InputIterator first, InputIterator last )
{
    std::string::assign(first,last);
    TestPathValidity();
    return(std::string::assign(first,last));
}

std::string& FileString::insert ( size_t pos1, const std::string& str )
{
    std::string::insert(pos1,str);
    TestPathValidity();
    return(std::string::insert(pos1,str));
}
std::string& FileString::insert ( size_t pos1, const std::string& str, size_t pos2, size_t n )
{
    std::string::insert(pos1,str,pos2,n);
    TestPathValidity();
    return(std::string::insert(pos1,str,pos2,n));
}
std::string& FileString::insert ( size_t pos1, const char* s, size_t n)
{
    std::string::insert(pos1,s,n);
    TestPathValidity();
    return(std::string::insert(pos1,s,n));
}
std::string& FileString::insert ( size_t pos1, const char* s )
{
    std::string::insert(pos1,s);
    TestPathValidity();
    return(std::string::insert(pos1,s));
}
std::string& FileString::insert ( size_t pos1, size_t n, char c )
{
    std::string::insert(pos1,n,c);
    TestPathValidity();
    return(std::string::insert(pos1,n,c));
}
std::string::iterator FileString::insert ( std::string::iterator p, char c )
{
    std::string::insert(p,c);
    TestPathValidity();
    return(std::string::insert(p,c));
}
void FileString::insert ( std::string::iterator p, size_t n, char c )
{
    std::string::insert(p,n,c);
    TestPathValidity();
    return(std::string::insert(p,n,c));
}
template<class InputIterator>
    void FileString::insert ( std::string::iterator p, InputIterator first, InputIterator last )
{
    std::string::insert(p,first,last);
    TestPathValidity();
    return(std::string::insert(p,first,last));
}

std::string& FileString::replace ( size_t pos1, size_t n1,   const std::string& str )
{
    std::string::replace(pos1,n1,str);
    TestPathValidity();
    return(std::string::replace(pos1,n1,str));
}
std::string& FileString::replace ( std::string::iterator i1, iterator i2, const std::string& str )
{
    std::string::replace(i1,i2,str);
    TestPathValidity();
    return(std::string::replace(i1,i1,str));
}
std::string& FileString::replace ( size_t pos1, size_t n1, const std::string& str, size_t pos2, size_t n2 )
{
    std::string::replace(pos1,n1,str,pos2,n2);
    TestPathValidity();
    return(std::string::replace(pos1,n1,str,pos2,n2));
}
std::string& FileString::replace ( size_t pos1, size_t n1, const char* s, size_t n2 )
{
    std::string::replace(pos1,n1,s,n2);
    TestPathValidity();
    return(std::string::replace(pos1,n1,s,n2));
}
std::string& FileString::replace ( std::string::iterator i1, std::string::iterator i2, const char* s, size_t n2 )
{
    std::string::replace(i1,i2,s,n2);
    TestPathValidity();
    return(std::string::replace(i1,i2,s,n2));
}
std::string& FileString::replace ( size_t pos1, size_t n1,   const char* s )
{
    std::string::replace(pos1,n1,s);
    TestPathValidity();
    return(std::string::replace(pos1,n1,s));
}
std::string& FileString::replace ( std::string::iterator i1, std::string::iterator i2, const char* s )
{
    std::string::replace(i1,i2,s);
    TestPathValidity();
    return(std::string::replace(i1,i2,s));
}
std::string& FileString::replace ( size_t pos1, size_t n1,   size_t n2, char c )
{
    std::string::replace(pos1,n1,n2,c);
    TestPathValidity();
    return(std::string::replace(pos1,n1,n2,c));
}
std::string& FileString::replace ( std::string::iterator i1, std::string::iterator i2, size_t n2, char c )
{
    std::string::replace(i1,i2,n2,c);
    TestPathValidity();
    return(std::string::replace(i1,i2,n2,c));
}
template<class InputIterator>
    std::string& FileString::replace ( std::string::iterator i1, std::string::iterator i2, InputIterator j1, InputIterator j2 )
{
    std::string::replace(i1,i2,j1,j2);
    TestPathValidity();
    return(std::string::replace(i1,i2,j1,j2));
}


//two swaps, a safe, when swapping with a FileString, and an unsafe, when swapping with a String
//I don't know if either works... esp the safe one.
void FileString::swap ( std::string& str )
{
    std::string::swap(str);
    TestPathValidity();
    return;
}

void FileString::swap ( FileString& fstr )
{
     std::string::swap(fstr);
     return;
}
