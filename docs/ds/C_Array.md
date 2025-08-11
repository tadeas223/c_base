# C_Array : ClassObject
**package:** [ds](ds.md)

**implements:**  
- **IHashable**: `C_Array_equals`, `C_Array_hash`
- **IFormattable**: `C_Array_to_str_format_R`, `C_Array_to_str_R`

---

## *overview*

`C_Array` is a datastructure that should replace normal C array where possible.
Can store objects which extend `ClassObject`.
Supports peeking and indexed access.
The array has a static length, and the elements can be only added with `C_Array_put_P`

- Stores references and manages ownership (with ref/unref)
- Not thread-safe

---
## **macros**

### **C_ArrayForeach(array, code)**
Iterates over all elements in the array.

exposes variables:
- `u32 iter`: index of the current value
- `value`: current value

example of printing all values from an array:
``` C
C_ArrayForeach(array, {
  console_write_single_ln(value);
});
```

## functions**

### **C_Array\* C_Array_new(u32 len)**
Creates and initializes a new `C_Array` instance.

**returns:**
- `C_Array*`: New instance of `C_Array`

---
### **void C_Array_destroy(void\* self)**
Destroys the array and unreferences all values.

**notes:**
- The array must not be used after this call.

---
### **void C_Array_put_P(C_Array\* self, u32 index, void\* value)**
Stores a value at the specified index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `index` >= length of the darray

**params:**
- `index`: Index where to store the value
- `value`: Value to store (reference is added)

---
### **void\* C_Array_at_B(C_Array\* self, u32 index);
Borrows the value at the specified index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `index` >= length of the darray

**params:**
- `index`: Index of the value

**returns:**
- `void*`: Borrowed reference

---
### **void\* C_Array_at_R(C_Array\* self, u32 index)**
Returns a reference to the value at the specified index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `index` >= length of the darray

**params:**
- `index`: Index of the value

**returns:**
- `void*`: Owned reference

---
### **void\* C_Array_peek_B(C_Array\* self)
Borrows the last element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `index` >= length of the darray

**returns:**
- `void*`: Borrowed reference to the last element

---
### **void\* C_Array_peek_R(C_Array\* self)**
Returns a reference to the last element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBounds, ...)`:
    if `index` >= length of the darray

**returns:**
- `void*`: Owned reference to the last element

---
### **void\* C_Array_peek_front_B(C_Array\* self)
Borrows the the first element.

**returns:**
- `void*`: Borrowed reference to the first element

---

### **void\* C_Array_peek_front_R(C_Array\* self)**
Returns a reference to the first element.

**returns:**
- `void*`: Owned reference to the first element

---
### **bool C_Array_equals(void\* a, void\* b)**
Checks all elements in both arrays. If the arrays contain the same elements this function returns true.

**returns:**
- `bool`: true if arrays are equal, false otherwise

---
### **u32 C_Array_hash(void\* self)**
Hashes the array by hashing all its elements and adding it together.

**notes:**
- the hash changes each time new elements are added or removed from the array

**returns:**
- `u32`: hash of the array

---
### **C_String\* C_Array_to_str_format_R(void\* self, C_String* format)**
Documented in [strings.h](../base/strings/strings.h)

---
### **C_String\* C_Array_to_str_R(void\* self, C_String* format)**
Documented in [strings.h](../base/strings/strings.h)

---
### **u32 C_Array_get_len(C_Array\* self)**
Returns the number of elements in the array.

**returns:**
- `u32`: Number of elements in the array
