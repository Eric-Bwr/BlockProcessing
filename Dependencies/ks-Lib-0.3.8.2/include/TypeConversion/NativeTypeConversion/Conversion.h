#pragma once
#include <vector>
#include <string>

//convert to c string (char*)
namespace ntc {
  uint64_t getCStringLen(const char* cstring);                                                                 //get the lenght from a c string
  char* getCombinedCString(const char* first, const char* second);                                              //combines two c string to a single one
  char* getCombinedCString(const char* first, char letter);                                               //combines a cstring and a single chat to a new c string
  bool isCStringEqual(const char* first, const char* second);                                                          //check first c string == second c string
  char* boolToCstring(bool value);                                                                  //convert an bool to a c string
  char* boolPtrToCstring(bool* value);                                                              //convert an bool pointer to a c string
  char* shortToCstring(short value);                                                                //convert a short to a c string
  char* shortPtrToCString(short* value);                                                            //convert a short pointer to a c string
  char* intToCstring(int value);                                                                    //convert a int to a c string
  char* intPtrToCString(int* value);                                                                //convert a int pointer to a c string
  char* longToCstring(long value);                                                                  //convert a long to a c string
  char* longPtrToCString(long* value);                                                              //convert a long pointer to a c string
  char* floatToCString(float value);
  char* floatPtrToCString(float* value);
  char* doubleToCString(double value);                                                              //convert a double to a c string
  char* doublePtrToCString(double* value);
  char* boolArrayToCstring(bool* array, int size);                                                  //convert an bool array to a c string with \n between each bool
  char* boolArrayToCstring2(bool* array, int size);                                                 //convert an bool array to a c string
  char** boolArrayToCstringArray(bool* array, int size, int& rSize);                                //convert an bool array to a c string array
  char* shortArrayToCstring(short* array, int size);                                                //convert an short array to a c string with \n between each bool
  char* shortArrayToCstring2(short* array, int size);                                               //convert an short array to a c string
  char** shortArrayToCstringArray(short* array, int size, int& rSize);                              //convert an short array to a c string array
  char* intArrayToCstring(int* array, int size);                                                    //convert an int array to a c string with \n between each bool
  char* intArrayToCstring2(int* array, int size);                                                   //convert an int array to a c string
  char** intArrayToCstringArray(int* array, int size, int rSize);                                   //convert an int array to a c string array
  char* longArrayToCstring(long* array, int size);                                                  //convert an long array to a c string with \n between each bool
  char* longArrayToCstring2(long* array, int size);                                                 //convert an long array to a c string
  char** longArrayToCstringArray(long* array, int size, int& rSize);                                //convert an long array to a c string array

  namespace Unsigned {
    char* unsignedShortToCstring(unsigned short value);                                             //convert a unsigned short to a c string
    char* unsignedShortPtrToCString(unsigned short* value);                                         //convert a unsigned short pointer to a c string
    char* unsignedIntToCstring(unsigned int value);                                                 //convert a unsigned int to a c string
    char* unsignedIntPtrToCString(unsigned int* value);                                             //convert a unsigned int pointer to a c string
    char* unsignedLongToCstring(unsigned long value);                                               //convert a unsigned long to a c string
    char* unsignedLongPtrToCString(unsigned long* value);                                           //convert a unsigned long pointer to a c string
    char* unsignedShortArrayToCstring(unsigned short* array, int size);                             //convert an unsigned short array to a c string with \n between each bool
    char* unsignedShortArrayToCstring2(unsigned short* array, int size);                            //convert an unsigned short array to a c string
    char** unsignedShortArrayToCstringArray(unsigned short* array, int size, int& rSize);           //convert an unsigned short array to a c string array
    char* unsignedIntArrayToCstring(unsigned int* array, int size);                                 //convert an unsigned int array to a c string with \n between each bool
    char* unsignedIntArrayToCstring2(unsigned int* array, int size);                                //convert an unsigned int array to a c string
    char** unsignedIntArrayToCstringArray(unsigned int* array, int size, int rSize);                //convert an unsigned int array to a c string array
    char* unsignedLongArrayToCstring(unsigned long* array, int size);                               //convert an unsigned long array to a c string with \n between each bool
    char* unsignedLongArrayToCstring2(unsigned long* array, int size);                              //convert an unsigned long array to a c string
    char** unsignedLongArrayToCstringArray(unsigned long* array, int size, int& rSize);             //convert an unsigned long array to a c string array
  } // namescape Unsigned

