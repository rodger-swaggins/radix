#include "glbl.h"
#include "primitives.h"
#include "bool.h"

#ifndef FLIST_HEADER_FILE
#define FLIST_HEADER_FILE
    /* ## private structs ##
     * These are structures used internally to represent a list. You won't
     * have to interface with them yourself - you'll only have to use them as
     * variable types.
     */
    typedef struct RadixListElement {
        // Represents the value in the list, which will also be fully compared
        // when searching for values.
        RadixMemoryBlob value;
        // Points to the next element in the list, as this is a linked list
        struct RadixListElement *next_element;
    } RadixListElement;

    // Root definition of the list, containing the length, a pointer to the
    // first element and a pointer to the last element.
    typedef struct RadixList {
        unsigned long long length;
        RadixListElement *first_element;
        RadixListElement *last_element;
    } RadixList;

    // A higher level index structure used to check if an index exists
    // while keeping it starting from 0.
    typedef struct RadixListIndex {
        bool present;
        unsigned long long index;
        // See RadixList_IndexStructExists and RadixList_IndexStructPosition
    } RadixListIndex;

    // List iterator for performing iterative operations on a list
    typedef struct RadixListIterator {
        // The list that is being iterated over
        RadixList *list;
        // The current element
        RadixListElement *element;
        // The current index
        unsigned long long index;
        // The next and previous elements
        RadixListElement *previous;
        RadixListElement *next;
        // Whether or not the iterator has been exhausted
        // (can also boolify element)
        bool exhausted;
    } RadixListIterator;

    // Result from querying the list
    typedef struct RadixListQueryResult {
        // Was the query successful?
        bool found;
        // The index that the query succeeded on
        RadixListIndex index;
        // The previous element, successful element and next element
        RadixListElement *previous;
        RadixListElement *current;
        RadixListElement *next;
    } RadixListQueryResult;

    // A query input - what to search for
    typedef struct RadixListQuery {
        // See linked_shared -> RadixIteratorQueryFlags for possible query_for
        // values.
        char query_for;
        // Value to search for. Can be NULL pointer.
        RadixMemoryBlob *value;
        // Index to query for. Can be 0.
        unsigned long long index;
        // Interval to use for QUERY_VALUE_CONTAINS
        unsigned int containsInterval;
    } RadixListQuery;

    // Sides to append/concatenate to the list
    enum RadixListSides {
        LIST_SIDE_LEFT = false,
        LIST_SIDE_RIGHT = true
    };

    // IMPORTANT: Read this for query flags
    #include "linked_shared.h"

    // Create a new list
    RadixList RadixList_New();

    // Get the list length, 0 if empty
    unsigned long long RadixList_Length(RadixList *list);

    // Make a new list iterator
    RadixListIterator RadixList_NewIterator(RadixList *list);

    // Get the list iterator's current element
    RadixListElement * RadixList_IteratorGet(RadixListIterator *ki);

    // Get the list iterator's previous element
    RadixListElement * RadixList_IteratorGetPrev(RadixListIterator *ki);

    // Get the list iterator's next element (from da future!)
    RadixListElement * RadixList_IteratorGetNext(RadixListIterator *ki);

    // Get the list iterator's current index
    unsigned long long RadixList_IteratorIndex(RadixListIterator *ki);

    // Move the list iterator to the next element in the list
    void RadixList_IteratorNext(RadixListIterator *ki);

    // Check if a list iterator's current element matches the given query
    bool RadixList_IteratorCheckElement(
        RadixListIterator *ki,
        RadixListQuery *kq);

    // Construct a query for the list - see the RadixListQuery struct
    RadixListQuery RadixList_ConstructQuery(
        char query_for,
        RadixMemoryBlob *value,
        unsigned long long index,
        unsigned int containsInterval);

    // Carry out a query on the list and return the first valid result.
    RadixListQueryResult RadixList_Query(
        RadixList *list,
        RadixListQuery query);

    // Find an element by index in the list.
    RadixListElement * RadixList_FindIndex(
        RadixList *list,
        unsigned long long index);

    // Find an element by value in the list.
    RadixListElement * RadixList_FindValue(
        RadixList *list,
        RadixMemoryBlob value);

    // Check if an index exists in the list
    bool RadixList_IndexIn(RadixList *list, unsigned long long index);

    // Check if a value exists in the list
    bool RadixList_ValueIn(RadixList *list, RadixMemoryBlob value);

    // Turn a list index into a value - you can modify this blob too!
    RadixMemoryBlob * RadixList_GetValue(
        RadixList *list,
        unsigned long long index);

    // Get an index by value, useful for Python-like remove()s.
    RadixListIndex RadixList_GetIndex(RadixList *list, RadixMemoryBlob value);

    // Set an existing index to the given value, returns false if the index
    // doesn't exist.
    bool RadixList_SetIndex(
        RadixList *list,
        unsigned long long index,
        RadixMemoryBlob value);

    // Insert a new element that will appear at the given position.
    // (E.g 0 places the element right at the start.)
    // Returns the position.
    unsigned long long RadixList_InsertNew(
        RadixList *list,
        unsigned long long new_position);

    // Insert a new element at the right or left side of the list, specified
    // by the RadixListSides enum. Returns the new position.
    unsigned long long RadixList_Append(RadixList *list, char side);

    // Same as RadixList_Append, but it appends an actual value rather than a
    // blank new element.
    unsigned long long RadixList_AppendValue(
        RadixList *list,
        char side,
        RadixMemoryBlob value);

    // Swap the values of two indexes in the list, useful for sorting.
    void RadixList_SwapIndexValues(
        RadixList *list,
        unsigned long long p0,
        unsigned long long p1);

    // Remove a given index from the list, deallocating the memory used.
    bool RadixList_DestroyIndex(RadixList *list, unsigned long long index);

    // Destroy the entire list, completely dealloacting all the memory it uses.
    void RadixList_DestroyList(RadixList *list);

    // Check if the index presented by a RadixListIndex struct exists.
    bool RadixList_IndexStructExists(RadixListIndex index);

    // Ditto, returns the position of a given RadixListIndex struct.
    unsigned long long RadixList_IndexStructPosition(RadixListIndex index);

    // Concatenate two lists at the given side, specified by the
    // RadixListSides enum.
    void RadixTable_Concatenate(RadixList *dest, RadixList *src, char side);
#endif
