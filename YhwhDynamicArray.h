#ifndef YHWH_DYNAMIC_ARRAY_H
#define YHWH_DYNAMIC_ARRAY_H

#include <Windows.h>

typedef struct YHWH_DYNAMIC_ARRAY {
	LPVOID Array;
	SIZE_T Size;
	SIZE_T Capacity;
	SIZE_T SizeOf;
} YHWH_DYNAMIC_ARRAY;

typedef VOID(*YHWH_DESTRUCTOR_PROCEDURE)(LPVOID);

#define YHWH_DYNAMIC_ARRAY_DECLARE(Name, SizeOf) YHWH_DYNAMIC_ARRAY Name;\
												 YhwhDynamicArrayConstructor(&Name, SizeOf)


inline VOID YhwhDynamicArrayConstructor(
	YHWH_DYNAMIC_ARRAY*				   Array,
	const SIZE_T					   SizeOf
);

VOID YhwhDynamicArrayDestructor(
	YHWH_DYNAMIC_ARRAY*		    Array,
	YHWH_DESTRUCTOR_PROCEDURE   Procedure
);

inline BOOLEAN YhwhDynamicArrayEmpty(
	const YHWH_DYNAMIC_ARRAY*       Array
);

inline SIZE_T YhwhDynamicArraySize(
	const YHWH_DYNAMIC_ARRAY*     Array
);

inline SIZE_T YhwhDynamicArrayMaxSize(
	const YHWH_DYNAMIC_ARRAY*	     Array
);

HRESULT YhwhDynamicArrayReserve(
	YHWH_DYNAMIC_ARRAY*		   Array,
	const SIZE_T			   Capacity
);

inline SIZE_T YhwhDynamicArrayCapacity(
	const YHWH_DYNAMIC_ARRAY*		  Array
);

HRESULT YhwhDynamicArrayShrinkToFit(
	YHWH_DYNAMIC_ARRAY*			   Array
);

VOID YhwhDynamicArrayClear(
	YHWH_DYNAMIC_ARRAY*		  Array,
	YHWH_DESTRUCTOR_PROCEDURE Procedure
);

HRESULT YhwhDynamicArrayInsert(
	YHWH_DYNAMIC_ARRAY*		  Array,
	LPCVOID					  Data,
	const SIZE_T			  DataSize,
	const SIZE_T			  Offset
);

HRESULT YhwhDynamicArrayInsertRange(
	YHWH_DYNAMIC_ARRAY*			   Array,
	LPCVOID						   DataArray,
	const SIZE_T				   DataSize,
	const SIZE_T				   Offset,
	const SIZE_T				   Elements
);

VOID YhwhDynamicArrayErase(
	YHWH_DYNAMIC_ARRAY*       Array,
	const SIZE_T		      Offset,
	YHWH_DESTRUCTOR_PROCEDURE Procedure
);

VOID YhwhDynamicArrayEraseRange(
	YHWH_DYNAMIC_ARRAY*		   Array,
	const SIZE_T			   Offset,
	const SIZE_T			   Elements,
	YHWH_DESTRUCTOR_PROCEDURE  Procedure
);

HRESULT YhwhDynamicArrayPushback(
	YHWH_DYNAMIC_ARRAY*		    Array,
	LPCVOID						Data,
	const SIZE_T				DataSize
);

VOID YhwhDynamicArrayPopback(
	YHWH_DYNAMIC_ARRAY*       Array,
	YHWH_DESTRUCTOR_PROCEDURE Procedure
);

inline LPVOID YhwhDynamicArrayData(
	YHWH_DYNAMIC_ARRAY*			  Array
);

inline LPVOID YhwhDynamicArraySubscript(
	YHWH_DYNAMIC_ARRAY*				   Array,
	const SIZE_T					   Offset
);

inline LPVOID YhwhDynamicArrayFront(
	YHWH_DYNAMIC_ARRAY*			   Array
);

inline LPVOID YhwhDynamicArrayBack(
	YHWH_DYNAMIC_ARRAY*			  Array
);

HRESULT YhwhDynamicArrayCopy(
	const YHWH_DYNAMIC_ARRAY* ConstArray,
	YHWH_DYNAMIC_ARRAY*		  Array,
	YHWH_DESTRUCTOR_PROCEDURE Procedure
);


#endif