  //convert from c++ types
  namespace Cpp {
    char* stringVecToCString(std::vector<std::string>& vector);                                     //convert an std::vector<std::string> to a c string with \n between each c string
    char* stringVecToCString2(std::vector<std::string>& vector);                                    //convert an std::vector<std::string> to a c string
    char* stringPtrVecToCString(std::vector<std::string*>& vector);                                 //convert an std::vector<std::string*> to a c string with \n between each c string
    char* stringPtrVecToCString2(std::vector<std::string*>& vector);                                //convert an std::vector<std::string*> to a c string
    char** stringVecToCStringArray(std::vector<std::string>& vector, int& rSize);                   //convert an std::vector<std::string> to a c string array
    char** stringPtrVecToCStringArray(std::vector<std::string*>& vector, int& rSize);               //convert an std::vector<std::string*> to a c string array
    char* boolVecToCString(std::vector<bool> vector);                                               //convert an std::vector<bool> to a c string with \n between each c string
    char* boolVecToCString2(std::vector<bool> vector);                                              //convert an std::vector<bool> to a c string
    char* boolPtrVecToCString(std::vector<bool*> vector);                                           //convert an std::vector<bool*> to a c string
    char* boolPtrVecToCString2(std::vector<bool*> vector);                                          //convert an std::vector<bool*> to a c string with \n between each c string
    char** boolVecToCStringArray(std::vector<bool> vector, int& rSize);                             //convert an std::vector<bool> to a c string array
    char** boolPtrVecToCStringArray(std::vector<bool*> vector, int& rSize);                         //convert an std::vector<bool*> to a c string  array
    char* shortVecToCString(std::vector<short> vector);                                             //convert an std::vector<short> to a c string
    char* shortVecToCString2(std::vector<short> vector);                                             //convert an std::vector<short> to a c string
    char* shortPtrVecToCString(std::vector<short*> vector);                                         //convert an std::vector<short*> to a c string
    char* shortPtrVecToCString2(std::vector<short*> vector);                                         //convert an std::vector<short*> to a c string
    char** shortVecToCStringArray(std::vector<short> vector, int& rSize);                           //convert an std::vector<short> to a c string array
    char** shortPtrVecToCStringArray(std::vector<short*> vector, int& rSize);                       //convert an std::vector<short*> to a c string array
    char* intVecToCString(std::vector<int> vector);                                                 //convert an std::vector<int> to a c string
    char* intVecToCString2(std::vector<int> vector);                                                 //convert an std::vector<int> to a c string
    char* intPtrVecToCString(std::vector<int*> vector);                                             //convert an std::vector<int*> to a c string
    char* intPtrVecToCString2(std::vector<int*> vector);                                             //convert an std::vector<int*> to a c string
    char** intVecToCStringArray(std::vector<int> vector, int& rSize);                               //convert an std::vector<int> to a c string array
    char** intPtrVecToCStringArray(std::vector<int*> vector, int& rSize);                           //convert an std::vector<int*> to a c string array
    char* longVecToCString(std::vector<long> vector);                                               //convert an std::vector<long> to a c string
    char* longVecToCString2(std::vector<long> vector);                                               //convert an std::vector<long> to a c string
    char* longPtrVecToCString(std::vector<long*> vector);                                           //convert an std::vector<long*> to a c string
    char* longPtrVecToCString2(std::vector<long*> vector);                                           //convert an std::vector<long*> to a c string
    char** longVecToCStringArray(std::vector<long> vector, int& rSize);                             //convert an std::vector<long> to a c string array
    char** longPtrVecToCStringArray(std::vector<long*> vector, int& rSize);                         //convert an std::vector<long*> to a c string array
    char* floatVecToCString(std::vector<float> vector);                                             //convert an std::vector<float> to a c string
    char* floatVecToCString2(std::vector<float> vector);                                             //convert an std::vector<float> to a c string
    char* floatPtrVecToCString(std::vector<float*> vector);                                         //convert an std::vector<float*> to a c string
    char* floatPtrVecToCString2(std::vector<float*> vector);                                         //convert an std::vector<float*> to a c string
    char** floatVecToCStringArray(std::vector<float> vector, int& rSize);                           //convert an std::vector<float> to a c string array
    char** floatPtrVecToCStringArray(std::vector<float*> vector, int& rSize);                       //convert an std::vector<float*> to a c string array
    char* doubleVecToCString(std::vector<double> vector);                                           //convert an std::vector<double> to a c string
    char* doubleVecToCString2(std::vector<double> vector);                                           //convert an std::vector<double> to a c string
    char* doublePtrVecToCString(std::vector<double*> vector);                                       //convert an std::vector<double*> to a c string
    char* doublePtrVecToCString2(std::vector<double*> vector);                                       //convert an std::vector<double*> to a c string
    char** doubleVecToCStringArray(std::vector<double> vector, int& rSize);                         //convert an std::vector<double> to a c string array
    char** doublePtrVecToCStringArray(std::vector<double*> vector, int& rSize);                     //convert an std::vector<double*> to a c string array
    
