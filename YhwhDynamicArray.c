#include "YhwhDynamicArray.h"
#include <assert.h>

inline VOID YhwhDynamicArrayConstructor(
	YHWH_DYNAMIC_ARRAY*				   Array,
	const SIZE_T					   SizeOf
) {
	assert(Array != NULL);
	assert(SizeOf != 0);
	Array->Array    = NULL;
	Array->Size     = 0;
	Array->Capacity = 0;
	Array->SizeOf   = SizeOf;
}

VOID YhwhDynamicArrayDestructor(
	YHWH_DYNAMIC_ARRAY*				  Array,
	YHWH_DESTRUCTOR_PROCEDURE         Procedure
) {
	assert(Array != NULL);
	assert(Array->SizeOf != 0);

	if (Array->Array == NULL) return;

	if (Procedure != NULL) {
		for (SIZE_T i = 0; i < Array->Size; i++) {
			Procedure((LPBYTE)Array->Array + (i * Array->SizeOf));
		}
	}

	(VOID)HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, Array->Array);
	
	Array->Array    = NULL;
	Array->Size     = 0;
	Array->Capacity = 0;
	Array->SizeOf   = 0;

}

inline BOOLEAN YhwhDynamicArrayEmpty(
	const YHWH_DYNAMIC_ARRAY*		Array
) {
	assert(Array != NULL);
	return (Array->Array == NULL) ? TRUE : FALSE;
}

inline SIZE_T YhwhDynamicArraySize(
	const YHWH_DYNAMIC_ARRAY*	   Array
) {
	assert(Array != NULL);
	return Array->Size;
}

inline SIZE_T YhwhDynamicArrayMaxSize(
	const YHWH_DYNAMIC_ARRAY*		  Array
) {
	assert(Array != NULL);
	return (SIZE_T)-1;
}

HRESULT YhwhDynamicArrayReserve(
	YHWH_DYNAMIC_ARRAY*				  Array,
	const SIZE_T					  Capacity
) {
	assert(Array != NULL);
	assert(Capacity != 0);
	assert(Array->SizeOf != 0);
	if (Capacity > Array->Capacity) {
		LPVOID Buffer = (LPVOID)HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, Array->SizeOf * Capacity);
		if (Buffer == NULL) return E_OUTOFMEMORY;
		if (Array->Array != NULL) {
			memcpy(Buffer, Array->Array, Array->SizeOf * Array->Size);
			(VOID)HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, Array->Array);
		}
		Array->Array    = Buffer;
		Array->Capacity = Capacity;
	}

	return S_OK;
}

inline SIZE_T YhwhDynamicArrayCapacity(
	const YHWH_DYNAMIC_ARRAY*		  Array
) {
	assert(Array != NULL);
	return Array->Capacity;
}

HRESULT YhwhDynamicArrayShrinkToFit(
	YHWH_DYNAMIC_ARRAY*			   Array
) {
	assert(Array != NULL);
	assert(Array->SizeOf != 0);
	if (Array->Size < Array->Capacity) return S_FALSE;
	LPVOID Buffer = HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, Array->SizeOf * Array->Size);
	if (Buffer == NULL) return E_OUTOFMEMORY;
	memcpy(Buffer, Array->Array, Array->SizeOf * Array->Size);
	return S_OK;
}

VOID YhwhDynamicArrayClear(
	YHWH_DYNAMIC_ARRAY*		  Array,
	YHWH_DESTRUCTOR_PROCEDURE Procedure
) {
	assert(Array != NULL);
	if (Array->Array == NULL) return;
	assert(Array->SizeOf != 0);

	if (Procedure != NULL) {
		for (SIZE_T i = 0; i < Array->Size; i++) {
			Procedure((LPBYTE)Array->Array + (Array->SizeOf * i));
		}
	}

	(VOID)HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, Array->Array);
	Array->Array    = NULL;
	Array->Size     = 0;
	Array->Capacity = 0;

}

