
# **C_DArray** : **ClassObject**
**package:** [ds](ds.md)

**implements:**  
- **IHashable**: `C_DArray_equals`, `C_DArray_hash`
- **IFormattable**: `C_DArray_to_str_R`, `C_DArray_to_str_format_R`

---

## **overview**

`C_DArray` stores objects which extend `ClassObject`.  
Supports pushing, popping, peeking, and indexed access.

- Stores references and manages ownership (with ref/unref)
- Not thread-safe

---
## **macros**

### **C_DArrayForeach(darray, code)**
> *tested*

Iterates over all elements in the darray.

exposes variables:
- `u32 iter`: index of the current value
- `value`: current value (borrowed)

example of printing all values from a darray:
``` C
C_DArrayForeach(darray, {
  console_write_single_ln(value);
});
```

## **functions**

### **C_DArray\* C_DArray_new_cap(u32 cap)**
> *tested*

Creates and initializes a new `C_DArray` instance and preallocates space for `cap` elements.

**returns:**
- `C_DArray*`: new instance of `C_DArray`

---
### **C_DArray\* C_DArray_new(void)**
> *tested*

Creates and initializes a new `C_DArray` instance.

**returns:**
- `C_DArray*`: new instance of `C_DArray`

---
### **void C_DArray_destroy(void\* self)**
> *tested*

Destroys the array and unreferences all values.

**params:**
- `self`: pointer to the **C_DArray** instance

**notes:**
- The darray must not be used after this call.

---
### **C_Array\* C_DArray_to_array_PR(C_DArray\* self)**
> *tested*

Converts the darray into a `C_Array`.

**params:**
- `self`: pointer to the darray

**returns:**
- `C_Array*`: array containing all elements from the darray 

---
### **void C_DArray_push_P(C_DArray\* self, void\* value)**
> *tested*

Appends a value to the end of the darray.

**params:**
- `self`: the darray 
- `value`: the object to store (reference is added)

---
### **void C_DArray_push_front_P(C_DArray\* self, void\* value)**
> *tested*

Prepends a value to the start of the darray.

**params:**
- `self`: the darray
- `value`: the object to store (reference is added)

---
### **void\* C_DArray_pop_R(C_DArray\* self)** 
> *tested*

Removes and returns the last element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if the darray is empty

**returns:**
- `void*`: referenced object

---
### **void\* C_DArray_pop_front_R(C_DArray\* self)**
> *tested*

Removes and returns the first element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if the darray is empty

**returns:**
- `void*`: referenced object

---
### **void\* C_DArray_peek_B(C_DArray\* self)**
> *tested*

Borrows the last element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if the darray is empty

**returns:**
- `void*`: borrowed reference (do not unref)

---
### **void\* C_DArray_peek_R(C_DArray\* self)**
> *tested*

Returns a reference to the last element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if the darray is empty


**returns:**
- `void*`: owned reference (must be unref’d by user)

---
### **void\* C_DArray_peek_front_B(C_DArray\* self)**
> *tested*

Borrows the first element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if the darray is empty


**returns:**
- `void*`: borrowed reference to the first element

---
### **void\* C_DArray_peek_front_R(C_DArray\* self)**
> *tested*

Returns a reference to the first element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if the darray is empty


**returns:**
- `void*`: owned reference to the first element

---
### **void C_DArray_add_P(C_DArray\* self, u32 index, void\* value)**
> *tested*

Adds a value at the specified index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `index` > length of the darray


**params:**
- `index`: index where to insert
- `value`: balue to store (reference is added)

---
### **void\* C_DArray_at_B(C_DArray\* self, u32 index)**
> *tested*

Borrows the value at the specified index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `index` >= length of the darray

**params:**
- `index`: index of the value

**returns:**
- `void*`: borrowed reference

---
### **void\* C_DArray_at_R(C_DArray\* self, u32 index)**
> *tested*

Returns a reference to the value at the specified index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `index` >= length of the darray

**params:**
- `index`: index of the value

**returns:**
- `void*`: owned reference

---
### **void\* C_DArray_remove_R(C_DArray\* self, u32 index)**
> *tested*

Removes a value at the given index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `index` >= length of the darray

**params:**
- `index`: index of the value to remove

**returns:**
- `void*`: referenced value

---
### **void C_DArray_resize(C_DArray\* self, u32 cap)**
> *tested*

Changes the capacity to of the darray.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `cap` < length of the darray
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `cap` == 0

**params:**
- `cap`: the new capacity

---
### **void C_DArray_compress(C_DArray\* self)**
> *tested*

Changes the capacity to the length of the darray.

---
### **void C_DArray_clear(C_DArray\* self)**
Removes all elements and unreferences them.

- DArray remains valid after clearing.
- Length is reset to 0.

---
### **u32 C_DArray_hash(C_DArray\* self)**
> *not tested*: cannot test

Hashes the darray by hashing all its elements and adding it together.

**notes:**
- the hash changes each time new elements are added or removed from the darray.

**returns:**
- `u32`: hash of the darray 

---
### **bool C_DArray_equals(void\* a, void\* b)**
> *tested*

Checks all elements in both darrays. 
If the darrays contain the same elements this function returns true.

**returns:**
- `bool`: true if darrays are equal, false otherwise

---
### **C_String\* C_DArray_to_str_format_R(void\* self, C_String\* format)**
> *tested*

Converts the darray into a string.

**format:**
``` format
{start}element{sep}element{sep}element{end}
```

**returns:** 
- `C_String*`: darray converted into a string

---
### **C_String\* C_DArray_to_str_R(void\* self, C_String\* format)**
> *not tested*: format may change in the future

Converts the darray into a string.
``` C
C_DArray_to_str_format_R(self, S("start=[;end=];sep=, ")
```

**format:**
``` format
[element, element, element]
```

**returns:** 
- `C_String*`: darray converted into a string

---
### **u32 C_DArray_get_len(C_DArray\* self)**
> *not tested*: too simple

Returns the number of elements in the darray.

**returns:**
- `u32`: number of elements in the darray 

---
### **u32 C_DArray_get_cap(C_DArray\* self)**
> *not tested*: too simple

Returns the current capacity of the darray.

**returns:**
- `u32`: capacity of the darray