    //convert from unsigned
    namespace Unsigned {
      char* unsignedShortVecToCString(std::vector<unsigned short> vector);                          //convert an std::vector<unsigned short> to a c string
      char* unsignedShortVecToCString2(std::vector<unsigned short> vector);                          //convert an std::vector<unsigned short> to a c string
      char* unsignedShortPtrVecToCString(std::vector<unsigned short*> vector);                      //convert an std::vector<unsigned short*> to a c string
      char* unsignedShortPtrVecToCString2(std::vector<unsigned short*> vector);                      //convert an std::vector<unsigned short*> to a c string
      char** unsignedShortVecToCStringArray(std::vector<unsigned short> vector, int& rSize);        //convert an std::vector<unsigned short> to a c string array
      char** unsignedShortPtrVecToCStringArray(std::vector<unsigned short*> vector, int& rSize);    //convert an std::vector<unsigned short*> to a c string array
      char* unsignedIntVecToCString(std::vector<unsigned int> vector);                              //convert an std::vector<unsigned int> to a c string
      char* unsignedIntVecToCString2(std::vector<unsigned int> vector);                              //convert an std::vector<unsigned int> to a c string
      char* unsignedIntPtrVecToCString(std::vector<unsigned int*> vector);                          //convert an std::vector<unsigned int*> to a c string
      char* unsignedIntPtrVecToCString2(std::vector<unsigned int*> vector);                          //convert an std::vector<unsigned int*> to a c string
      char** unsignedIntVecToCStringArray(std::vector<unsigned int> vector, int& rSize);            //convert an std::vector<unsigned int> to a c string array
      char** unsignedIntPtrVecToCStringArray(std::vector<unsigned int*> vector, int& rSize);        //convert an std::vector<unsigned int*> to a c string array
      char* unsignedLongVecToCString(std::vector<unsigned long> vector);                            //convert an std::vector<unsigned long> to a c string
      char* unsignedLongVecToCString2(std::vector<unsigned long> vector);                            //convert an std::vector<unsigned long> to a c string
      char* unsignedLongPtrVecToCString(std::vector<unsigned long*> vector);                        //convert an std::vector<unsigned long*> to a c string
      char* unsignedLongPtrVecToCString2(std::vector<unsigned long*> vector);                        //convert an std::vector<unsigned long*> to a c string
      char** unsignedLongVecToCStringArray(std::vector<unsigned long> vector, int& rSize);          //convert an std::vector<unsigned long> to a c string array
      char** unsignedLongPtrVecToCStringArray(std::vector<unsigned long*> vector, int& rSize);      //convert an std::vector<unsigned long*> to a c string array
    } // namespace Unsigned
  } // namespace Cpp
} // namespace NTC

//convert to std::string
namespace ntc {
  std::string boolArrayToString(bool* array, int size);
  std::string shortArrayToString(short* array, int size);
  std::string intArrayToString(int* array, int size);
  std::string longArrayToString(long* array, int size);
  std::string floatArrayToString(float* array, int size);
  std::string doubleArrayToString(double* array, int size);

  //convert from unsigned
  namespace Unsigned {
    std::string unsignedShortArrayToString(unsigned short* array, int size);
    std::string unsignedIntArrayToString(unsigned int* array, int size);
    std::string unsignedLongArrayToString(unsigned long* array, int size);
  } // namespace unsigned

  //convert to std::string*
  namespace Ptr {
    std::string* boolArrayToStringPtr(bool* array, int size);
    std::string* shortArrayToStringPtr(short* array, int size);
    std::string* intArrayToStringPtr(int* array, int size);
    std::string* longArrayToStringPtr(long* array, int size);
    std::string* floatArrayToStringPtr(float* array, int size);
    std::string* doubleArrayToStringPtr(double* array, int size);

    //convert from unsigned
    namespace Unsigned {
      std::string* unsignedShortArrayToStringPtr(unsigned short* array, int size);
      std::string* unsignedIntArrayToStringPtr(unsigned int* array, int size);
      std::string* unsignedLongArrayToStringPtr(unsigned long* array, int size);
    } // namespace unsigned
  } //namespace Ptr

  //convert from c++ types
  namespace ntc {
    std::string boolVecToString(std::vector<bool> vector);
    std::string boolPtrVecToString(std::vector<bool*> vector);
    std::string shortVecToString(std::vector<short> vector);
    std::string shortPtrVecToString(std::vector<short*> vector);
    std::string intVecToString(std::vector<int> vector);
    std::string intPtrVecToString(std::vector<int*> vector);
    std::string longVecToString(std::vector<long> vector);
    std::string longPtrVecToString(std::vector<long*> vector);
    std::string floatVecToString(std::vector<float> vector);
    std::string floatPtrVecToString(std::vector<float*> vector);
    std::string doubleVecToString(std::vector<double> vector);
    std::string doublePtrVecToString(std::vector<double*> vector);

