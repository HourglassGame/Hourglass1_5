#ifndef HOURGLASS_FILESTRING_H
#define HOURGLASS_FILESTRING_H
#include <string>
#include "Exceptions.h"

/*--------------------===FileString===--------------------------
 *
 * Provides a wraper around strings which checks for their applicability as paths 
 * and throws an exception if an operation renders a string inappropriate as a path.
 * Currently only checks for size() < MAX_PATH, but could be expanded to include a few
 * more things (maybe).
 *
 * Useful in interim before files are implemented using Allegro's file system 
 * (if that turns out to be any good).
 *
 * WARNING -- ONLY TESTED FOR A FEW SPECIFIC CASES (operater=() for example),
 * IF YOU WANT TO USE OTHERS FUNCTIONS IN OTHER WAYS DON'T BE SUPRISED,
 * CHECK FOR TYPOS, and COMPARE WITH WORKING CASES - IF YOU GET ERRORS.
 */

class FileString: public std::string
{
	public:
		// class constructor
		explicit FileString();
		FileString ( const std::string& str );
        FileString ( const std::string& str, size_t pos, size_t n = npos );
        FileString ( const char * s, size_t n );
        FileString ( const char * s );
        FileString ( size_t n, char c );
        template<class InputIterator> FileString (InputIterator begin, InputIterator end);
        
		// class destructor
		~FileString();
		std::string& operator= (std::string);
		std::string& operator= ( const char* s );
        std::string& operator= ( char c );
        
        void resize ( size_t n, char c );
        void resize ( size_t n );
        
        std::string& operator+= ( const std::string& str );
        std::string& operator+= ( const char* s );
        std::string& operator+= ( char c );

        std::string& append ( const std::string& str );
        std::string& append ( const std::string& str, size_t pos, size_t n );
        std::string& append ( const char* s, size_t n );
        std::string& append ( const char* s );
        std::string& append ( size_t n, char c );
        template <class InputIterator>
                 std::string& append ( InputIterator first, InputIterator last );
        
        void push_back ( char c );
        
        std::string& assign ( const std::string& str );
        std::string& assign ( const std::string& str, size_t pos, size_t n );
        std::string& assign ( const char* s, size_t n );
        std::string& assign ( const char* s );
        std::string& assign ( size_t n, char c );
        template <class InputIterator>
            std::string& assign ( InputIterator first, InputIterator last );
        
        std::string& insert ( size_t pos1, const std::string& str );
        std::string& insert ( size_t pos1, const std::string& str, size_t pos2, size_t n );
        std::string& insert ( size_t pos1, const char* s, size_t n);
        std::string& insert ( size_t pos1, const char* s );
        std::string& insert ( size_t pos1, size_t n, char c );
        std::string::iterator insert ( std::string::iterator p, char c );
        void insert ( std::string::iterator p, size_t n, char c );
        template<class InputIterator>
             void insert ( std::string::iterator p, InputIterator first, InputIterator last );
           
        std::string& replace ( size_t pos1, size_t n1,   const std::string& str );
        std::string& replace ( std::string::iterator i1, iterator i2, const std::string& str );
        std::string& replace ( size_t pos1, size_t n1, const std::string& str, size_t pos2, size_t n2 );
        std::string& replace ( size_t pos1, size_t n1,   const char* s, size_t n2 );
        std::string& replace ( std::string::iterator i1, std::string::iterator i2, const char* s, size_t n2 );
        std::string& replace ( size_t pos1, size_t n1,   const char* s );
        std::string& replace ( std::string::iterator i1, std::string::iterator i2, const char* s );
        std::string& replace ( size_t pos1, size_t n1,   size_t n2, char c );
        std::string& replace ( std::string::iterator i1, std::string::iterator i2, size_t n2, char c );
        template<class InputIterator>
            std::string& replace ( std::string::iterator i1, std::string::iterator i2, InputIterator j1, InputIterator j2 );
        
        void swap ( std::string& str );
        void swap ( FileString& fstr );
    private:
        void TestPathValidity();
};

#endif // FILESTRING_H