HRESULT YhwhDynamicArrayInsert(
	YHWH_DYNAMIC_ARRAY*		  Array,
	LPCVOID					  Data,
	const SIZE_T			  DataSize,
	const SIZE_T			  Offset
) {
	assert(Array    != NULL);
	assert(Data     != NULL);
	assert(Offset   > Array->Size);
	assert(Array->SizeOf != 0);
	assert(DataSize == Array->SizeOf);

	HRESULT Result = S_OK;

	if (Array->Size == Array->Capacity) {
		Result = YhwhDynamicArrayReserve(Array, Array->Capacity * 2);
		if (FAILED(Result)) return Result;
	}

	if (Offset == Array->Size) {
		memcpy((LPCBYTE)Array->Array + (Array->SizeOf * Array->Size), Data, Array->SizeOf);
		Result = S_OK;
		return Result;
	}

	memmove((LPBYTE)Array->Array + (Array->SizeOf * Offset + 1), (LPCBYTE)Array->Array + (Array->SizeOf * Offset), (Array->Size - Offset) * Array->SizeOf);
	memcpy((LPBYTE)Array->Array + (Array->SizeOf * Offset), Data, Array->SizeOf);
	Array->Size++;
	Result = S_OK;
	return Result;
}

HRESULT YhwhDynamicArrayInsertRange(
	YHWH_DYNAMIC_ARRAY*			   Array,
	LPCVOID						   DataArray,
	const SIZE_T				   DataSize,
	const SIZE_T				   Offset,
	const SIZE_T				   Elements
) {
	assert(Offset > Array->Size);
	assert(Array->SizeOf != 0);
	assert(DataSize == Array->SizeOf);
	assert(Array != NULL);
	assert(Elements != 0);
	assert(DataArray != NULL);

	HRESULT Result = S_OK;

	if (Array->Size + Elements > Array->Capacity) {
		Result = YhwhDynamicArrayReserve(Array, Array->Size + Elements);
		if (FAILED(Result)) {
			return Result;
		}
	}

	memmove((LPBYTE)Array->Array + (Array->SizeOf * (Offset + 1 + Elements)), (LPCBYTE)Array->Array + (Array->SizeOf * Offset), (Array->Size - Offset) * Array->SizeOf);
	memcpy((LPBYTE)Array->Array + (Array->SizeOf * Offset), DataArray, Array->SizeOf * Elements);
	Array->Size += Elements;
	Result = S_OK;
	return Result;
}

VOID YhwhDynamicArrayErase(
	YHWH_DYNAMIC_ARRAY*		  Array,
	const SIZE_T		      Offset,
	YHWH_DESTRUCTOR_PROCEDURE Procedure
) {
	assert(Array != NULL);
	assert(Array->SizeOf != 0);
	assert(Offset < Array->Size);
	
	if (Procedure != NULL) {
		Procedure((LPBYTE)Array->Array + (Array->SizeOf * Offset));
	}

	if (Offset == 0 && Array->Size > 1) {
		memmove(Array->Array, (LPCBYTE)Array->Array + Array->SizeOf, Array->SizeOf * (Array->Size - 1));
	} else {
		memmove((LPBYTE)Array->Array + (Array->SizeOf * (Offset - 1)), (LPCBYTE)Array->Array + (Array->SizeOf * (Offset + 1)), (Array->Size - Offset) * Array->SizeOf);
	}

	Array->Size--;
	return;

}