    //convert from unsigned
    namespace Unsigned {
      std::string unsignedShortVecToString(std::vector<unsigned short> vector);
      std::string unsignedShortPtrVecToString(std::vector<unsigned short*> vector);
      std::string unsignedIntVecToString(std::vector<unsigned int> vector);
      std::string unsignedIntPtrVecToString(std::vector<unsigned int*> vector);
      std::string unsignedLongVecToString(std::vector<unsigned long> vector);
      std::string unsignedLongPtrVecToString(std::vector<unsigned long*> vector);
    } // namespace unsigned

    //convert to std::string*
    namespace Ptr {
      std::string* boolVecToStringPtr(std::vector<bool> vector);
      std::string* boolPtrVecToStringPtr(std::vector<bool*> vector);
      std::string* shortVecToStringPtr(std::vector<short> vector);
      std::string* shortPtrVecToStringPtr(std::vector<short*> vector);
      std::string* intVecToStringPtr(std::vector<int> vector);
      std::string* intPtrVecToStringPtr(std::vector<int*> vector);
      std::string* longVecToStringPtr(std::vector<long> vector);
      std::string* longPtrVecToStringPtr(std::vector<long*> vector);
      std::string* floatVecToStringPtr(std::vector<float> vector);
      std::string* floatPtrVecToStringPtr(std::vector<float*> vector);
      std::string* doubleVecToStringPtr(std::vector<double> vector);
      std::string* doublePtrVecToStringPtr(std::vector<double*> vector);

      //convert from unsigned
      namespace Unsigned {
        std::string* unsignedShortVecToStringPtr(std::vector<unsigned short> vector);
        std::string* unsignedShortPtrVecToStringPtr(std::vector<unsigned short*> vector);
        std::string* unsignedIntVecToStringPtr(std::vector<unsigned int> vector);
        std::string* unsignedIntPtrVecToStringPtr(std::vector<unsigned int*> vector);
        std::string* unsignedLongVecToStringPtr(std::vector<unsigned long> vector);
        std::string* unsignedLongPtrVecToStringPtr(std::vector<unsigned long*> vector);
      } // namespace unsigned
    } // namespace Ptr
  } // namespace Cpp
} // namespace String

//convert to integer
namespace ntc {

  //convert from unsigned
  namespace Unsigned {

  } // namespace Unsigned

  //convert to integer pointer
  namespace Ptr {
   
    //convert from unsigned
    namespace Unsigned {

    } // namespace Unsigned
  } // namespace Ptr

  //convert from c++ types
  namespace ntc {

    //convert from unsigned
    namespace Unsigned {

    } // namespace Unsigned

    //convert to integer pointer
    namespace Ptr {
      bool* boolVecToBoolArray(std::vector<bool> vector, int& rSize);
      bool* boolVecToBoolArray(std::vector<bool*> vector, int& rSize);
      short* shortVecToShortArray(std::vector<short> vector, int& rSize);
      short* shortVecToShortArray(std::vector<short*> vector, int& rSize);
      int* intVecToIntArray(std::vector<int> vector, int& rSize);
      int* intVecToIntArray(std::vector<int*> vector, int& rSize);
      long* longVecToLongArray(std::vector<long> vector, int& rSize);
      long* longVecToLongArray(std::vector<long*> vector, int& rSize);
      
      //convert from unsigned
      namespace Unsigned {
        unsigned short* unsignedShortVecToUnsignedShortArray(std::vector<unsigned short> vector, int& rSize);
        unsigned short* unsignedShortVecToUnsignedShortArray(std::vector<unsigned short*> vector, int& rSize);
        unsigned int* unsignedIntVecToUnsignedIntArray(std::vector<unsigned int> vector, int& rSize);
        unsigned int* unsignedIntVecToUnsignedIntArray(std::vector<unsigned int*> vector, int& rSize);
        unsigned long* unsignedLongVecToUnsignedLongArray(std::vector<unsigned long> vector, int& rSize);
        unsigned long* unsignedLongVecToUnsignedLongArray(std::vector<unsigned long*> vector, int& rSize);
      } // namespace Unsigned
    } // namespace Ptr
  } // namespace Cpp
} // namespace Integer

namespace ntc {

  //convert to double pointer
  namespace Ptr {

  } // namespace Ptr

  //convert from c++ types
  namespace Cpp {

    //convert to double pointer
    namespace Ptr {

    } // namespace Ptr
  } // namespace Cpp
} // namespace Double

//Convert to std::Vector<>
namespace ntc {
  //convert to vector<>* pointer
  namespace Ptr {

  } // namespace Ptr

  //convert from c++ types
  namespace Cpp {

    //convert to vector<>* pointer
    namespace Ptr {

    } // namespace Ptr
  } // namespace Cpp
} // namespace Vector