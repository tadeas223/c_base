# **C_List** : **ClassObject**
**package:** [ds](ds.md)

**implements:**  
- **IHashable**: `C_List_equals`, `C_List_hash`
- **IFormattable**: `C_List_to_str_R`, `C_List_to_str_format_R`

---

## **overview**

`C_List` is a singly linked list that stores objects which extend `ClassObject`.  
Supports pushing, popping, peeking, and indexed access.

- Stores references and manages ownership (with ref/unref)
- Not thread-safe
- Append is **O(N)**, prepend is **O(1)**

---
## **macros**

### **C_ListForeach(list, code)**
> *tested*

Iterates over all elements in the list.

exposes variables:
- `u32 iter`: index of the current value
- `value`: current value

example of printing all values from a list:
``` C
C_ListForeach(list, {
  console_write_single_ln(value);
});
```

## **functions**

### **C_List\* C_List_new(void)**
> *tested*

Creates and initializes a new `C_List` instance.

**returns:**
- `C_List*`: New instance of `C_List`

---

### **void C_List_destroy(void\* self)**
> *tested*

Destroys the list and unreferences all values.

**params:**
- `self`: Pointer to the **C_List** instance

**notes:**
- The list must not be used after this call.

---

### **C_Array\* C_List_to_array_PR(C_List\* self)**
> *tested*

Converts the list into a `C_Array`.

**params:**
- `self`: Pointer to the list

**returns:**
- `C_Array*`: Array containing all elements from the list

---

### **void C_List_push_P(C_List\* self, void\* value)**
> *tested*

Appends a value to the end of the list.

**params:**
- `self`: The list
- `value`: The object to store (reference is added)

---

### **void C_List_push_front_P(C_List\* self, void\* value)**
> *tested*

Prepends a value to the start of the list.

**params:**
- `self`: The list
- `value`: The object to store (reference is added)

---

### **void\* C_List_pop_R(C_List\* self)**
> *tested*

Removes and returns the last element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if the list is empty

**returns:**
- `void*`: Referenced object

---

### **void\* C_List_pop_front_R(C_List\* self)**
> *tested*

Removes and returns the first element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if the list is empty

**returns:**
- `void*`: Referenced object

---

### **void\* C_List_peek_B(C_List\* self)**
> *tested*

Borrows the last element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if the list is empty

**returns:**
- `void*`: Borrowed reference (do not unref)

---

### **void\* C_List_peek_R(C_List\* self)**
> *tested*

Returns a reference to the last element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if the list is empty

**returns:**
- `void*`: Owned reference (must be unref’d by user)

---

### **void\* C_List_peek_front_B(C_List\* self)**
> *tested*

Borrows the first element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if the list is empty

**returns:**
- `void*`: Borrowed reference to the first element

---

### **void\* C_List_peek_front_R(C_List\* self)**
> *tested*

Returns a reference to the first element.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if the list is empty

**returns:**
- `void*`: Owned reference to the first element

---

### **void C_List_add_P(C_List\* self, u32 index, void\* value)**
> *tested*

Adds a value at the specified index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if `index` > length of the list

**params:**
- `index`: Index where to insert
- `value`: Value to store (reference is added)

---

### **void\* C_List_at_B(C_List\* self, u32 index)**
> *tested*

Borrows the value at the specified index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if the list is empty
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if `index` >= length of the list

**params:**
- `index`: Index of the value

**returns:**
- `void*`: Borrowed reference

---

### **void\* C_List_at_R(C_List\* self, u32 index)**
> *tested*

Returns a reference to the value at the specified index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if the list is empty
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if `index` >= length of the list

**params:**
- `index`: Index of the value

**returns:**
- `void*`: Owned reference

---

### **void\* C_List_remove_R(C_List\* self, u32 index)**
> *tested*

Removes a value at the given index.

**crashes:**
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if the list is empty
- `E(EG_Datastructures, E_OutOfBouds, ...)`:
    if `index` >= length of the list

**params:**
- `index`: Index of the value to remove

**returns:**
- `void*`: Referenced value

---

### **void C_List_clear(C_List\* self)**
> *tested*

Removes all elements and unreferences them.

- List remains valid after clearing.
- Length is reset to 0.

---
### **u32 C_List_hash(C_List\* self)**
> *not tested*: cannot test

Hashes the list by hashing all its elements and adding it together.

**notes:**
- the hash changes each time new elements are added or removed from the list.

**returns:**
- `u32`: hash of the list

---
### **bool C_List_equals(void\* a, void\* b)**
> *tested*

Checks all elements in both lists. If the lists contain the same elements this function returns true.

**returns:**
- `bool`: true if lists are equal, false otherwise

---
### **C_String\* C_List_to_str_format_R(void\* self, C_String\* format)**
> *tested*

Converts the list into a string.

**format:**
``` format
{start}element{sep}element{sep}element{end}
```

**returns:** 
- `C_String*`: list converted into a string

---
### **C_String\* C_List_to_str_R(void\* self, C_String\* format)**
> *not tested*: format may change in the future

Converts the list into a string.
``` C
C_List_to_str_format_R(self, S("start=[;end=];sep=, ")
```

**format:**
``` format
[element, element, element]
```

**returns:** 
- `C_String*`: list converted into a string

---
### **u32 C_List_get_len(C_List\* self)**
> *not tested*: too simple

Returns the number of elements in the list.

**returns:**
- `u32`: Number of elements in the list