VOID YhwhDynamicArrayEraseRange(
	YHWH_DYNAMIC_ARRAY*		   Array,
	const SIZE_T			   Offset,
	const SIZE_T			   Elements,
	YHWH_DESTRUCTOR_PROCEDURE  Procedure
) {
	assert(Array != NULL);
	assert(Array->SizeOf != 0);
	assert(Offset < Array->Size);
	assert(Elements != 0 && (Offset + Elements) < Array->Size);

	if (Procedure != NULL) {
		for (SIZE_T i = Offset; i < Offset + Elements; i++) {
			Procedure((LPBYTE)Array->Array + (Array->SizeOf * i));
		}
	}

	if (Offset == 0) {
		memmove(Array->Array, (LPCBYTE)Array->Array + (Offset + Elements) * Array->SizeOf, (Array->Size - (Offset - Elements)) * Array->SizeOf);
	} else {
		memmove((LPBYTE)Array->Array + (Offset - 1) * Array->SizeOf, (LPCBYTE)Array->Array + (Offset + Elements + 1) * Array->SizeOf, (Array->Size - (Offset - Elements)) * Array->SizeOf);
	}

	Array->Size -= Elements;
	return;
}

HRESULT YhwhDynamicArrayPushback(
	YHWH_DYNAMIC_ARRAY*			Array,
	LPCVOID						Data,
	const SIZE_T				DataSize
) {
	assert(Array != NULL);
	assert(Data != NULL);
	assert(Array->SizeOf != 0);
	assert(DataSize == Array->SizeOf);
	

	HRESULT Result;

	if (Array->Size == Array->Capacity) {
		Result = YhwhDynamicArrayReserve(Array, Array->Capacity * 2);
		if (FAILED(Result)) return Result;
	}

	memcpy((LPBYTE)Array->Array + (Array->SizeOf * Array->Size), Data, Array->SizeOf);
	Array->Size++;
	Result = S_OK;
	return Result;
}

VOID YhwhDynamicArrayPopback(
	YHWH_DYNAMIC_ARRAY*		  Array,
	YHWH_DESTRUCTOR_PROCEDURE Procedure
) {
	assert(Array != NULL);
	assert(Array->Size != 0);
	assert(Array->SizeOf != 0);

	if (Procedure != NULL) {
		Procedure((LPBYTE)Array->Array + (Array->SizeOf * Array->Size));
	}

	Array->Size--;
}

inline LPVOID YhwhDynamicArrayData(
	YHWH_DYNAMIC_ARRAY*		      Array
) {
	assert(Array		!= NULL);
	return Array->Array;
}

inline LPVOID YhwhDynamicArraySubscript(
	YHWH_DYNAMIC_ARRAY*				   Array,
	const SIZE_T					   Offset
) {
	assert(Array != NULL);
	assert(Offset < Array->Size);
	assert(Array->SizeOf != 0);

	return (LPBYTE)Array->Array + (Array->SizeOf * Offset);
}

inline LPVOID YhwhDynamicArrayFront(
	YHWH_DYNAMIC_ARRAY*			   Array
) {
	return Array->Array;
}

inline LPVOID YhwhDynamicArrayBack(
	YHWH_DYNAMIC_ARRAY*			  Array
) {
	if (Array->Size != 0) {
		return (LPBYTE)Array->Array + (Array->SizeOf * (Array->Size - 1));
	} else {
		return NULL;
	}
}

HRESULT YhwhDynamicArrayCopy(
	const YHWH_DYNAMIC_ARRAY* ConstArray,
	YHWH_DYNAMIC_ARRAY*		  Array,
	YHWH_DESTRUCTOR_PROCEDURE Procedure
) {
	assert(ConstArray != NULL);
	assert(ConstArray->SizeOf != 0);
	assert(Array != NULL);

	YHWH_DYNAMIC_ARRAY_DECLARE(Temp, ConstArray->SizeOf);

	HRESULT Result = S_OK;
	Result = YhwhDynamicArrayReserve(&Temp, ConstArray->Capacity);
	if (FAILED(Result)) {
		return Result;
	}

	memcpy(Temp.Array, ConstArray->Array, ConstArray->SizeOf * ConstArray->Size);
	Temp.Size = ConstArray->Size;

	YhwhDynamicArrayDestructor(&Array, Procedure);

	memcpy(Array, &Temp, sizeof(YHWH_DYNAMIC_ARRAY));
	Result = S_OK;
	return Result;
}